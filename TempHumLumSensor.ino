/*
 * Tempurature Humidity and Luminance Sensor outputing to a SPI Screen
 * 
 * Components
 * Adafruit Feather HuzzaH
 * DHT22
 * Photo Resistor
 * 10k Resistor
 * SSD1306 OLED Display
 * 
 */


// DHT Libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// Screen Libraries
#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Using SPI
// Screen Parameters
#define OLED_MOSI   14
#define OLED_CLK   12
#define OLED_DC    13
#define OLED_CS    15
#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

// DHT Parameters (Tempurature Humitiy Sensor)
#define DHTPIN            2         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

// Define Analogue Pin for Light Mesurement
#define LIGHTPIN A0

void setup() {
  Serial.begin(9600); 
  // Initialize DHT device.
  dht.begin();
  sensor_t sensor;

  // Intialize the Screen
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}


void loop() {

  // Setup Display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    display.println("Error reading temperature!");
  }
  else {
    display.print("Temp: ");
    display.print((int)round(1.8*event.temperature+32));
    display.println(" F");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    display.println("Error reading humidity!");
  }
  else {
    display.print("Hum: ");
    display.print(event.relative_humidity);
    display.println("%");
  }

  // Get Light Reading from Analogue Pin
  display.print("Lum: ");
  display.print(map(analogRead(LIGHTPIN), 0, 1024, 0, 100));
  display.println("%");

  display.display();
  delay(1500);
  display.clearDisplay();


}
