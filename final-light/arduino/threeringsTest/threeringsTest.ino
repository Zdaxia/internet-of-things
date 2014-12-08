// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24
#define NUMPIXELS2      16


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(16, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3= Adafruit_NeoPixel(12, 8, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pixels2.begin();
  pixels3.begin();
}

void loop() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for(int i=0;i<255;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for(int j=0; j<NUMPIXELS; j++){
       pixels.setPixelColor(j, pixels.Color(i,0,0)); // Moderately bright green color.
       pixels2.setPixelColor(j, pixels2.Color(i,0,0)); 
       pixels3.setPixelColor(j, pixels3.Color(i,0,0)); 
       pixels.show(); // This sends the updated pixel color to the hardware.
       pixels2.show();
       pixels3.show();
    }
   
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}
