# KIST_Motion
======================
Motion capture using imu sensor
-------------------------------------
               
##### ★blender file is blender/bno085_motionsuit.blender

##### ★The python file is used in the blender.
              
              
##### ★esp32 source code is two type

##### 1. receiver

##### - Source of esp32 that receives and finally transmits sensor data. (Change the "for" according to the number of sensors.)

##### - _bluetooth is a version of Bluetooth that transmits data. (not _bluetooth is Serial version)
         
##### 2. sender

##### - This is the source code for sending sensor data from "bno085" to other esp32.

##### - If receiver esp32 is changed, the broadcastAddress should be modified.
