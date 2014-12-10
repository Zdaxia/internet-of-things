#include <Adafruit_NeoPixel.h>

#include <Bridge.h>
#include <YunClient.h>

YunClient client;

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24
#define NUMPIXELS2      16

//config websocket we are uising netsocket here
#define SERVER_ADDRESS "10.0.1.72"
#define SERVER_PORT 5000

unsigned long postingInterval = 500;  //delay between server updates
unsigned long lastPost = 0;         // when you last made a request
unsigned long currTime = 0;            //what time is it right now

String incomingDataString = "";
boolean serverResponded = false;
boolean isSunOnce = false;


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(16, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3 = Adafruit_NeoPixel(12, 8, NEO_GRB + NEO_KHZ800);

int delayval = 0.5; // delay for half a second
bool isLightUp = true;
int statesValue = 0;

int ledStates = 0;

void setup() {
  //setup yun
  Bridge.begin();
  Console.begin();
  Serial.begin(9600);

  //setup three neorings
  pixels.begin(); // This initializes the NeoPixel library.
  pixels2.begin();
  pixels3.begin();

  //connect to YOUR SERVER'S IP ADDRESS + PORT
  client.connect(SERVER_ADDRESS, SERVER_PORT);
}

String hi = "hello from yun! count: ";
int count = 0;

void loop() {


  // current time elapsed so you can calculate post intervals:
  currTime = millis();

  //check if it's time to post an update to the server
  if (currTime - lastPost >= postingInterval) {
    if (client.connected()) {
      String data = hi + count;
      client.print(data);
      count++;
    } else {
      //no connection, try to make one again:
      Console.println("\nattempting to connect to server");
      client.connect(SERVER_ADDRESS, SERVER_PORT);
      delay(2000); //delay 2 seconds before trying another server reconnect
    }
    lastPost = currTime;
  }

  //while client.available()==true (meaning >= 1 char of data awaits us) then:
  while (client.available()) {
    //if(!serverResponded) Console.println(">>> Server Says : <<<"); //expecting something to be received
    char c = client.read();     //read this char
    if (isDigit(c)) {
      statesValue = String(c).toInt();
//      digitalWrite(13, statesValue);
     Console.print("received digit!");
     
    }
    incomingDataString += c; //add this char to our dataString
    serverResponded = true; //regardless of data, we got a response
  }

  if (serverResponded) {
    Console.print("received from server: ");
    Console.println(incomingDataString);
    //if(incomingDataString == "toggle") LEDValue = !LEDValue; //if you're looking for a string
    incomingDataString = ""; //clear out our data string for next server message
    serverResponded = false; //start this at false after server request
    //if(incomingDataString == "1") digitalWrite(13, HIGH);
    //else digitalWrite(13, LOW);
  }
  
  if(statesValue==2){
      isSunOnce=false;
       blink();
   }else if(statesValue==0){
     isSunOnce=false;
     black();
   }else if (statesValue==1){
     if(!isSunOnce){
       sunset();
       isSunOnce=true;
     }
     
   }else if(statesValue==3){
     if(!isSunOnce){
       sunrise();
       isSunOnce=true;
     }
   }
  
}

void blink() {
  
  
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for (int i = 0; i < 25; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(i * 10, i * 10, i * 10)); // Moderately bright green color.
      pixels2.setPixelColor(j, pixels2.Color(i * 10, i * 10, i * 10));
      pixels3.setPixelColor(j, pixels3.Color(i * 10, i * 10, i * 10));
      pixels.show(); // This sends the updated pixel color to the hardware.
      pixels2.show();
      pixels3.show();
    }

    delay(delayval); // Delay for a period of time (in milliseconds).
  }
  delay(200);
  for (int i = 25; i >= 0; i--) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(i * 10, i * 10, i * 10)); // Moderately bright green color.
      pixels2.setPixelColor(j, pixels2.Color(i * 10, i * 10, i * 10));
      pixels3.setPixelColor(j, pixels3.Color(i * 10, i * 10, i * 10));
      pixels.show(); // This sends the updated pixel color to the hardware.
      pixels2.show();
      pixels3.show();
    }

    delay(delayval); // Delay for a period of time (in milliseconds).
  }
  delay(200);
  

}

void black() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(0, 0, 0)); // Moderately bright green color.
      pixels2.setPixelColor(j, pixels2.Color(0, 0, 0));
      pixels3.setPixelColor(j, pixels3.Color(0, 0, 0));
      pixels.show(); // This sends the updated pixel color to the hardware.
      pixels2.show();
      pixels3.show();
    }

}

void sunset() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for (int i = 0; i < 51; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(i * 5, i * 5, i * 5)); // Moderately bright green color.
      pixels2.setPixelColor(j, pixels2.Color(i * 5, i * 5, i * 5));
      pixels3.setPixelColor(j, pixels3.Color(i * 5, i * 5, i * 5));
      pixels.show(); // This sends the updated pixel color to the hardware.
      pixels2.show();
      pixels3.show();
    }

    delay(delayval); // Delay for a period of time (in milliseconds).
  }

}

void sunrise(){
  for (int i = 0; i < 255; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(i, i, i/10)); // Moderately bright green color.
      pixels2.setPixelColor(j, pixels2.Color(i, i, i/10));
      pixels3.setPixelColor(j, pixels3.Color(i, i, i/10));
      pixels.show(); // This sends the updated pixel color to the hardware.
      pixels2.show();
      pixels3.show();
    }

    delay(200); // Delay for a period of time (in milliseconds).
  }
}
