// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_MPR121.h>

/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#if defined(PARTICLE)
#include "Particle.h"
#else
#include <Wire.h>
#endif

const int numReadings = 10;

long readings[numReadings];

long total = 0;

long average = 0;

int readIndex = 0;

long sensorMin = 100;
long start = 0;
long tempMin = 0;
long triggered = 0;
uint8_t thresh = 0xFF;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'

void setup() {
  #if !defined(PARTICLE)
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!
  #endif

  Serial.begin(9600);
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    while (1)
    {
      #if defined(PARTICLE)
      Particle.process();
      #endif
    } 
  }
    for (int thisReading = 0; thisReading<numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
   calibrate();
}

void loop() {
  long timePassed = millis() - start;
    if(timePassed > 500) {
      sensorMin = long(thresh * 0.5 + tempMin * 0.5) ;
      start = millis();
      tempMin = thresh; 
    }
  
  total = total - readings[readIndex];
    readings[readIndex] = cap.filteredData(4);
    total = total + readings[readIndex];
    readIndex = readIndex +1;

    if (readIndex >= numReadings) {
      readIndex = 0;
    }

    average = total/numReadings;

    Serial.print(average);
    Serial.print(" ");
    Serial.println(sensorMin);

    if (average < tempMin) {
      tempMin = average;
  }
}

  void calibrate() {
    start = millis();
    
    while(millis() - start < 5000){
    //   settle
  }

    while (millis() - start < 10000) {
     uint8_t tempread = cap.filteredData(4);
     if (tempread < thresh) {
         thresh = tempread;
     }
  }
    start = 0;
    return;
}