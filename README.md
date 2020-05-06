# DailyQuotes
Get daily quotes on TFT screen, controlled by an IL9341 controller, connected to an ESP8266.

Check the [DEMO!](https://www.youtube.com/watch?v=v00FHcqeyeM)! :smirk:

I.How it works?
 1. When the system is pluged in to power, it will connect to the WiFi (the connection informations --- ssid and password ---, must be configured in code ), will display an inspirational quote for one hour and go to sleep for 24 hours. After the long sleep, it will connect again to the WiFi, display another quote and go to sleep. Again and again.
 2. But there is a plot twist. The used microcontroller (ESP8266) can go to sleep for only 3 hours and 25 minutes (according to the Internet --- see IV.Credits --- ). So I decided to wake up the microcontroller after the before mentioned time (3 hours and 25 minutes) and then put it back to sleep. This will be done 7 times (a total of 23 hours). To keep track of the numbers of wake-ups between 2 quotes, I saved a counter in the RTC user memory from the ESP8266 ( --- see IV.Credits --- ). In this way, when the microcontroller goes to sleep the counter will be kept.
 3. This seems to be all, but... IT IS NOT! The RTC from ESP8266, when this microcontroller is in DeepSleepMode, is not very precise ( in my tests it will fall behind with at least 1 hour in 23 hours of DeepSleep ). So to fix this issue, I made the ESP8266 to take the time from an NTP Server, after displaying the quote for an hour, and storing the hours and minutes in the RTC memory. When the 7 cycles of waking-up and going back to sleep, the ESP8266 take again the time from the NTP Server and compare the stored time with the one took seconds ago. If the hour stored is later than the current hour, the ESP8266 will go to DeepSleep for the remaining time between the two intervals of time ( the stored one and the current one ).Else, if the number of minutes stored are greater than the number of current minutes, the ESP8266 will go to LightSleep for the remaining number of minutes between the two intervals of time ( this time, the RTC is precise enough to wake up the ESP8266 at the exact time ).
 4. And this is how it works! :D 	

II.What I learned?

 1.ESP8266 deepSleepMode - enable/disable, setup, how it works, functional parameters(Amperage, Voltage, max time, RTC clock deviation);
 
 2.ESP8266 lightSleepMode - enable/disable, setup, how it works, functional parameters(Amperage, Voltage, max time);
 
 3.ESP8266 WiFi - enable/disable, setup, functional parameters(Amperage, Voltage, max time);
 
 4.ESP8266 RTC user memory - read/write in RTC user memory, memory mapping for the include RTC from the ESP8266, bit shifting;
 
 5.ESP8266 Client/NTP - Get() from a server, Client Mode, how NTP works, NTP client, Json parsing with basic string functions;
 
 6.ILI9341 TFT controller - memory mapping, internal commands, functional parameters;

III.Steps to implement:
 1. Install the libraries for TFT (IV.Credits 5.TFT libraries)
 2. Set up the ESP8266 (IV.Credits 2.Set up ESP8266 in Arduino IDE)
 3. Build the electrical circuit
 4. Upload the code with the needed changes (ssid and password)

IV.Components:
 1. ESP8266 (preferable and NodeMCU or any other development board, for easy connections );
 2. Any display with IL9341 controller ( mine is a 2.8inch TFT_LCD 240*320 RGB screen );
 3. Wires to connect the ESP8266 and the Display;
 4. 2n2222 transistor and 1kOhm resistor for the command of the TFT power;
 5. 1kOhm resistor for the connection between GPIO16 and RST ( needed for waking-up from DeepSleepMode );

V.Credits:
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

VI.Electrical circuit:
![Circuit](/Images/Circuit.PNG)

VII.The project ( on layers ):
![Bottom_ESP8266](/Images/Bottom_ESP8266.jpg)
![Bottom_Circuit](/Images/Bottom_Circuit.jpg)
![Top_Circuit](/Images/Top_Circuit.jpg)
![Top_Display](/Images/Top_Display.jpg)
