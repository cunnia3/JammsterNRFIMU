# JammsterNRFIMU
Firmware used for custom IMU's used in the Baxter on Wheels project.  The system architecture consists of a gateway arduino
node and two wireless sensor modules.  The networking is set up to be a mesh so it should be relatively easy to add additional 
sensors to the network while maintaining a single gateway if one desired.

## Hardware
Sensor Module 
1. MPU 9150
2. nrf24l01
3. Arduino nano

Gateway
1. nrf24l01
2. Arduino mega
