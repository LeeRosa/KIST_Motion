#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO08x.h>

#include "BluetoothSerial.h"

Adafruit_BNO08x  bno08x;
sh2_SensorValue_t sensorValue;
BluetoothSerial bt;

// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress[] = {0xA8, 0x03, 0x2A, 0xEA, 0xB6, 0xB8};
// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  int id;
  float real;
  float i;
  float j;
  float k;
} struct_message;


struct_message BNO08X_incomingReadings;
// Create a struct_message to hold incoming sensor readings
struct_message BNO08X_incomingReadings_01;
struct_message BNO08X_incomingReadings_02;

struct_message BNO08X_struct[2] = {BNO08X_incomingReadings_01, BNO08X_incomingReadings_02};

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&BNO08X_incomingReadings, incomingData, sizeof(BNO08X_incomingReadings));
  //Serial.print("Bytes received: ");
  //Serial.println(len);

  BNO08X_struct[BNO08X_incomingReadings.id - 1].real = BNO08X_incomingReadings.real;
  BNO08X_struct[BNO08X_incomingReadings.id - 1].i = BNO08X_incomingReadings.i;
  BNO08X_struct[BNO08X_incomingReadings.id - 1].j = BNO08X_incomingReadings.j;
  BNO08X_struct[BNO08X_incomingReadings.id - 1].k = BNO08X_incomingReadings.k;

  bno();
}

void bno() {
  //switch (sensorValue.sensorId) {
  //  case SH2_GAME_ROTATION_VECTOR:
      if(bt.read()=='a'){

      for ( int i = 0; i < 2; i++) {
        bt.print((float)BNO08X_struct[i].real);
        bt.print(",");
        bt.print((float)BNO08X_struct[i].i);
        bt.print(",");
        bt.print((float)BNO08X_struct[i].j);
        bt.print(",");
        bt.print((float)BNO08X_struct[i].k);
        bt.print(";");
      }

      //      //Serial.print("com7--");
      bt.print((float)sensorValue.un.gameRotationVector.real, 2);
      bt.print(",");
      bt.print((float)sensorValue.un.gameRotationVector.i, 2);
      bt.print(",");
      bt.print((float)sensorValue.un.gameRotationVector.j, 2);
      bt.print(",");
      bt.print((float)sensorValue.un.gameRotationVector.k, 2);
      bt.println(";");
      //delay(10);
   //   break;

     //  }
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Wire.begin();
  bt.begin("ESP32_SPP___");
  bno08x.begin_I2C();

  if (! bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
  }

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (! bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
 // delay(100);
}
