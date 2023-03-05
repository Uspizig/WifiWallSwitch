A AirQ&EarthQ-LoRa EInk WallSwitch that measures your Air Quality, monitores Earthquakes, measures Temperature

Please see https://hackaday.io/project/189884-airqearthq-lora-eink-wallswitch for further details

This files are additional Material for AirQ&EarthQ-LoRa EInk WallSwitch project published on Hackaday.io (https://hackaday.io/project/189884-airqearthq-lora-eink-wallswitch)


WARNINGS

Before you compile please ensure you modify all your credentials and used sensors in "credentials.h" which have been marked REPLACEMEUSER
This Source Code is provided "AS it is" . If you use this code you agree that any harm, damage, burn or injuries are YOUR RESPONSIBILITY! 
The Author may not be held liable for any damage that might happen if you try this code out.
Legal validity of this disclaimer: This disclaimer is to be regarded as part of the internet publication which you were referred from. 
If sections or individual terms of this statement are not legal or correct, the content or validity of the other parts remain uninfluenced by this fact.

Referrals and links The author is not responsible for any contents linked or referred to from his pages - unless he has full knowledge of illegal contents and would be able to prevent the visitors of his site from viewing those pages. If any damage occurs by the use of information presented there, the author might not be liable. Furthermore the author is not liable for any postings or messages published by users of discussion boards, guest books or mailing lists provided.

Also ensure you have installed the following libs:

Code Sources that influenced this Source Code and which libaries you may need:

https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_auth/mqtt_auth.ino
https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
RGBW for SK6812:
Original code by Jim Bumgardner (http://krazydad.com).
Modified by David Madison (http://partsnotincluded.com).
Extended by Christoph Wempe
https://gist.github.com/CWempe?direction=desc&sort=created

Adafruit BME/BMP280/SGP30 sketches/libs
Please consider buying their products due to their great work in Arduino libs
https://github.com/adafruit/Adafruit_SGP30
https://github.com/adafruit/Adafruit_BMP280_Library

Bosch BME680 BSEC Lib
https://github.com/BoschSensortec/BSEC-Arduino-library

Andreas Spiess
OTA Sketch for OTA Update of the ESP32 over Wifi
https://github.com/SensorsIot/ESP32-OTA

Knolleary pubsubclient

/pubsubclient/blob/master/examples/mqtt_auth/mqtt_auth
https://github.com/knolleary/pubsubclient

Fastled
https://github.com/FastLED/FastLED

/* Goal: Project Goal: Build a Desktop Sensorset that can measure the Air Quality, Detect Earthquakes and warn the user if certain levels are reached.

What is working:
- None

What is planned:
- EInk Touch Screen 2.7 '
- A Touch screen
- Ultra Low Power RTC RV3028 for Low Power Battery Operation wakeup
- Ultra Low Noise MEMS LSM6DSL GYRO+ACCEL for Earthquake Detection-> https://github.com/biagiom/QuakeSense
- Ultra Low Noise MEMS LSM6DSL for movement detection and Tap Detection
- Ultra Low Power Mode TPL5110: Let the Processor sleep with TPL5110 Power Gateing and shut down all that is battery draining.
- 9 physical buttons for moments where you do not wnat to use a touchscreen
- LORA:
  - Transmit Air Quality Data on a regular Basis (every 15min)
  - Transmit Data when Air Quality changes dramatically
  - Transmit Data when Air Quality changes dramatically
  - Recieve Commands to Switch on / off ext Relay
  - Switch to Desaster Mode and search for BLE Heart Rate Devices nearby and sent this data via LoRa

