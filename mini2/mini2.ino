/**
 * A Mirf example to test the latency between two Ardunio.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include <Wire.h>

char SMPLRT_DIV= 0x19;
char itgAddress = 0x68;
int IMU1[7] = {0,0,0,0,0,0,0};

void setup(){
  Serial.begin(57600);
  /*
   * Setup pins / SPI.
   */
   
  /* To change CE / CSN Pins:
   * 
   * Mirf.csnPin = 9;
   * Mirf.cePin = 7;
   */
 
   //Initialize the I2C communication. This will set the Arduino up as the 'Master' device.
  //                                sensor innitionalizothin
  Wire.begin();  
  //set the power mangment to start to wake up the chip
  itgWrite(itgAddress, 0x6B, 0x00);
  //Set the sample rate to 8KHz
  itgWrite(itgAddress, SMPLRT_DIV, 0x11);
  //low pass filter set
  itgWrite(itgAddress,0x1A , 0x00);
  //gyro full sclae range 2000deg/s
  itgWrite(itgAddress,0x1B , 0x18);
  //accel full sclae range 8g
  itgWrite(itgAddress,0x1C , 0x10);
 
  
  Mirf.spi = &MirfHardwareSpi;
  
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
   
  Mirf.setRADDR((byte *)"clie2");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = 14;
  Mirf.channel = 91;
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
   
  Mirf.config();
  
  Serial.println("Beginning ... "); 
}

void loop(){
  
  Read_ac_gy_t();  
  
  Mirf.setTADDR((byte *)"serv1");
  
  Mirf.send((byte *)&IMU1);
  
  while(Mirf.isSending()){
  }


  //Serial.print((IMU1[0]));
  //Serial.print(" ");
  //Serial.println((IMU1[1]));
} 



void itgWrite(char address, char registerAddress, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(address);
  //Tell the I2C address which register we are writing to
  Wire.write(registerAddress);
  //Send the value to write to the specified register
  Wire.write(data);
  //End the communication sequence
  Wire.endTransmission();
}



void Read_ac_gy_t()
{
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,14,true);
  IMU1[0]=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  IMU1[1]=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  IMU1[2]=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  IMU1[6]=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  IMU1[3]=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  IMU1[4]=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  IMU1[5]=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Wire.endTransmission();
  
}

  
  
  
