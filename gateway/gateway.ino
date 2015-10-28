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

#define DEBUG 0

int IMU1[7] = {0,0,0,0,0,0,0};
int IMU2[7] = {0,0,0,0,0,0,0};
unsigned long now = 0;
unsigned long old = 0;

//used to communicate with Robot Raconteur through serial
byte byteBuffer[14];

void setup(){
  Serial.begin(57600);
  
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
   
  
  Serial.println("Listening..."); 
}

void loop(){
  /*
   * A buffer to store the data.
   */
   
  now = millis();
  byte data[Mirf.payload];
  
  unsigned long time = millis();
  
  // READ THE FIRST IMU
  Mirf.channel = 90;
  Mirf.config();
  Mirf.setTADDR((byte *)"clie1");
  delay(2);

  //read from the first sensor if data is ready
  if(Mirf.dataReady()){

    Mirf.getData(data);
   
    byteAToIntA(data, IMU1);
    
    if(DEBUG){ 
      Serial.print("IMU1: ");
      Serial.print((IMU1[0]));
      Serial.print(" ");
      Serial.println((IMU1[1]));
    }
  }
  
  
  // READ THE SECOND IMU
  Mirf.channel = 91;
  Mirf.config();
  Mirf.setTADDR((byte *)"clie2");
  delay(2);
  
  //read from the second sensor if data is ready
  if(Mirf.dataReady()){

    Mirf.getData(data);
   
    byteAToIntA(data, IMU2);
     
    if(DEBUG){
      Serial.print("IMU2:                          ");
      Serial.print((IMU2[0]));
      Serial.print(" ");
      Serial.println((IMU2[1]));
    }
  }  
 
  //check rate
  //Serial.println(millis()-now);
 
  //read serial commands 
  int val = 0;
  int static s = -1;
  if (Serial.available() >0) {
    
    /* whatever is available from the serial is read here    */
    val = Serial.read();
    
    if(DEBUG){
      Serial.print("val is : ");
      Serial.println(val);
    }
    
    if(DEBUG){
      Serial.print("s is : ");
      Serial.println(s);
    }
    
    /* This part basically implements a state machine that 
       reads the serial port and makes just one transition 
       to a new state, depending on both the previous state 
       and the command that is read from the serial port. 
       Some commands need additional inputs from the serial 
       port, so they need 2 or 3 state transitions (each one
       happening as soon as anything new is available from 
       the serial port) to be fully executed. After a command 
       is fully executed the state returns to its initial 
       value s=-1                                            */

    switch (s) {

      /* s=-1 means NOTHING RECEIVED YET ******************* */
      case -1:      

      /* calculate next state                                */
      if (val>47 && val<90) {
	  /* the first received value indicates the mode       
           49 is ascii for 1, ... 90 is ascii for Z          
           s=0 is change-pin mode;
           s=10 is DI;  s=20 is DO;  s=30 is AI;  s=40 is AO; 
           s=90 is query script type (1 basic, 2 motor);
           s=340 is change analog reference;
           s=400 example echo returning the input argument;
                                                             */
        s=10*(val-48);
      }
      
      /* the following statements are needed to handle 
         unexpected first values coming from the serial (if 
         the value is unrecognized then it defaults to s=-1) */
      if ((s>40 && s<90) || (s>90 && s!=340 && s!=400)) {
        s=-1;
      }

      /* the break statements gets out of the switch-case, so
      /* we go back and wait for new serial data             */
      break; /* s=-1 (initial state) taken care of           */


     
   
 
      /*s=20 means send IMU data*/
      case 20:
      if (val == 0){
        intToBytes(IMU1,7,byteBuffer);
        Serial.write(byteBuffer, 14 );
        } 
      else if(val == 1){
        intToBytes(IMU2,7,byteBuffer);
        Serial.write(byteBuffer, 14 );
        }
      s=-1;
      break; /* s=0 taken care of                            */
     

 

      /* ******* UNRECOGNIZED STATE, go back to s=-1 ******* */
      
      default:
      /* we should never get here but if we do it means we 
         are in an unexpected state so whatever is the second 
         received value we get out of here and back to s=-1  */
      
      s=-1;  /* go back to the initial state, break unneeded */



    } /* end switch on state s                               */

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

// convert an array of ints to an array of bytes
// specify size (number of ints) in input as sizeOfIntArray
void intToBytes(int input[], int sizeOfIntArray, byte buf[]){
  for(int i = 0; i < sizeOfIntArray;i++){
    buf[i*2] = (byte) input[i];
    buf[i*2+1] = (byte) (input[i] >> 8);
  }
  return;
}
