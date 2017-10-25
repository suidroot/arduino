/*


https://gist.github.com/igrr/3b66292cf8708770f76b
https://learn.adafruit.com/esp8266-temperature-slash-humidity-webserver/wiring?view=all

*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>



#define WLAN_SSID       "newhax0r"
#define WLAN_PASS       "syncm@st3r"


WiFiClient client;


ESP8266WebServer server(80);



void setup()
{
  Serial.begin(9600);

  pinMode(statusled, OUTPUT);
  digitalWrite(statusled, LOW);


}

void loop(void) {
  // check for incomming client connections frequently in the main loop:
  server.handleClient();
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


server.on("/", [](){
  server.send(200, "text/plain", "This is an index page.");
});

// or, which is equivalent:
// CODE: SELECT ALL
void handle_index() {
  server.send(200, "text/plain", "This is an index page.");
}

// later, in setup() function:
server.on("/", handle_index);


// The first argument is the URI and the second is the function that will be called to respond to this URI.
// In that function you need to call
// CODE: SELECT ALL
server.send(code, content_type, content)
// to actually send contents.
//
// Processing arguments of GET and POST requests is also easy enough. Let's make our sketch turn a led on or off depending on the value of a request argument.
// http://<ip address>/led?state=on will turn the led ON
// http://<ip address>/led?state=off will turn the led OFF
// CODE: SELECT ALL
server.on("/led", []() {
  String state=server.arg("state");
  if (state == "on") digitalWrite(13, LOW);
  else if (state == "off") digitalWrite(13, HIGH);
  server.send(200, "text/plain", "Led is now " + state);
});
