extern "C" {                      //this library is used for the RTC memory read/write functions ( system_rtc_mem_write/read )
  #include "user_interface.h"     //
}                                 //

#include <ESP8266WiFi.h>          //include the library for the wifi connection
#include <ESP8266HTTPClient.h>    //

#include "SPI.h"                  //include the needed library for the TFT display
#include "Adafruit_GFX.h"         //
#include "Adafruit_ILI9341.h"     //

#define TFT_POWER 5               // Define the pins from ESP8266 that are used for the TFT screen
#define TFT_DC   12               // Pins defined with -1 are not connected,
#define TFT_CLK  13               // but need to be set this way to avoid errors in Adafruit_ILI9341 library
#define TFT_MOSI 14               //
#define TFT_CS   15               //
#define TFT_RST  -1               //
#define TFT_MISO -1               //


#define HOUR (unsigned long)36e5     //define the values for the time to sleep (unsigned long is used as parameter for delay()
                                     //one hour in milisecond (3.600.000 miliseconds=3.600 seconds=60 minutes=1 hour)
uint64_t sleepTimeUs = 123e8;        //123e8 for approximately 3 hours and 25 minutes
                                     //1e6   for 1 second
uint32_t sleepCounter;               //the sleep counter is used to make the ESP8266 sleep for 24 hours
                                     //ESP8266 can deepSleep for maximum 3 hours and 25 minutes
                                     //so we put the ESP8266 to deepSleep 7 times
                                     //3*7=21hours and 25*7=175minutes=2hour, in total 23 hours of deepSleep


const char* ssid = "yourSSID";          //the SSID data
const char* password = "yourPassword";  //    
byte networkTimeOut=20;                 //timeout for WiFi connection, in seconds    

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);    // Create a TFT object for the display

void setup() {
  
  wifi_set_sleep_type(LIGHT_SLEEP_T);       //Set the lightSleep mode when the delay() function is called
  
  checkSleepCounter();
  
  turnDisplayOn();
  
  tft.println("Hello!");                                         //Print the startup message
  tft.setTextSize(1);                                            //
  tft.println("I am here to make your day better.");             //
  tft.println("But first, let me connect to the internet!");     //

  connectWiFi();
  
  tft.setCursor(0, tft.getCursorY() + 10);                    //Inform the user that the network connection had succeded
  tft.setTextSize(2);                                         //
  tft.println("Done!");                                       //
  tft.setCursor(tft.getCursorX(), tft.getCursorY() + 10);     //
  tft.setTextSize(1);                                         //
  tft.println("Let's find an interesting quote!");            //

  displayQuote();

  initializeSleepCounter();
  delay(HOUR);                                    //light sleep for one hour
  
  goSleep();
}

void loop() {
  //do nothing
}

void turnDisplayOn(){
  pinMode(TFT_POWER, OUTPUT);        //if the couter had reached zero
  digitalWrite(TFT_POWER, HIGH);     //turn on the power supply for the TFT display

  tft.begin();                        //set the TFT display with the desired parameters
  tft.setRotation(1);                 //
  tft.fillScreen(ILI9341_BLACK);      //
  tft.setCursor(0, 0);                //
  tft.setTextColor(ILI9341_WHITE);    //
  tft.setTextSize(3);                 //
}

void goSleep(){
  tft.fillScreen(ILI9341_BLACK);                  //Clear the display
  digitalWrite(TFT_POWER, LOW);                   //turn off the power supply for the TFT display
  ESP.deepSleep(sleepTimeUs, WAKE_RF_DEFAULT);    //go to deepsleep
}

void checkSleepCounter(){
  system_rtc_mem_read(68, &sleepCounter, 4); //take the counter 
  --sleepCounter;                            // decrement it to mark another wakeUp

  if ( 0 < sleepCounter && sleepCounter < 7) { //check if the ESP8266 had sleept enough ( 7 times ) 
                                                //if the counter had not reached zero
    system_rtc_mem_write(68, &sleepCounter, 4);  //write the new counter to the RTC memory
    
    goSleep();
  }
}

void initializeSleepCounter(){
  //the initalization means that the ESP8266 will display the next quote after aproximately 24hours
  //from the time of the power on cycle, if the power supply is not disconnected in this interval
  sleepCounter=7;
  system_rtc_mem_write(68,&sleepCounter,4);
}

void connectWiFi(){
  WiFi.begin(ssid, password);                                               //Try to connect to the WiFi
  tft.print("Connecting");                                                  //
  while (WiFi.status() != WL_CONNECTED) {                                   //
    tft.print(".");                                                         //
    delay(1000);                                                            //
    --networkTimeOut;                                                       //Check if is network TimeOut
    if(networkTimeOut<=0){                                                  //
      tft.setCursor(0, tft.getCursorY() + 30);                              //
      tft.setTextSize(2);                                                   //
      tft.println("Can't connect to the WiFi!");                            //
      tft.println("Please check the WiFi info (ssid and password)!"); //
      delay(HOUR);                                                          //
    }                                                                       //
  }                                                                         //
}

void displayQuote(){
  if (WiFi.status() == WL_CONNECTED) {                                        //Check WiFi connection status
    HTTPClient http;                                                          //Declare an object of class HTTPClient
    http.begin("http://quotes.rest/qod.json?category=inspire");               //Specify request destination
    int httpCode = http.GET();                                                //Send the request
    if (httpCode > 0) {                                                       //Check the returning code
      String payload = http.getString();                                      //Get the request response payload
      tft.fillScreen(ILI9341_BLACK);                                          //Clear the display

      int startIndex = payload.indexOf("\"quote\":") + 10;                    //Extract the quote text from the string got from the internet
      String quote = payload.substring(startIndex);                           //
      int endIndex = quote.indexOf('\"');                                     //
      quote = quote.substring(0, endIndex);                                   //

      startIndex = payload.indexOf("\"author\":") + 11;                       //Extract the author text from the string got from the internet
      String author = payload.substring(startIndex);                          //
      endIndex = author.indexOf('\"');                                        //
      author = author.substring(0, endIndex);                                 //

      tft.setTextSize(1);                                                     //Print the qoute and the author
      tft.setCursor(30, 110);                                                 //
      tft.println(quote);                                                     //
      tft.setCursor(200, tft.getCursorY() + 10);                              //
      tft.println(author);                                                    //
    }
    http.end();                                                               //Close connection
  }
}
