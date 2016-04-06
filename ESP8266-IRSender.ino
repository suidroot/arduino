/*
 * WIFI-IRSender
 * This code will receive a Decimal formatted IR code from Adafruit.io and pulse it utilizing a IR LED.
 * Code is designed for a Adafruit HUZZAH ESP8266
 * Author: Ben Mason (suidroot)
 * Version: 1.0
 * 
 * NEC: 61A0F00F tv power
 * NEC: 48B710EF auto sleep
 * NEC: 48B7609F  auto
 * NEC: 77E1505B apple tv up
 */


#include <IRremoteESP8266.h>
#include <IRremoteInt.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

int irpin = 16;
int statusled = 0;

IRsend irsend(irpin);

// how often to report battery level to adafruit IO (in minutes)
#define BATTERY_INTERVAL 5
#define SLEEP_LENGTH 3

// wifi credentials
#define WLAN_SSID       "newhax0r"
#define WLAN_PASS       "syncm@st3r"

// configure Adafruit IO access

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "suidroot"
#define AIO_KEY         "220522759f1eff93075d74d264591922047b2efa"


/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ AIO_USERNAME;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'lamp' for subscribing to changes.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char IRSEND_FEED[] PROGMEM = AIO_USERNAME "/feeds/IRSEND";
Adafruit_MQTT_Subscribe lamp = Adafruit_MQTT_Subscribe(&mqtt, IRSEND_FEED);
const char BATTERY_FEED[] PROGMEM = AIO_USERNAME "/feeds/battery";
Adafruit_MQTT_Publish battery = Adafruit_MQTT_Publish(&mqtt, BATTERY_FEED);



// function prototypes
void connect(void);

void setup()
{
  Serial.begin(9600);
  irsend.begin();
  pinMode(statusled, OUTPUT);
  digitalWrite(statusled, LOW);

  

  // listen for events on the lamp feed
  mqtt.subscribe(&lamp);

  // connect to adafruit io
  connect();

}

void connect_wifi() {
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  
}

void loop() {

    Adafruit_MQTT_Subscribe *subscription;

  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }

  EEPROM.begin(512);
  
  // get the current count position from eeprom
  byte battery_count = EEPROM.read(0);

  // we only need this to happen once every X minutes,
  // so we use eeprom to track the count between resets. 
  if(battery_count >= ((BATTERY_INTERVAL * 60) / SLEEP_LENGTH)) {
    // reset counter
    battery_count = 0;
    // report battery level to Adafruit IO
    battery_level();
  } else {
    // increment counter
    battery_count++;
  }

  // save the current count
  EEPROM.write(0, battery_count);
  EEPROM.commit();

  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(1000)) {

    // we only care about the lamp events
    if (subscription == &lamp) {
      Serial.print("triggered: ");

      char *ircode = (char *)lamp.lastread;
      long unsigned int value = atoi(ircode);
      //long unsigned int value = (long unsigned int )lamp.lastread;
      Serial.println(value);

      if (value != 0) {

        for (int i = 0; i < 1; i++) {
          digitalWrite(statusled, HIGH);
          irsend.sendNEC(value, 32); // Sony TV power code
          Serial.println("sent");
          delay(1000);
          digitalWrite(statusled, LOW);

        }
      }
      battery_level();
    }
  }
}


// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}

void battery_level() {

  // read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // our 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).
  int level = analogRead(A0);
  //Serial.println(level);
  // convert battery level to percent
  level = map(level, 290, 415, 0, 100);
  Serial.print("Battery level: "); Serial.print(level); Serial.println("%");
  // turn on wifi if we aren't connected
  
  // grab the battery feed
  battery.publish((int32_t)level);
  
  
}
