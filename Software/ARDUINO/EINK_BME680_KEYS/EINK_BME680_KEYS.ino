
//Display Tap Erkennung
//Anzeige von Werten
//Grafen
// Probleme: immer 500 ppm Co2

//BrownOut Disable
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
//Task Watchdog
#include <esp_task_wdt.h>

//#include <GxEPD.h>//nur für fonts???
#include "user_kennung.h"
#include <time.h>               // In-built

// select the display class to use, only one

//#include <GxGDEW027W3/GxGDEW027W3.h>      // 2.7" b/w
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include "GxEPD2_boards_added.h"

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
//#include <Fonts/Homemade_Apple_Regular_9.h>
//#include <Fonts/Crafty_Girls_Regular_12.h>
//#include <Fonts/Aclonica_Regular_16.h>
//#include <Fonts/Dialog6pt.h>

//#include <Fonts/FreeSansBold12pt7b.h>
//#include <Fonts/FreeSerifBold12pt7b.h>

const GFXfont* Schrift1 = &FreeMonoBold12pt7b;
const GFXfont* Schrift2 = &FreeMonoBold9pt7b;
const GFXfont* Schrift3 = &FreeMonoBold18pt7b; 
const GFXfont* Schrift4 = &FreeMono9pt7b; 
//const GFXfont* Schrift2 = &Crafty_Girls_Regular_12; 
//const GFXfont* Schrift2 = &Aclonica_Regular_16;
//const GFXfont* Schrift5 = &FreeSansBold12pt7b; 
//const GFXfont* Schrift6 = &Dialog_plain_8;
const GFXfont* Schrift6 = &FreeMono9pt7b; 

#include "SparkFunLSM6DS3.h"
#include "Wire.h"
//https://www.st.com/resource/en/application_note/an5040-lsm6dsl-alwayson-3d-accelerometer-and-3d-gyroscope-stmicroelectronics.pdf

LSM6DS3 myIMU( I2C_MODE, 0x6A );
#define INT1_PIN 8
uint8_t int1Status;



//Messdaten
//RealTimeVariables which don't get erased during Sleep
  struct Daten_struct {//https://esp32.com/viewtopic.php?t=11984
    //uint16_t Stack[15];
    float Stack[15];
    uint8_t Counter_History;
  };
  int IconSize = 11;//10
  int x = 10;
  int y = 40;
  RTC_DATA_ATTR  struct Daten_struct Druck_Werte;                 
  RTC_DATA_ATTR  struct Daten_struct Temp_Werte;
  RTC_DATA_ATTR  struct Daten_struct Feuchte_Werte;

  RTC_DATA_ATTR  struct Daten_struct CO2_Werte;
  RTC_DATA_ATTR  struct Daten_struct TVOC_Werte;                                                   
  RTC_DATA_ATTR  struct Daten_struct IAQ_Werte;                                                   
  RTC_DATA_ATTR  struct Daten_struct STATIC_IAQ_Werte;                                                   

  RTC_DATA_ATTR  struct Daten_struct BREATHEVOC_Werte;
  RTC_DATA_ATTR  struct Daten_struct GAS_Werte;  
  RTC_DATA_ATTR  struct Daten_struct STABILITY_Werte; 
  RTC_DATA_ATTR  struct Daten_struct RUNIN_Werte;
  RTC_DATA_ATTR  struct Daten_struct RUNIN_IAQ_Werte;
  
  RTC_DATA_ATTR  struct Daten_struct ROLL_Werte;
  RTC_DATA_ATTR  struct Daten_struct PITCH_Werte;

    
 


//Uhrzeit:
const char* Timezone    = "CET-1CEST,M3.5.0,M10.5.0/3";  
const char* ntpServer   = "1.de.pool.ntp.org";
int   gmtOffset_sec     = 0;
int   daylightOffset_sec = 3600;
String  Time_str = "--:--:--";
String  Date_str = "-- --- ----";
int     wifi_signal, CurrentHour = 0, CurrentMin = 0, CurrentSec = 0, EventCnt = 0, vref = 1100;
//Day of the week
const char* weekday_D[] = { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" };

//Month
const char* month_M[] = { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dez" };



#include "bsec.h"

// Helper functions declarations
void checkIaqSensorStatus(void);

#define SDA2 42
#define SCL2 2
#define LED_BUILTIN 41
// Create an object of the class Bsec
Bsec iaqSensor;

  
  
  int zahlen = 0;
  float temp_BME680 = 0;
  float humidity_BME680 = 0;
  float pressure_BME680 = 0;

  float co2_BME680 = 0;
  float iaq_BME680 = 0;
  float iaq_Accuracy_BME680 = 0;
    
  float staticIaq_BME680 = 0;
  float breatheVoc_BME680 = 0;
  float gas_BME680 = 0;
    
  float stability_BME680 = 0;
  float runInStatus_BME680 = 0;
    

//#define debug false

#include <WiFi.h>  
#include <SPI.h>   

#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
//WiFiClient client;
//WiFiClientSecure client2;
WiFiClientSecure client;


#include <WiFiManager.h>

//Buttons
//Definitionen für Keyboard
uint8_t keyboard_val = 0;
 #define KEYPRESS_DETECTION_VALUE 1  
//Keyboard PINS ESP32-S3
  #define ROW_NUM     3 
  #define COLUMN_NUM  3 
  #define ROW1 7
  #define ROW2 15
  #define ROW3 16
  #define COL1 4
  #define COL2 5
  #define COL3 6



//Eink Mini epaper Board -> ESP32 S3
  #define E_INK_PIN_SPI_BUSY 40
  #define E_INK_PIN_SPI_CS   47
  #define E_INK_PIN_SPI_RST  39//21
  #define E_INK_PIN_SPI_DC   38
  #define E_INK_PIN_SPI_SCK  21
  #define E_INK_PIN_SPI_DIN  9
  #define E_INK_PIN_SPI_MISO 17//n/A

    long lastTrigger_SENSOR=0;
    long lastTrigger_DISPLAY=0;
    long lastTrigger_KEYS=0;
    #define UPDATERATE_SENSOR 2000
    #define UPDATERATE_DISPLAY 60000
    #define UPDATERATE_KEYS 200
    int Display_Page_Number = 0;

  
//GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> gfx(GxEPD2_270c(/*CS=5*/ E_INK_PIN_SPI_CS, /*DC=*/ E_INK_PIN_SPI_DC, /*RST=*/ E_INK_PIN_SPI_RST, /*BUSY=*/ E_INK_PIN_SPI_BUSY));
GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> gfx(GxEPD2_270(/*CS=5*/ E_INK_PIN_SPI_CS, /*DC=*/ E_INK_PIN_SPI_DC, /*RST=*/ E_INK_PIN_SPI_RST, /*BUSY=*/ E_INK_PIN_SPI_BUSY));

/*------------------------------------------ EINK DISPLAY TEIL--------------------------------------------------------------*/
bool LargeIcon =  true;
bool SmallIcon =  false;
int fail_counter = 0;
//#define Anzeige_42
#define Anzeige_213
#define LORA_RESET 15

#ifdef Anzeige_42
  #define Large  7
  #define Small  4
  #define xanpassung 0
  //#define weite 128 
  //#define hoehe 296
#endif
#ifdef Anzeige_213
  #define Large  5
  #define Small  4
  #define xanpassung -30
#endif



#define uS_TO_S_FACTOR 1000000UL
#define TIME_TO_SLEEP_MAX 80UL
#define TIME_TO_SLEEP_CASE1 3600UL
#define TIME_TO_SLEEP_CASE2 9000UL


void IRAM_ATTR int1Handler() {
    int1Status++;
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);//BrownOut Disable
  esp_task_wdt_init(30, false); //Task Watchdog set to 30sec
  delay(500);
  Serial.begin(115200);
  BME680_init();
  Display_Start(); //Initalisiert das Display
  delay(500);
  Serial.println("WifiManager_Start");
  wifi_manager_start();
  //wifi_start();//macht Wifi an und Basis Funktion für schnelles Connecten
  //if (init_wifi_ota()){Serial.println("Wifi aktiv");} //startet Verbindung zum Wifi
  Serial.println("Config Gyro");
  config_Gyro();
  UpdateLocalTime();
  Serial.println("setup done");
}


void loop()
{
   
   unsigned long now = millis();
   if (int1Status > 0) {
    checkForDoubleTap();
    int1Status = 0;
    Display_Data(Display_Page_Number);
   }
   if(now - lastTrigger_SENSOR > (UPDATERATE_SENSOR)){
      Measure_BME680();
      lastTrigger_SENSOR = millis();
      
   }
   if(now - lastTrigger_DISPLAY > (UPDATERATE_DISPLAY)){
      Display_Data(Display_Page_Number);
      lastTrigger_DISPLAY = millis();
      
   }
   if(now - lastTrigger_KEYS > (UPDATERATE_KEYS)){
    keyboard_val =  key_test2();
    if (keyboard_val != 0){
        Serial.println(keyboard_val);
        keyboard_val = 0;
    }
    lastTrigger_KEYS = millis();
   }  

}


void Measure_BME680(void){
  if (iaqSensor.run()) { 
    temp_BME680 = iaqSensor.temperature;
    humidity_BME680 = iaqSensor.humidity;
    pressure_BME680 = iaqSensor.pressure;

    co2_BME680 = iaqSensor.co2Equivalent;
    iaq_BME680 = iaqSensor.iaq;
    iaq_Accuracy_BME680 = iaqSensor.iaqAccuracy;
    
    staticIaq_BME680 = iaqSensor.staticIaq;
    breatheVoc_BME680 = iaqSensor.breathVocEquivalent;  
    gas_BME680 = iaqSensor.gasResistance;
    
    stability_BME680 = iaqSensor.stabStatus;
    runInStatus_BME680 = iaqSensor.runInStatus;
    Serial.print("BME680 CO2: ");Serial.println(co2_BME680);

    
    SAVE_DATA_2_RTC(pressure_BME680, &Druck_Werte);
    SAVE_DATA_2_RTC(temp_BME680, &Temp_Werte);
    SAVE_DATA_2_RTC(humidity_BME680, &Feuchte_Werte);  

    SAVE_DATA_2_RTC(co2_BME680, &CO2_Werte);
    SAVE_DATA_2_RTC(iaq_BME680, &IAQ_Werte);
    SAVE_DATA_2_RTC(staticIaq_BME680, &STATIC_IAQ_Werte);
    SAVE_DATA_2_RTC(iaq_Accuracy_BME680, &RUNIN_IAQ_Werte);

    SAVE_DATA_2_RTC(breatheVoc_BME680, &BREATHEVOC_Werte);
    SAVE_DATA_2_RTC(gas_BME680, &GAS_Werte);
    SAVE_DATA_2_RTC(stability_BME680, &STABILITY_Werte);
    SAVE_DATA_2_RTC(runInStatus_BME680, &RUNIN_Werte);
    //gasPercentage_BME680 = iaqSensor.gasPercentage;
  }
  else {
    checkIaqSensorStatus();
  }

}

void Display_Data(int Display_Page_Number){
   
   unsigned long time_trigger = millis();
   switch(Display_Page_Number){
     case 0:
        gfx.fillRect(0, 0, 180, 40, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 20);
        gfx.print("- P0 - Status -");
        Verlauf_Anzeige(x, y, IconSize, &Temp_Werte, "Temp:"); // Diagramm Temp
        Verlauf_Anzeige(x, y+80, IconSize, &Feuchte_Werte, "Feuchte:"); // Feuchte Temp

        gfx.fillRect(0, 200, 180, 260, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 220); gfx.print("Time:"); gfx.print(CurrentHour); gfx.print(":");gfx.print(CurrentMin); gfx.print(":");gfx.print(CurrentSec);
        gfx.setCursor(10, 240); gfx.print("Temp:"); gfx.print(temp_BME680);     
        gfx.setCursor(10, 260); gfx.print("Feuchte:"); gfx.print(humidity_BME680);     
        //gfx.setCursor(10, 260); gfx.print("Druck:"); gfx.print(pressure_BME680); 
        gfx.display(true);     
        break;
     case 1:
        gfx.fillRect(0, 0, 180, 40, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 20);
        gfx.print("- P1 - CO2 -");
        Verlauf_Anzeige(x, y, IconSize, &CO2_Werte, "CO2:"); // Diagramm Temp
        Verlauf_Anzeige(x, y+80, IconSize, &IAQ_Werte, "IAQ:"); // Diagramm Temp
        
        gfx.fillRect(0, 200, 180, 260, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 220); gfx.print("Time:");  gfx.print(CurrentHour); gfx.print(":");gfx.print(CurrentMin); gfx.print(":");gfx.print(CurrentSec);
        gfx.setCursor(10, 240); gfx.print("CO2:"); gfx.print(co2_BME680);     
        gfx.setCursor(10, 260); gfx.print("IAQ:"); gfx.print(iaq_BME680);     
        //gfx.setCursor(10, 260); gfx.print("Valid:"); gfx.print(iaq_Accuracy_BME680); 
        gfx.display(true);   
     break;
     case 2:
        gfx.fillRect(0, 0, 180, 40, GxEPD_WHITE);
        gfx.setCursor(10, 20);
        gfx.print("- P2 - VOC -");
        gfx.setFont(Schrift2);
        Verlauf_Anzeige(x, y, IconSize, &STATIC_IAQ_Werte, "IAQ:"); // Diagramm Temp
        Verlauf_Anzeige(x, y+80, IconSize, &BREATHEVOC_Werte, "Atem:"); // Diagramm Temp
        gfx.fillRect(0, 200, 180, 260, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 220); gfx.print("Time:"); gfx.print(CurrentHour); gfx.print(":");gfx.print(CurrentMin); gfx.print(":");gfx.print(CurrentSec);
        gfx.setCursor(10, 240); gfx.print("sIAQ:"); gfx.print(staticIaq_BME680);     
        gfx.setCursor(10, 260); gfx.print("VOC:"); gfx.print(breatheVoc_BME680);     
        //gfx.setCursor(10, 260); gfx.print("GAS:"); gfx.print(gas_BME680); 
        gfx.display(true);   

     break;
     case 3:
        gfx.fillRect(0, 0, 180, 40, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 20);
        gfx.print("- P3 - GAS -");
        Verlauf_Anzeige(x, y, IconSize, &RUNIN_Werte, "RUNIN:"); // Diagramm Temp
        Verlauf_Anzeige(x, y+80, IconSize, &RUNIN_IAQ_Werte, "RIAQ:"); // Diagramm 
        gfx.setFont(Schrift2);
        gfx.fillRect(0, 200, 180, 260, GxEPD_WHITE);
        //gfx.setCursor(10, 220); gfx.print("Time:"); gfx.print(CurrentHour); gfx.print(":");gfx.print(CurrentMin); gfx.print(":");gfx.print(CurrentSec);
        gfx.setCursor(10, 220); gfx.print("RIAQ:"); gfx.print(iaq_Accuracy_BME680);     
        gfx.setCursor(10, 240); gfx.print("Stability:"); gfx.print(stability_BME680);     
        gfx.setCursor(10, 260); gfx.print("RUNIN:"); gfx.print(runInStatus_BME680);     
        //gfx.setCursor(10, 260); gfx.print("GAS:"); gfx.print(gas_BME680); 
        gfx.display(true);   
     break;
     case 4:
       gfx.fillRect(0, 0, 180, 40, GxEPD_WHITE);
       gfx.setFont(Schrift2);
       gfx.setCursor(10, 20);
       gfx.print("- P4 - WIFI -");
        Verlauf_Anzeige(x, y, IconSize, &Temp_Werte, "Temp:"); // Diagramm Temp
        Verlauf_Anzeige(x, y+80, IconSize, &CO2_Werte, "CO2:"); // Diagramm 
        gfx.fillRect(0, 200, 180, 260, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 220); gfx.print("Time:"); gfx.print(CurrentHour); gfx.print(":");gfx.print(CurrentMin); gfx.print(":");gfx.print(CurrentSec);
        //gfx.setCursor(10, 220); gfx.print("Stability:"); gfx.print(stability_BME680);     
        gfx.setCursor(10, 240); gfx.print("SSID:"); gfx.print(WLAN_SSID);     
        gfx.setCursor(10, 260); gfx.print("RSSI:"); gfx.print(WiFi.RSSI()); 
        gfx.display(true);  
     break;
     case 5:
       gfx.fillRect(0, 0, 180, 40, GxEPD_WHITE);
       gfx.setFont(Schrift2);
       gfx.setCursor(10, 20);
       gfx.print("- P5 - Gyro -");
        Verlauf_Anzeige(x, y, IconSize, &ROLL_Werte, "Roll:"); // Diagramm Temp
        Verlauf_Anzeige(x, y+80, IconSize, &PITCH_Werte, "Pitch:"); // Diagramm 
        gfx.fillRect(0, 200, 180, 260, GxEPD_WHITE);
        gfx.setFont(Schrift2);
        gfx.setCursor(10, 220); gfx.print("Time:"); gfx.print(CurrentHour); gfx.print(":");gfx.print(CurrentMin); gfx.print(":");gfx.print(CurrentSec);
        //gfx.setCursor(10, 220); gfx.print("Stability:"); gfx.print(stability_BME680);     
        gfx.setCursor(10, 240); gfx.print("SSID:"); gfx.print(WLAN_SSID);     
        gfx.setCursor(10, 260); gfx.print("RSSI:"); gfx.print(WiFi.RSSI()); 
        gfx.display(true);  
     break;
     default:
     break;

   }
   
}

// Helper function definitions
void checkIaqSensorStatus(void)
{
  String output;
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      for (;;)
        //errLeds(); /* Halt in case of failure */
        Serial.println("Defective");
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      for (;;)
       // errLeds(); /* Halt in case of failure */
       Serial.println("Defective");
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}

void BME680_init(void){
  Wire.begin(SDA2, SCL2);

  //iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
  iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
  String output;
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

   bsec_virtual_sensor_t sensorList[13] = {
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_STABILIZATION_STATUS,
    BSEC_OUTPUT_RUN_IN_STATUS,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    BSEC_OUTPUT_GAS_PERCENTAGE
  };

  iaqSensor.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
  output = "Timestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
  Serial.println(output);

}

//versucht 3 Mal eine Verbindung herzustellen. Geht dann schlafen, Wartet bei jedem Versuch 10 Sec
void retry_mechanism(void){
    delay(10000);
    fail_counter++;
    if(fail_counter < 4){
      Serial.println("Probleme ich starte nochmal einen Verbindunsgversuch");
    }
    else{
      Serial.println("Schwerwiegende Verbindungsprobleme -- 5 x Retries erfolglos");
      delay(500);
      begin_sleep();
      delay(120000);
    }
}

//Initalisiert das E-Ink Display mit den wichtigsten Grundparametern
void Display_Start(void){
  
  gfx.epd2.init(E_INK_PIN_SPI_SCK, E_INK_PIN_SPI_DIN, 115200, true, 20, false); 
  delay(100);
  gfx.setRotation(0); gfx.fillScreen(GxEPD_WHITE); gfx.setTextColor(GxEPD_BLACK);
  //gfx.fillCircle(15,70,50, GxEPD_BLACK);
  //gfx.fillRect(0, 0, 120, 80, GxEPD_BLACK);
  Serial.println("Display setup done");
  //gfx.display(false);
  //gfx.drawFastHLine(0, 115, gfx.width(), GxEPD_BLACK); //Linie horizontal Volle Breite
  gfx.setCursor(65, 90); gfx.setFont(Schrift2); 
  gfx.display(false);
  /*
    gfx.print("Wallswitch");     
    gfx.display(true);
    
    
    gfx.setCursor(10, 140); gfx.setFont(Schrift2);
    gfx.print("WiFi Check");     
    gfx.display(true);


    gfx.setCursor(10, 160); gfx.setFont(Schrift2);
    gfx.print("E-Ink Display -");     
    gfx.display(true);
  
    gfx.setCursor(10, 180); gfx.setFont(Schrift2);
    gfx.print("BME680 Test -");     
    gfx.display(true);
  */
  

}

//Grundfunktionen des Wifi, Wifi anmachen, 1. Verbindungsversuch,  Hostname setzen
void wifi_start(void){
  WiFi.setHostname("muelli");
  WiFi.mode(WIFI_STA); // WLAN auf Client setzen
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print(F("Verbinde mit "));
  Serial.println(WLAN_SSID);
  WiFi.setSleep(false); //Deaktiviert WIFI Sleep
}

void wifi_manager_start(void){
   WiFi.mode(WIFI_STA); 
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
}


// Wifi Verbindungsversuch mit Retry Mechainsmus und Energiesparen bei WLAN OFFLINE
int init_wifi_ota(void){
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    delay(500);
    int wifi_retries = 0;
    Serial.println("Check ob  WLAN verbunden :-> ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(135);//150 läuft besser
        wifi_retries++;        
        if (wifi_retries > 30) {
          Serial.println("2. Anlauf WLAN :-< ");
          WiFi.begin(WLAN_SSID, WLAN_PASS);
          delay(1500);
          if(WiFi.status() != WL_CONNECTED){
              Serial.println("WLAN geht gar nicht :-( ");
              /*gfx.setCursor(15, 55); gfx.println("\nWLAN ist OFF-line :-<");
              gfx.display(); delay(1800);*/
              begin_sleep();
              delay(180);
              return 0;
              delay(180);
          }
          else{
            Serial.println("2. Anlauf WLAN geht :-) ");
            return 1;
          }
       }
        
    }
    Serial.println("");
    Serial.println("WiFi connected");
    return 1;
}

void begin_sleep(){
  //gfx.sleep();
  esp_sleep_enable_timer_wakeup(UpdateInterval);
  //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_CASE2 * uS_TO_S_FACTOR);
  Serial.print(F("Starte Dornrösschen-Schlaf.."));
  #ifdef BUILTIN_LED
    pinMode(BUILTIN_LED,INPUT);     // In case it's on, turn output off, sometimes PIN-5 on some boards is used for SPI-SS
    digitalWrite(BUILTIN_LED,HIGH); // In case it's on, turn LED off, as sometimes PIN-5 on some boards is used for SPI-SS
  #endif
  esp_deep_sleep_start();         // Sleep for e.g. 30 minutes
}

void config_Gyro(void){
  if ( myIMU.begin() != 0 )
    {
      Serial.print("Error at beginCore().\n");
    }
    else
    {
      Serial.print("\nbeginCore() passed.\n");
    }

    //Error accumulation variable
    uint8_t errorAccumulator = 0;
    uint8_t dataToWrite = 0; //Temporary variable

    //Setup the accelerometer******************************
    dataToWrite = 0; //Start Fresh!
    dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_200Hz;
    dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
    dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_416Hz;

    // //Now, write the patched together data
    errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);

    //Set the ODR bit
    errorAccumulator += myIMU.readRegister(&dataToWrite, LSM6DS3_ACC_GYRO_CTRL4_C);
    dataToWrite &= ~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);

    // Enable tap detection on X, Y, Z axis, but do not latch output

    errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_CFG1, 0x8E ); // org 0x0E

    // Set tap threshold
    // Write 0Ch into TAP_THS_6D
    errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_THS_6D, 0x8C ); // 0x03

    // Set Duration, Quiet and Shock time windows
    // Write 7Fh into INT_DUR2
    errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_INT_DUR2, 0x7F );

    // Single & Double tap enabled (SINGLE_DOUBLE_TAP = 1)
    // Write 80h into WAKE_UP_THS
    errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x80 );

    // Single tap interrupt driven to INT1 pin -- enable latch
    // Write 08h into MD1_CFG
    errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD1_CFG, 0x48 ); // 0x48

    if ( errorAccumulator )
    {
      Serial.println("Problem configuring the device.");
    }
    else
    {
      Serial.println("Device O.K.");
    }

    pinMode(INT1_PIN, INPUT);
    attachInterrupt(INT1_PIN, int1Handler, RISING);

}

bool checkForDoubleTap()
{
    
    uint8_t reg;
    myIMU.readRegister(&reg, LSM6DS3_ACC_GYRO_TAP_SRC);

    
      UpdateLocalTime();
      if (((reg & 0x40) == 0x40) && ((reg & 0x10) == 0x10)) {     // double tap
        Serial.print("double tap ");
        Serial.println(millis());
        Display_Page_Number = 0;
        
        return true;
      }
      else{
        Serial.print("single tap ");
        Serial.println(millis());
        if (Display_Page_Number < 6){
          Display_Page_Number++;
          int x, y, z;                        //three axis acceleration data
          double roll = 0.00, pitch = 0.00;       //Roll & Pitch are the angles which rotate by the axis X and y
          x = myIMU.readFloatAccelX();
          y = myIMU.readFloatAccelY();
          z = myIMU.readFloatAccelZ();
          double x_Buff = float(x);
          double y_Buff = float(y);
          double z_Buff = float(z);
          
          roll = atan2(y_Buff , z_Buff) * 57.3;
          pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
          SAVE_DATA_2_RTC(roll, &ROLL_Werte);
          SAVE_DATA_2_RTC(pitch, &PITCH_Werte);
          Serial.print("Roll: "); Serial.print(roll);
          Serial.print(" Pitch: "); Serial.println(pitch);
          Serial.print(" A: ");
          Serial.print(" X = ");
          Serial.print(x, 1);
          Serial.print(" Y = ");
          Serial.print(y, 1);
          Serial.print(" Z = ");
          Serial.println(z, 1);

          Serial.print(" G: ");
          Serial.print(" X = ");
          Serial.print(myIMU.readFloatGyroX(), 1);
          Serial.print(" Y = ");
          Serial.print(myIMU.readFloatGyroY(), 1);
          Serial.print(" Z = ");
          Serial.println(myIMU.readFloatGyroZ(), 1);

          Serial.print(" T: ");
          Serial.println(myIMU.readTempC(), 1);
        }
        else{
          Display_Page_Number = 0;
        }  
        return true;
        
      }
    
    return false;
}


boolean UpdateLocalTime() {
  struct tm timeinfo;
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return false;
 }
 else{
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  CurrentHour = timeinfo.tm_hour;
  CurrentMin  = timeinfo.tm_min;
  CurrentSec  = timeinfo.tm_sec;
  return true;
 }
}

int key_test2(void){
    String str = "";
    int rueckgabe = 0;
    int nRow = 3;
    int nCol = 3;
    int KeyBoard[nRow][nCol] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    uint8_t colPins[3] = {ROW1, ROW2, ROW3}; //connect to the column pinouts of the keypad
    pinMode(ROW1, OUTPUT);
    pinMode(ROW2, OUTPUT);
    pinMode(ROW3, OUTPUT);
    pinMode(COL1, INPUT);
    pinMode(COL2, INPUT);
    pinMode(COL3, INPUT);
    
    //Abfrage der Tasten
    for (int i = 0; i < nCol; i++){
      digitalWrite(colPins[i], LOW);  
      KeyBoard[i][0] = digitalRead(COL1); 
      KeyBoard[i][1] = digitalRead(COL2); 
      KeyBoard[i][2] = digitalRead(COL3); 
      digitalWrite(colPins[i], HIGH);  
      /*Serial.print("Col"); Serial.print(i);
      Serial.print(KeyBoard[i][0]);
      Serial.print(KeyBoard[i][1]);
      Serial.println(KeyBoard[i][2]);*/
      
    }
    
    
    //DebugAusgabe
    //#define WEB_DEBUG_SERIAL 
     for (int i = 0; i < nCol; i++){
      for (int b = 0; b < nRow; b++){
        int a = i+1; int c = b+1;
        if (KeyBoard[i][b] < KEYPRESS_DETECTION_VALUE){         
          #ifdef WEB_DEBUG_SERIAL
            Serial.print(str + a + c +":" + KeyBoard[i][b] +" \n");
          #endif
          rueckgabe = (a*10)+c;
        }
        }
      }
    return rueckgabe;
}



//LinienDiagramm Anzeige
void Verlauf_Anzeige(int x, int y, int IconSize, struct Daten_struct *daten, const char* header){
   int box_w = IconSize * 16;//13
   int box_h = IconSize * 8;//7
   int x2 = x+20;//-9
   int y2 = y+20;//10
   float max_value = 0;
   float min_value = 1500;
   int range = 30;

   for (int i = 0; i < 14; i++){//Finde max und Min Value
      if (daten->Stack[i] > max_value) max_value = daten->Stack[i];
      if (daten->Stack[i] < min_value) min_value = daten->Stack[i];
   }
   float mid_value = ((max_value - min_value) / 2) + min_value;
   gfx.fillRect(x-10, y-13, box_w, box_h, GxEPD_WHITE);
   gfx.drawRect(x-10, y-13, box_w, box_h, GxEPD_BLACK);
   gfx.setFont(&FreeMonoBold9pt7b);
   gfx.setCursor(x-5, y); gfx.print(header); 
   gfx.setFont(Schrift6);
   gfx.setCursor(x-5, y+20); gfx.print(max_value); 
   gfx.setCursor(x-5, y+40); gfx.print(mid_value);
   gfx.setCursor(x-5, y+60); gfx.print(min_value); 
   
   //Serial.println(header);Serial.print(": X_Header:");Serial.print(x-5); Serial.print(" / Y_Header:");Serial.print(y);
   //Serial.print("; Min_Value: ");Serial.print(min_value);Serial.print(" / Max_Value: ");Serial.println(max_value);  
   
   //Liniendiagramm V2
   if (max_value - min_value !=0){//nicht durch 0 teilen
      //all save
   }
   else{ //Teile niemals durch Null
     min_value = min_value - 0.1;//1
     daten->Stack[0] = daten->Stack[0] -0.1;//1
   }
       for (int i = 0; i < 13; i++){
          float data_a = ((float)(max_value - constrain(daten->Stack[i], min_value, max_value))) / ((float)(max_value - min_value));//Serial.print(";Data_a:"); Serial.print(data_a); //constrain prüft ob Werte im Wertebereich
          float data_b = ((float)(max_value - constrain(daten->Stack[i+1], min_value, max_value))) / ((float)(max_value - min_value)); //Serial.print(";Data_b:"); Serial.print(data_b); 
          int data1 = data_a * range;//Serial.print(";Data1:");Serial.print(data1);
          int data2 = data_b *range;//Serial.print("/ Data2:");Serial.print(data2);
          //Serial.print(i);Serial.print(": "); Serial.print(daten->Stack[i]);Serial.print(" / "); Serial.print(data1); Serial.print(" / "); Serial.print(data2); Serial.print(" / "); Serial.print(max_value); Serial.print(" / "); Serial.println(min_value);
          //Serial.print("/ x_Start:");Serial.print(x2+i*5); Serial.print("/ y_Start:");Serial.print(y2+data1);Serial.print("/ x_Ende:");Serial.print(x2+((i+1)*5));Serial.print("/ y_Ende:");Serial.println(y2+data2);
          /*gfx.drawLine(x2+(i*5), y2+data1, x2+((i+1)*5), y2+data2, GxEPD_BLACK);//Ursprungslinie
          gfx.drawLine(x2+(i*5), y2-1+data1, x2+((i+1)*5), y2-1+data2, GxEPD_BLACK);//Linienverstärkung um 1 nach unten versetzt
          gfx.drawLine(x2+(i*5), y2-2+data1, x2+((i+1)*5), y2-2+data2, GxEPD_BLACK);//Linienverstärkung um 2 nach unten versetzt*/
          gfx.drawLine(x2+(i*IconSize), y2+data1, x2+((i+1)*IconSize), y2+data2, GxEPD_BLACK);//Ursprungslinie
          gfx.drawLine(x2+(i*IconSize), y2-1+data1, x2+((i+1)*IconSize), y2-1+data2, GxEPD_BLACK);//Linienverstärkung um 1 nach unten versetzt
          gfx.drawLine(x2+(i*IconSize), y2-2+data1, x2+((i+1)*IconSize), y2-2+data2, GxEPD_BLACK);//Linienverstärkung um 2 nach unten versetzt
       } 

       //Balkendiagramm invertiert ohne max Begrenzung VORSICHT!!!
      /*
      for (int i = 0; i < 14; i++){
          int delta = 45 - ((daten->Stack[i])-min_value);
          display.fillRect(x2+i*5,  y+2, 4, 45,        GxEPD_BLACK);//1
          display.fillRect(x2+i*5,  y+2, 4, (45-delta),GxEPD_WHITE);//1
      }
      */
}

//***************************************************************************************************
//                                      SAVE DATA 2 RTC                                                 *
//***************************************************************************************************
// Sets basic Value for RTC Data fields
// Set all Values of Temp Pressure on first startup to a typical value
// Fills in Values according to LiFo, each value is shifted one higher. latest entries gets number 0
//***************************************************************************************************
void SAVE_DATA_2_RTC(float value, struct Daten_struct *daten){
   //Serial.println("Gespeicherte RTC-Daten:");
   for (int i = 14; i > 0; i--){
    daten->Stack[i] = daten->Stack[i-1];
    //Serial.print(i);Serial.print(":");Serial.print(daten->Stack[i]);Serial.print("/");
   }
   daten->Stack[0] = value;
   //Serial.print("0:");Serial.println(daten->Stack[0]);
}