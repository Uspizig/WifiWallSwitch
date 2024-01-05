#define mySSID "CHANGEME"
#define myPASSWORD "CHANGEME"
#define thingspeak_server "api.thingspeak.com"
#define thingspeak_api_key "xxxxxxxxxxxxxx"

//#define keyboard1 // ESP32 Wroom
#define keyboard2 // ESP32-S38N1 WROOM

#ifdef keyboard2
    //ESP32-S3 KeyboardBoard
    //#define mqtt_client_on
  //#define WEB_DEBUG_SERIAL 1
  #define ANZAHL_LEDS 8 
    #define RGB_LED_DATA_PIN 41
    #define WS2812_LEDS
    #define updaterateLED 70 
  #define ausschalten 18000 
  #define weiss_helligkeit 200
  #define KEYPRESS_DETECTION_VALUE 1  
    
    
    #define BUTTON_DISABLED 0
    #define BUTTON_ENABLED 1
    #define timeSeconds 500
    #define updaterate_leds 2000
    #define max_counter 255
    #define min_counter 0
  
  /*//Joystick ESP32-S3 Wroom
  #define X1_PIN 17
  #define Y1_PIN 18
  #define X2_PIN 1
  #define Y2_PIN 2
  */

  //Keyboard PINS ESP32-S3
  #define ROW_NUM     3 
  #define COLUMN_NUM  3 
  #define ROW1 7
  #define ROW2 15
  #define ROW3 16
  #define COL1 4
  #define COL2 5
  #define COL3 6
  
  //Eink KEYBOARD -> ESP32-S3 WROOM
    #define E_INK_PIN_SPI_BUSY 40
    #define E_INK_PIN_SPI_CS   47
    #define E_INK_PIN_SPI_RST  39
    #define E_INK_PIN_SPI_DC   38
    #define E_INK_PIN_SPI_SCK  21
    #define E_INK_PIN_SPI_DIN  9
    #define E_INK_PIN_SPI_MISO 17
    #define Large  5
    #define Small  4
    #define xanpassung -30

  //LSM6 Gyro
    #define GYRO_INT_PIN 8
    #define GYRO_ADRESS_PIN 0x6B

//SD Card
   /*#define SD_CARD_PIN_DAT2 10 // den braucht es offenbar nicht
   #define SD_CARD_PIN_DAT3 11
   #define SD_CARD_PIN_CMD 13
   #define SD_CARD_PIN_CLK 14
   #define SD_CARD_PIN_DAT0 47
   #define SD_CARD_PIN_DAT1 48*/
#endif
