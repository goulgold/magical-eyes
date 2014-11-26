// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include  "Adafruit_NeoPixel.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 30; // delay for half a second

// For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
int x=15;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  for(int i=0;i<NUMPIXELS;i++){
    x=(x+1)%120; //The amptitude of LED    
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if ( x < 40 )
    {
        pixels.setPixelColor(i, pixels.Color(40-x,x,0)); // Moderately bright green color.
    }
    else if ( x < 80 )
   {
        pixels.setPixelColor(i, pixels.Color(0,80-x,x-40)); // Moderately bright green color.
   } 
   else
   {
        pixels.setPixelColor(i, pixels.Color(x-80,0,120-x)); // Moderately bright green color.
   }
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}
