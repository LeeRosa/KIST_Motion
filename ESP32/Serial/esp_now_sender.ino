#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO08x.h>

Adafruit_BNO08x  bno08x;
sh2_SensorValue_t sensorValue;

 //#define SCL_PIN 22
// #define SDA_PIN 23

// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0xD2, 0xF4, 0x74};

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

// Create a struct_message called BME280Readings to hold sensor readings
struct_message BNO08X_reading;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  }
  else {
    success = "Delivery Fail :(";
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  //Wire.begin(SDA_PIN,SCL_PIN,400000);
  Wire.begin();
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (! bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

    switch (sensorValue.sensorId) {
      case SH2_GAME_ROTATION_VECTOR:
        BNO08X_reading.id = 2;
        BNO08X_reading.real = sensorValue.un.gameRotationVector.real;
        BNO08X_reading.i = sensorValue.un.gameRotationVector.i;
        BNO08X_reading.j = sensorValue.un.gameRotationVector.j;
        BNO08X_reading.k = sensorValue.un.gameRotationVector.k;
        Serial.print(BNO08X_reading.real);
        break;
    }
    // Set values to send

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &BNO08X_reading, sizeof(BNO08X_reading));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
