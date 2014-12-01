#include  "Adafruit_NeoPixel.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
// The amplitude of how much led is work.
#define DEBUG_AMP      2.5

// MSGEQ7 Control
int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5
int left[7]; // store band values in these arrays
int right[7];

// Use Finite State Machine to control the system
#define INIT  1
#define FREQ_1 2
#define FREQ_2 3

int state = INIT;


// The led strip parameter
// DIM means how many leds are used to demonstrate a single frequency.
const int DIM = 7;
// FRE means the audio signal is divided to how many freqencies.
const int FRE = 7; 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      DIM*FRE; 

int led_strip_number [FRE][DIM];
led_strip_number[0] = {0, 1, 2, 3, 4, 5, 6};
led_strip_number[1] = {7, 8, 9, 10, 11, 12, 13};
led_strip_number[2] = {14, 15, 16, 17, 18, 19, 20};
led_strip_number[3] = {21, 22, 23, 24, 25, 26, 27};
led_strip_number[4] = {28, 29, 30, 31, 32, 33, 34};
led_strip_number[5] = {35, 36, 37, 38, 39, 40, 41};
led_strip_number[6] = {42, 43, 44, 45, 46, 47, 48};

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
  int band;
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
  
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  switch(state){
    case INIT:
    {
      for( int i = 0; i < FRE; i++ )
      {
        for( int j = 0; j < DIM; j++ )
        {
          pixels.setPixelColor(i, pixels.Color(100, 0, 0));
          pixels.show();
          delayMicroseconds(500);
        }
      }
      break;
    }
    case FREQ_1:
    {
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
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
      break;
    }

  }
 



}


