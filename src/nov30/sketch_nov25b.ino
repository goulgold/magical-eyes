// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include  "Adafruit_NeoPixel.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
#define DEBUG_AMP 2.5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      6

// MSGEQ7 Control
int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5

int left[7]; // store band values in these arrays
int right[7];
int band;
int x=30;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
  pinMode(res, OUTPUT); // reset
  pinMode(strobe, OUTPUT); // strobe
  digitalWrite(res,LOW); // reset low
  digitalWrite(strobe,HIGH); //pin 5 is RESET on the shield
}

void readMSGEQ7() {
// Function to read 7 band equalizers
  digitalWrite(res, HIGH);
  digitalWrite(res, LOW);
  for(band=0; band <7; band++) {
    digitalWrite(strobe,LOW); // strobe pin on the shield - kicks the IC up to the next band
    delayMicroseconds(3); //
    left[band] = DEBUG_AMP*map(analogRead(0), 0, 1023, 0, 255); // store left band reading
    right[band] = DEBUG_AMP*map(analogRead(1), 0, 1023, 0, 255); // ... and the right
    digitalWrite(strobe,HIGH);
  }
}

void loop() {
  
  readMSGEQ7();
  // display values of left channel on serial monitor
  for (band = 0; band   < 7; band++) {
    Serial.print(left[band]);
    Serial.print(",");
  }
  // display values of right channel on serial monitor
  for (band = 0; band < 7; band++) {
    Serial.print(right[band]);
    Serial.print(",");
  }
  Serial.println();
  delay(10);
  
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
 
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int test=left[5]/42;
    int amp = test>i?200:0;
    int pick = random(1,4);
    switch(pick){
      case 1: pixels.setPixelColor(i, pixels.Color(0,amp,0)); break;
      case 2: pixels.setPixelColor(i, pixels.Color(amp,0,0)); break;
      default: pixels.setPixelColor(i, pixels.Color(0,0,amp)); 
    }
//    pixels.setPixelColor(i, pixels.Color(0,amp,0)); // Moderately bright green color.    
    pixels.show(); // This sends the updated pixel color to the hardware.
//  delay(delayval); // Delay for a period of time (in milliseconds).
  }

Serial.println(x);

}


