# DailyQuotes
Get daily quotes on TFT screen, controlled by an IL9341 controller, connected to an ESP8266

Description:
    The system displays for one hour, at every 24 hours a new quote. After this, it goes to sleep to save as much power as possible. 
	It wakes up at aproximately 3 hours and 25 minutes ( the max time to sleep for ESP8266 ) and go to sleep again seven times (23 hours). This is detected by decrementing a variable stored in the RTC memory which is not erased when the ESP8266 wakes up from sleep.

Components:
1. ESP8266 (preferable and NodeMCU or any other development board, for easy connections );
2. Any dispaly with IL9341 controller ( mine is a 2.8inch TFT_LCD 240*320 RGB screen );
3. Wires to connect the ESP8266 and the Display;

Credits:
1. Arduino IDE: https://www.arduino.cc/
2. Set up ESP8266 in Arduino IDE: https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/
3. HTTP Request: https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
4. Quotes API from: https://theysaidso.com/api/
5. TFT libraries: https://github.com/adafruit/Adafruit_ILI9341
				  https://github.com/adafruit/Adafruit-GFX-Library
6. RTC memory guidance: https://www.esp8266.com/viewtopic.php?p=20009
						https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
						https://github.com/SensorsIot/ESP8266-RTC-Memory/blob/master/RTCmemTest/RTCmemTest.ino
7. Light sleep guidance: https://community.blynk.cc/t/esp8266-light-sleep/13584