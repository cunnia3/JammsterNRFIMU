/**
 * An Mirf example which copies back the data it recives.
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
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

int IMU0[7] = {0,0,0,0,0,0,0};
int IMU1[7] = {0,0,0,0,0,0,0};

void setup(){
  Serial.begin(9600);
  
  /*
   * Set the SPI Driver.
   */

 

  Mirf.spi = &MirfHardwareSpi;
  
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  
  /*
   * Setup pins / SPI.
   */
   
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
   
  Mirf.setRADDR((byte *)"serv1");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = 14;
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  Mirf.config();
  
  Serial.println("Listening..."); 
}

void loop(){
  /*
   * A buffer to store the data.
   */
  byte data[Mirf.payload];
  
  unsigned long time = millis();
  
  // READ THE FIRST IMU
  Mirf.setTADDR((byte *)"clie1");
  delay(10);

  //read from the first sensor if data is ready
  if(Mirf.dataReady()){

    Mirf.getData(data);
   
    byteAToIntA(data, IMU0);
     
    Serial.print((IMU0[0]));
    Serial.print(" ");
    Serial.println((IMU0[1]));
  }
  
  
  // READ THE SECOND IMU
  Mirf.setTADDR((byte *)"clie2");
  delay(10);
  
  //read from the second sensor if data is ready
  if(Mirf.dataReady()){

    Mirf.getData(data);
   
    byteAToIntA(data, IMU1);
     
    Serial.print((IMU1[0]));
    Serial.print(" ");
    Serial.println((IMU1[1]));
  }  
 
}

// Fill an int array with the contents of a byte array
// assume that the size of the int array is 7
// and that the size of the byte array is 14
void byteAToIntA(byte bArray[], int iArray[]){
  for(int i =0; i < 7; i++){
   iArray[i] = bArray[2*i] | bArray[(2*i)+1] << 8; 
  }
}
