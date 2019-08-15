#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

const int led = 12;

const int numReadings = 10;

long readings[numReadings];

long total = 0;

long average = 0;

int readIndex = 0;

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

void setup()                    
{
   pinMode(led, OUTPUT);
  
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);

   for (int thisReading = 0; thisReading<numReadings; thisReading++) {
    readings[thisReading] = 0;
   }
}

void loop()                    
{
    long start = millis();
    total = total - readings[readIndex];
    readings[readIndex] = cs_4_2.capacitiveSensor(30);
    total = total + readings[readIndex];
    readIndex = readIndex +1;

    if (readIndex >= numReadings) {
      readIndex = 0;
    }

    average = total/numReadings;

    Serial.println(average);                  // print sensor output 1

    delay(100);                             // arbitrary delay to limit data to serial port 

    if (average > 100) {
      digitalWrite(led, HIGH);
    }
      else {
  digitalWrite (led, LOW);
    }

}
