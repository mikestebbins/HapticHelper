/*
    Use an Android app (Appinventor), EZ-Link Bluetooth module,
    and Teensy to control the DRV2605 Adafruit Haptic controller breakout.

    LAST UPDATED: 03-01-2016
    Copyright 2016 TEAGUE
    Arduino 1.6.7, Teensyduino 1.27, Windows 7 used to upload

  ---------------------------- Pin Assignments -------------------------------------

  Teensy 3.1 / 3.2                |      |
                          --------|      |--------
        Power Supply (-) [] Gnd              Vin [] Power Supply (+)
                         [] RX1             AGND []
                         [] TX1             3.3V [] DRV2605 VIN
                         [] 02                23 []
                         [] 03                22 []
                         [] 04                21 []
                         [] 05                20 []
                         [] 06                19 [] DRV2605 SCL
                         [] 07                18 [] DRV2605 SDA
                         [] 08                17 []
           EZ-Link BT TX [] RX2               16 []
           EZ-Link BT RX [] TX2               15 []
                         [] 11                14 []
                         [] 12                13 []
                          ------------------------

  Adafruit EZ-Link Bluetooth Module
                        -------------
                        |        GND [] Power Supply (-)
                        |        DSR [] 
                        |        Vin [] Power Supply (+)
                        |        TX  [] Teensy RX2
                        |        RX  [] Teensy TX2
                        |        DTR [] Power Supply (-)
                         ------------

  Power Supply (USB from Teensy)
                        + [] Teensy Vin
                        - [] Teensy GND

  LM7805 voltage regulator
                        -------------
                      |        VIN [] Power Supply Rail 1 (+) (9V)
                      |        GND [] Power Supply (-)
                      |       VOUT [] Power Supply Rail 2 (+) (5V)
                         ------------

  DRV2605 Breakout
                        -------------
                        |        VIN [] Power Supply (+)
                        |        GND [] Power Supply (-)
                        |        SCL [] Teensy 19
                        |        SDA [] Teensy 18
                        |        IN  [] 
                        |  MOTOR (+) []
                        |  MOTOR (-) []
                         ------------

FDS6911 N-channel MOSFET
                          -------------
GND +  DRV2605 MOTOR (-) [] S1     D1 [] to vibration motor terminal A
       DRV2605 MOTOR (+) [] G1     D1 []
                         [] S2     D2 [] 
                         [] G2     D2 []
                          -------------

VIBRATION MOTOR
                        A [] MOSFET D1
                        B [] +9v POWER SUPPLY   (there is a flyback diode across these, stripe towards +9V)

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
  Serial.begin(9600);   // For reporting out status to serial monitor, if connected to a computer
  Serial2.begin(9600);  // For all communication between Teensy and EZLink BT module
  
  drv.begin();
  drv.selectLibrary(1);

  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);
  Serial.println("serial up and running");
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void loop(void)
{
  if (Serial2.available() > 1)     // Read serial bytes (if available) and store them for use below
  {
    if (Serial2.available() == 2)  // Make sure its 2 bytes
    {
      digitalWrite(ledPin, HIGH);  // toggle LED to show some activity is happening

      messageByte1 = Serial2.read();   // Read and store the two bytes
      messageByte2 = Serial2.read();
      
      Serial.print("messageByte1 = "); // Print the message bytes out serial monitor
      Serial.print(messageByte1);
      Serial.print("\t");
      Serial.print("messageByte2 = ");
      Serial.println(messageByte2);

      playTheWaveform(messageByte1, messageByte2);  // play the appropriate waveform effect
      
      Serial2.flush();     // clear out any noise            
      Serial2.print("X");  // message acknowledged
      Serial.flush();            
      Serial.println("X");      
      
      digitalWrite(ledPin, LOW); // toggle LED to show some activity is happening
    }
    else   //if the data is not 2 bytes treat it as invalid, warn, then flush the buffer.
    {
      Serial.println("Invalid Data was received");
      char a;
      while ( (a = Serial.read()) != -1) {
        Serial.print(a);
      }
    }
  }
  delay (20);          // delay for a bit
  
  digitalWrite(ledPin, LOW);  // keep LED off until 2 bytes received
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
// FUNCTIONS

void playTheWaveform(byte testByte, byte effect)
{
  if (testByte == 255)
  {
    if (effect == 118)          // 118 is a ~5second buzz, not useful and recks my cyclic demo
    {
      effect = 47;
    }
    drv.setWaveform(0, effect);  // set the waveform
    drv.setWaveform(1, 0);      // end waveform
    drv.go();                   // play the effect!
    delay(10);
  }
  else
  {
    Serial.println("messy messaging");
  }
}

/* // Standard code to play all effects, not used currently
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
*/
