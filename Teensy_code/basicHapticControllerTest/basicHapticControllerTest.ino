#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;
const int ledPin = 13;
boolean lightStatus = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("DRV test");
  drv.begin();

  drv.selectLibrary(1);

  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);
  pinMode(ledPin, OUTPUT);
}

uint8_t effect = 1;

void loop() {
  Serial.print("Effect #"); Serial.println(effect);

  lightStatus = !lightStatus;
  digitalWrite(ledPin, lightStatus);

  // set the effect to play
  drv.setWaveform(0, effect);  // play effect
  drv.setWaveform(1, 0);       // end waveform

  // play the effect!
  drv.go();

  // wait a bit
  delay(500);

  effect++;
  if (effect > 117) effect = 1;
}
