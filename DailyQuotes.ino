//this is for the RTC memory read/write functions
extern "C"{
  #include "user_interface.h"
}

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
#define TFT_DC   12
#define TFT_MOSI 14
#define TFT_CS   15
#define TFT_CLK  13
#define TFT_RST -1
#define TFT_MISO -1
#define TFT_POWER 5

// Create an object TFT for the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
uint64_t sleepTimeUs=1e7; //ESP.deepSleepMax();

void setup() {
  pinMode(TFT_POWER,OUTPUT);
//to initialize the program runs on a ESP8266 uncommment the next two lines
//the initalization means that the esp will display the next quote after aproximately 24hours
//from the time of the initialization, if the power supply is not disconnected in the meantime
//after this, the two lines bellow must be always commented
  //uint32_t flag=0;
  //uint32_t sleep=8;
  //system_rtc_mem_write(64,&flag,4);
  //system_rtc_mem_write(68,&sleep,4);

  uint32_t flag;
  uint32_t sleep;
  system_rtc_mem_read(64,&flag,4);
  if(!flag){
    system_rtc_mem_read(68,&sleep,4);
    sleep--;

    if(sleep>0 && sleep<8){
      //sleep for 3:25 hours:minutes asummed from this site: https://thingpulse.com/max-deep-sleep-for-esp8266/
      system_rtc_mem_write(68,&sleep,4);
      ESP.deepSleep(sleepTimeUs,WAKE_RF_DEFAULT);
    }else{
      flag=1;
      system_rtc_mem_write(64,&flag,4);
    }
  }

  digitalWrite(TFT_POWER,HIGH);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
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
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  
  tft.setCursor(0, tft.getCursorY()+10);
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
      int startIndex= payload.indexOf("\"quote\":") + 10;
      String quote=payload.substring(startIndex);
      int endIndex= quote.indexOf('\"');
      quote=quote.substring(0,endIndex);
      
      startIndex= payload.indexOf("\"author\":")+ 11;
      String author=payload.substring(startIndex);
      endIndex= author.indexOf('\"');
      author=author.substring(0,endIndex);
      
      tft.setTextSize(1); 
      tft.setCursor(30, 110);
      tft.println(quote);                                                     //Print the response payload
      tft.setCursor(200, tft.getCursorY()+10);  
      tft.println(author);  
    }
    http.end();                                                               //Close connection
  }

  flag=0;
  sleep=8;
  system_rtc_mem_write(64,&flag,4);
  system_rtc_mem_write(68,&sleep,4);

  tft.println("Going to sleep!");
  //light sleep for 4 seconds
  delay(40000);
  //sleep for 3:25 hours:minutes asummed from this site: https://thingpulse.com/max-deep-sleep-for-esp8266/
  ESP.deepSleep(sleepTimeUs,WAKE_RF_DEFAULT);     //when the ESP go to sleep it turns low all the pins, including the TFT_POWER
}

void loop() {
  //do nothing
}
