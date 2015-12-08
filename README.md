# JammsterNRFIMU

System uses MP9150 IMU
http://cdn.sparkfun.com/datasheets/Sensors/IMU/PS-MPU-9150A.pdf

Communicates with nRF24L01 using the MIRF arduino library:
https://github.com/stanleyseow/arduino-nrf24l01

To use:  
1. Install robot raconteur for python: https://robotraconteur.com  
2. Power the two sensors on by plugging the batteries in  
3. Plug the gateway arduino into the computer you would like to have as the server  
4. run  sudo chmod 777 /dev/ttyACM0 to change permissions on port
5. Run nrfIMU.py on the computer with the gateway arduino  
6. Connect string will look like: 'tcp://localhost:53869/arduinoIMU/arduinoIMUData' but will have a different port  
