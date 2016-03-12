/*
 *  Use an Android app (Appinventor), EZ-Link Bluetooth module, 
 *  and Teensy to control the DRV2605 Adafruit Haptic controller breakout.

 *  LAST UPDATED: 02-24-2016
 *  Copyright 2016 TEAGUE
 *  Arduino X.X.X and Windows 7 used to upload
 *

TODO's:

---------------------------- Pin Assignments -------------------------------------
                          
Teensy 3.1 / 3.2
                                  |      |
                          --------|      |--------
        Power Supply (-) [] Gnd              Vin [] Power Supply (+)
           EZ-Link BT Tx [] RX1             AGND []
           EZ-Link BT Rx [] TX1             3.3V [] DRV2605 VIN
                         [] 02                23 []
                         [] 03                22 []
                         [] 04                21 []
                         [] 05                20 []
                         [] 06                19 [] DRV2605 SCL
                         [] 07                18 [] DRV2605 SDA
                         [] 08                17 []
                         |  09                16 []
                         [] 10                15 []
                         [] 11                14 []
                         [] 12                13 []
                          ------------------------
 ??? was connected to Arduino Uno Reset pin: EZ-Link BT Vin  --|1uF|--

Adafruit EZ-Link Bluetooth Module
                        -------------
                        |        GND [] Power Supply (-)
                        |        DSR [] Power Supply (-)
                        |        Vin [] Power Supply (+)
                        |        TX  [] Teensy RX1
                        |        RX  [] Teensy TX1
                        |        DTR [] unconnected
                         ------------

DRV2605 Breakout
                     ----------------
        Teensy 3.3V [] VIN          [] 
         Teensy GND [] GND  OUT (-) [] MOSFET terminal S
          Teensy 19 [] SCL          [] 
          Teensy 18 [] SDA  OUT (+) [] MOSFET terminal G & 10k pulldown to Gnd
                    [] IN           [] 
                     ----------------
    
FDS 6911 N-channel MOSFET
                         ------------
        DRV2605 out (-) [] S1    D1 [] Motor terminal A
        DRV2605 out (+) [] G1    D1 [] 
                        [] S2    D2 []
                        [] G2    D2 []  
                         ------------

Motor (not polarized, doesn't matter which connection goes to which)
        
        terminal A [] MOSFET drain 1
                         |
                       -----
                       |   |
                       |   |
                       -----  Back EMF protection diode (stripe toward V+)
                       |   |
                       -----
                         |
        terminal B [] 14V power supply (+)


 Power Supply - 14V, 1A
                        [] +
                        [] -

 Linear Regulator 5V
                        [] +
                        [] -
                                        
 -----------------------------------------------------------------------------------------       
*/

#include <Wire.h>
#include "Adafruit_DRV2605.h"

int ledPin = 13;           //  Arduino on-board LED pin, for testing purposes

byte messageByte1;          // first byte of serial communication (holdover from previous messaging scheme)
byte messageByte2;          // second byte of serial communication

byte effect = 1;

Adafruit_DRV2605 drv;

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void setup() 
{
  pinMode (ledPin, OUTPUT); 
  Serial.begin(9600);
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
