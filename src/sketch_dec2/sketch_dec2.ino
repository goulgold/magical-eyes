#include  "Adafruit_NeoPixel.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
// The amplitude of how much led is work.
#define DEBUG_AMP      1.8

// MSGEQ7 Control
int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5
int left[7]; // store band values in these arrays
int right[7];

// Use Finite State Machine to control the system
#define INIT  1
#define FREQ_1 2
#define FREQ_2 3
#define FREQ_3 4
#define FREQ_4 5

int state = FREQ_4;


// The led strip parameter
// DIM means how many leds are used to demonstrate a single frequency.
const int DIM = 8;
// FRE means the audio signal is divided to how many freqencies.
const int FRE = 7; 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      DIM*FRE

int led_strip_number [FRE][DIM] = {{0,13,14,27,28,41,42,55},{1,12,15,26,29,40,43,54},{2,11,16,25,30,39,44,53},{3,10,17,24,31,38,45,52},{4,9,18,23,32,37,46,51},{5,8,19,22,33,36,47,50},{6,7,20,21,34,35,48,49}};

// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color[FRE][DIM];
//RED




int delayval = 500; // delay for half a second

int band;

void setup() {
  //RED
  color[0][0] = pixels.Color(255,0,0);
  color[0][1] = pixels.Color(255,26,26);
  color[0][2] = pixels.Color(255,51,51);
  color[0][3] = pixels.Color(255,77,77);
  color[0][4] = pixels.Color(255,102,102);
  color[0][5] = pixels.Color(255,128,128);
  color[0][6] = pixels.Color(255,154,154);
  color[0][7] = pixels.Color(255,179,179);
  //ORANGE
  color[1][0] = pixels.Color(255,128,0);
  color[1][1] = pixels.Color(255,141,26);
  color[1][2] = pixels.Color(255,154,51);
  color[1][3] = pixels.Color(255,166,77);
  color[1][4] = pixels.Color(255,179,102);
  color[1][5] = pixels.Color(255,192,128);
  color[1][6] = pixels.Color(255,205,154);
  color[1][7] = pixels.Color(255,218,179);
  //YELLOW
  color[2][0] = pixels.Color(255,213,0);
  color[2][1] = pixels.Color(255,218,26);
  color[2][2] = pixels.Color(255,222,51);
  color[2][3] = pixels.Color(255,226,77);
  color[2][4] = pixels.Color(255,230,102);
  color[2][5] = pixels.Color(255,235,128);
  color[2][6] = pixels.Color(255,239,154);
  color[2][7] = pixels.Color(255,243,179);
  //GREEN
  color[3][0] = pixels.Color(0,255,0);
  color[3][1] = pixels.Color(26,255,26);
  color[3][2] = pixels.Color(51,255,51);
  color[3][3] = pixels.Color(77,255,77);
  color[3][4] = pixels.Color(102,255,102);
  color[3][5] = pixels.Color(128,255,128);
  color[3][6] = pixels.Color(154,255,154);
  color[3][7] = pixels.Color(179,255,179);
  //BLUE
  color[4][0] = pixels.Color(0,0,255);
  color[4][1] = pixels.Color(26,26,255);
  color[4][2] = pixels.Color(51,51,255);
  color[4][3] = pixels.Color(77,77,255);
  color[4][4] = pixels.Color(102,102,255);
  color[4][5] = pixels.Color(128,128,255);
  color[4][6] = pixels.Color(154,154,255);
  color[4][7] = pixels.Color(179,179,255);
  //PURPLE
  color[5][0] = pixels.Color(102,0,102);
  color[5][1] = pixels.Color(153,0,153);
  color[5][2] = pixels.Color(204,0,204);
  color[5][3] = pixels.Color(255,0,255);
  color[5][4] = pixels.Color(255,51,255);
  color[5][5] = pixels.Color(255,102,255);
  color[5][6] = pixels.Color(255,153,255);
  color[5][7] = pixels.Color(255,204,204);
  //VIOLET
  color[6][0] = pixels.Color(137,0,255);
  color[6][1] = pixels.Color(148,26,255);
  color[6][2] = pixels.Color(160,51,255);
  color[6][3] = pixels.Color(172,77,255);
  color[6][4] = pixels.Color(184,102,255);
  color[6][5] = pixels.Color(196,128,255);
  color[6][6] = pixels.Color(208,154,255);
  color[6][7] = pixels.Color(220,179,255);
  
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
    delayMicroseconds(1); //
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
          pixels.setPixelColor(led_strip_number[i][j], color[i][j]);
          pixels.show();
          delay(100);
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
    case FREQ_2:
    {
      //The amplitude of one frequency is sliced to DIM sections
      int FRE_SLICE = 255/DIM;

      for (int i = 0; i < FRE; ++i)
      {
        int dim_max = left[i]/FRE_SLICE;
        for (int j = 0; j < DIM; ++j)
        {
          // The jth leds should be lighted or not
          int amp = dim_max > j ? 20 : 0;
          int pick = random(1,4);
          switch(pick){
            case 1: pixels.setPixelColor(led_strip_number[i][j], pixels.Color(0,amp,0)); break;
            case 2: pixels.setPixelColor(led_strip_number[i][j], pixels.Color(amp,0,0)); break;
            default: pixels.setPixelColor(led_strip_number[i][j], pixels.Color(0,0,amp)); 
        }  
        }
      }
      pixels.show();
      break;
    }
    //FREQ_3
        case FREQ_3:
    {
      //The amplitude of one frequency is sliced to DIM sections
      int FRE_SLICE = 255/DIM;

      for (int i = 0; i < FRE; ++i)
      {
        int dim_max = left[i]/FRE_SLICE;
        for (int j = 0; j < DIM; ++j)
        {
          // The jth leds should be lighted or not
          int amp = dim_max > j ? 20 : 0;
          pixels.setPixelColor(led_strip_number[i][j], pixels.Color(amp,0,0));
        }  
        }
      pixels.show();
      break;
    }
        //FREQ_4
        case FREQ_4:
    {
      //The amplitude of one frequency is sliced to DIM sections
      int FRE_SLICE = 255/DIM;

      for (int i = 0; i < FRE; ++i)
      {
        int dim_max = left[i]/FRE_SLICE;
        for (int j = 0; j < DIM; ++j)
        {
          // The jth leds should be lighted or not
          uint32_t amp = dim_max > j ? color[i][j]: 0;
          pixels.setPixelColor(led_strip_number[i][j], amp);
        }  
        }
      pixels.show();
      break;
    }
    }
}


