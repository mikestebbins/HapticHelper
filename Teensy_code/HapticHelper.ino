/*
 *  Use an Android app (Appinventor), EZ-Link Bluetooth module, 
 *  and Teensy to control the DRV2605 Adafruit Haptic controller breakout.

 *  LAST UPDATED: 02-24-2016
 *  Copyright 2016 TEAGUE
 *  Arduino X.X.X and Windows 7 used to upload
 *

TODO's:
- update diagram of board and hookups
- 

----------------------------Arduino Pin Assignments -------------------------------------
                   	                 ____
                            _           |    |
                         .-| |----------|    |----.
                         | | |  Arduino Uno  |    |
                         |  -            ----     |
                         |                   Gnd []
                         |                   D13 []
                  -   +  |                   D12 []
EZ-Link BT Vin  --|1uF|--[] Reset            D11 []
                         [] 3.3v             D10 []
       EZ-Link BT Vin    [] 5V                D9 []
       EZ-Link BT Ground [] Gnd               D8 []
                         [] Gnd                   |
                         [] Vin               D7 []
                         |                    D6 []
                         [] A0                D5 []
                         [] A1                D4 []
                         [] A2                D3 []
                         [] A3                D2 []
                         [] A4            tx  D1 [] EZ-Link BT Rx
                         [] A5            rx  D0 [] EZ-Link BT Tx
    			               L                   ____|
                           \_________________/      
                           
 -----------------------------------------------------------------------------------------       
*/

#include <Wire.h>
#include "Adafruit_DRV2605.h"

const int ledPin = 13;           //  Arduino on-board LED pin, for testing purposes

byte messageByte1;          // first byte of serial communication (holdover from old code)
byte messageByte2;          // second byte of serial communication

Adafruit_DRV2605 drv;

uint8_t effect = 1;

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void setup() 
{

  pinMode (ledPin, OUTPUT); 
  Serial.begin(9600);

  Serial.println("DRV test");
  drv.begin();
  
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void loop(void) 
{
   if (Serial.available() > 1)      // READ SERIAL BYTES (IF AVAILABLE) AND STORE THEM FOR USE BELOW
   {
    if (Serial.available() == 2)
    {  //make sure its 2 bytes
      messageByte1 = Serial.read();
      messageByte2 = Serial.read();
      Serial.flush();        // clear out any noise

      playTheWaveform(messageByte1,messageByte2);
    }
    else   //if the data is not 2 bytes treat it as invalid, warn, then flush the buffer.
    {    
      Serial.println("Invalid Data was received");
      char a;
      while( (a = Serial.read()) != -1){
        Serial.print(a);
      }
    }
  }
  delay (10);          // delay for a bit
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
// FUNCTIONS

void playTheWaveform(byte testByte, byte effect)  
{
  if (testByte == 255) 
  {
    drv.setWaveform(0,effect);   // set the waveform
    drv.setWaveform(1, 0);      // end waveform 
    drv.go();                   // play the effect!  
  }
  else  
  {
    Serial.println("messy messaging");
  }                
}


void doThemAll()  
{ 
  for (int i = 1; i < 123; i++)
  {
    // set the effect to play
    drv.setWaveform(0, i);      // play effect 
    drv.setWaveform(1, 0);      // end waveform
    drv.go();                   // play the effect!

   // wait a bit
    delay(500);
  }
}
