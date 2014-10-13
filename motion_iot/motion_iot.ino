

// include all Libraries needed:
//#include <Process.h>
#include <Bridge.h>
#include <HttpClient.h>
#include "passwords.h"      // contains my passwords, see below

int pirPin = 2; //digital 2
int pirVal = 0;
int pirNow;


// set up net client info:
//const unsigned long postingInterval = 10000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  //***************************************************************//
  // comment out this line if you want Yun to run w/o console open:
  while (!Console);   // wait for Network Serial to open
  //***************************************************************//

  Console.println("data.sparkfun client");
  
  pinMode(pirPin, INPUT);

  // Do a first update immediately
  updateData();
  sendData();
  lastRequest = millis();
}

void loop() {
  
  pirVal = digitalRead(pirPin);
  Console.print(pirVal);
  
  if(pirVal == LOW){ //was motion detected
    Console.print(pirVal); 
    Console.print("No one is there");
    if(pirNow!=pirVal){
      updateData();
      sendData();
      Console.print("update!!!");
    } 
    pirNow=pirVal;
    delay(500);
  }else{
    Console.print(pirVal); 
    Console.print("Theif!!!");
    if(pirNow!=pirVal){
      updateData();
      sendData();
      Console.print("update!!!");
    } 
    pirNow=pirVal; 
    delay(500);
    
  }
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
//  if (now - lastRequest >= postingInterval) {
//    updateData();
//    sendData();
//    lastRequest = now;
//  }
}

void updateData() {
  // convert the readings to a String to send it:
  dataString = "motion=";
  dataString += pirVal; //this is the data from my sensor
 
  // add pressure:

}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
  // form the string for the URL parameter:
  String url = "http://data.sparkfun.com/input/";
  url += PUBLICKEY;
  url += "?private_key=";
  url += PRIVATEKEY;
  url += "&";
  url += dataString;

  // Send the HTTP GET request

  HttpClient client;
  //Process xively;
//  Console.print("\n\nSending data... ");
//  Console.println(url);
  client.get(url);
  Console.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Console:
  while (client.available()) {
    char c = client.read();
//    Console.print(c);
  }
  Console.flush();
}




