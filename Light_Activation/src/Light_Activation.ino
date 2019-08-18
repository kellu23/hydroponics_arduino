/*
 * Project Light_Activation
 * Description:
 * Author:
 * Date:
 */
#if defined(PARTICLE)
#include "Particle.h"
#else
#include <Wire.h>
#endif
#include "Adafruit_MPR121.h"

int redPin = 4;
int greenPin = 5;
int bluePin = 6;

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'

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

void setup() {
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  

  setColor(255,0,0); // Initialize all pixels to 'off'
  Serial.begin(9600);
//   Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    // Serial.println("MPR121 not found, check wiring?");
    while (1)
    {
    //   #if defined(PARTICLE)
    //   Particle.process();
    //   #endif
    } 
    
  }
//   Serial.println("MPR121 found!");
  
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
      if (millis() - triggered > 500) {
      if (average < (sensorMin-10)) {

    setColor(0, 0, 0);
    delay(1000);
    setColor(0,255,255);
      }
      }
}

// uncomment if using LED strip and adjust code
// void colorWipe(uint32_t c, uint8_t wait) {
//   for(uint16_t i=0; i<strip.numPixels(); i++) {
//     strip.setPixelColor(i, c);
//     strip.show();
//     delay(wait);
//   }
// }

void calibrate() {
  start = millis();
    
  while(millis() - start < 5000){
    //   settle
    setColor(0,255,0);
  }

  while (millis() - start < 10000) {
     uint8_t tempread = cap.filteredData(4);
     if (tempread < thresh) {
         thresh = tempread;
     }
     setColor(0,0,255);
  }
  start = 0;
  return;
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}