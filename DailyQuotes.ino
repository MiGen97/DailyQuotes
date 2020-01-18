#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

const char* ssid = "yourSSID";
const char* password = "yourPassword";

// Define the pins from ESP8266 that are used
// Pins defined with -1 are not used, 
// but need to be set this way to avoid errors in Adafruit_ILI9341 library
#define TFT_DC   0
#define TFT_MOSI 1
#define TFT_CS   15
#define TFT_CLK  16
#define TFT_RST -1
#define TFT_MISO -1

// Create an object TFT for the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(3);
  tft.println("Hello!");
  tft.setTextSize(1);
  tft.println("I am here to make your day better.");
  tft.println("But first, let me connect to the internet!");
  WiFi.begin(ssid, password);
  tft.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    tft.print(".");
    delay(500);
  }
  
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+10);
  tft.setTextSize(2);  
  tft.println("Done!");
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+10);
  tft.setTextSize(1);  
  tft.println("Let's find an interesting quote!");
  
  if (WiFi.status() == WL_CONNECTED) {                                        //Check WiFi connection status
    HTTPClient http;                                                          //Declare an object of class HTTPClient
    http.begin("http://quotes.rest/qod.json?category=inspire");               //Specify request destination
    int httpCode = http.GET();                                                //Send the request
    if (httpCode > 0) {                                                       //Check the returning code
      String payload = http.getString();                                      //Get the request response payload
      tft.fillScreen(ILI9341_BLACK);
      int startQuote= payload.indexOf("\"quote\":") + 10;
      int startLength= payload.indexOf("\"length\":");
      int startAuthor= payload.indexOf("\"author\":")+ 11;
      int startTags= payload.indexOf("\"tags\":");
      String qoute=payload.substring(startQuote,startLength-6);
      String author=payload.substring(startAuthor,startTags-6);
      tft.setTextSize(1); 
      tft.setCursor(30, 110);
      tft.println(qoute);                                                   //Print the response payload
      tft.setCursor(200, tft.getCursorY()+10);  
      tft.println(author);  
    }
    http.end();                                                               //Close connection
  }
}

void loop() {
  //do nothing
}
