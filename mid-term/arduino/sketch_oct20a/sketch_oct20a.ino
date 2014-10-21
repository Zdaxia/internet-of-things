#include <Process.h>
#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information
#include "passwords.h"      // contains my passwords

//config twitter api information
const String TWITTER_ACCESS_TOKEN = "[YOUR-TOKEN]";
const String TWITTER_ACCESS_TOKEN_SECRET = "[YOUR-TWITTER_ACCESS_TOKEN_SECRET]";
const String TWITTER_CONSUMER_KEY = "[YOUR-KEY]";
const String TWITTER_CONSUMER_SECRET = "[YOUR-SECRET]";

//distance vars
int trigPin = 11;    //Trig - green Jumper
int echoPin = 12;    //Echo - yellow Jumper
int catEatDistanceMax = 13; // Cat eating distance range
int eatingTimer = 0;
int eatingTimerNum = 6; //Filter the data which is from very long distance.
int mealCatHadToday = 0;
int isCatEating = 1;

//xively
String dataString = "";

//time vars
Process date;                 // process used to get the date
int hours, minutes, seconds;  // for the results
int lastSecond = -1;          // need an impossible value for comparison


long duration, cm, inches;

void setup() {
  Bridge.begin();    // initialize Bridge
  Serial.begin (9600); //Serial Port begin

  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


}

void loop()
{


  //Get distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = (duration / 2) / 29.1;
  inches = (duration / 2) / 74;

  //  Serial.print(duration);
  //  Serial.print("duration, ");
  //  Serial.print(inches);
  //  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  // if cm cm <= catEatDistanceMax means that cat is eating
  if (cm <= catEatDistanceMax && isCatEating == 1) {
    //cat is eating
    eatingTimer++;
    if (eatingTimer == eatingTimerNum) {
      Serial.println("Hey I am eating");
      mealCatHadToday++;
      Serial.println(mealCatHadToday);
      Serial.println("meal");
      updateData();
      sendData();
      sendOutTwitter();
      isCatEating = 0; // when cat is eating, we just send out data once
    };

  } else if (cm > catEatDistanceMax) {
    //cat finish Eating
    isCatEating = 1;
    eatingTimer = 0;
  }



  //  getAndUpdateTime();// getAndUpdate Time
  delay(500);
}

void updateData() {
  // convert the readings to a String to send it:
  dataString = "meal,";
  dataString += mealCatHadToday ;

}

void sendData() {
  // form the string for the API header parameter:
  String apiString = "X-ApiKey: ";
  apiString += APIKEY;

  // form the string for the URL parameter:
  String url = "https://api.xively.com/v2/feeds/";
  url += FEEDID;
  url += ".csv";

  // Send the HTTP PUT request

  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process xively;
  Serial.print("\n\nSending data... ");
  xively.begin("curl");
  xively.addParameter("-k");
  xively.addParameter("--request");
  xively.addParameter("PUT");
  xively.addParameter("--data");
  xively.addParameter(dataString);
  xively.addParameter("--header");
  xively.addParameter(apiString);
  xively.addParameter(url);
  xively.run();
  Serial.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (xively.available() > 0) {
    char c = xively.read();
    Serial.write(c);
  }

}

void sendOutTwitter() {
  String tweetText ="YAMY!!! I like my food! I had ";
  tweetText += mealCatHadToday;
  tweetText+=" meals today!!!";

  TembooChoreo StatusesUpdateChoreo;

  // invoke the Temboo client
  // NOTE that the client must be reinvoked, and repopulated with
  // appropriate arguments, each time its run() method is called.
  StatusesUpdateChoreo.begin();

  // set Temboo account credentials
  StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
  StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);

  // identify the Temboo Library choreo to run (Twitter > Tweets > StatusesUpdate)
  StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");

  // add the Twitter account information
  StatusesUpdateChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
  StatusesUpdateChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);
  StatusesUpdateChoreo.addInput("ConsumerKey", TWITTER_CONSUMER_KEY);
  StatusesUpdateChoreo.addInput("ConsumerSecret", TWITTER_CONSUMER_SECRET);

  // and the tweet we want to send
  StatusesUpdateChoreo.addInput("StatusUpdate", tweetText);

  // tell the Process to run and wait for the results. The
  // return code (returnCode) will tell us whether the Temboo client
  // was able to send our request to the Temboo servers
  unsigned int returnCode = StatusesUpdateChoreo.run();

  // a return code of zero (0) means everything worked
  if (returnCode == 0) {
    Serial.println("Success! Tweet sent!");
  }


}

void getAndUpdateTime() {
  Serial.println("getAndUpdateTime");
  // run an initial date process. Should return:
  // hh:mm:ss :
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }

  while (date.available() > 0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();

    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");

    // get the substrings for hour, minute second:
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds;          // save to do a time comparison
    seconds = secString.toInt();

    Serial.println(timeString);
  }
};


