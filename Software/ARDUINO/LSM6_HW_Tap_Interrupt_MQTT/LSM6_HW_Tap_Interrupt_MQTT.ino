#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "SparkFunLSM6DS3.h"
#include "Wire.h"
//https://www.st.com/resource/en/application_note/an5040-lsm6dsl-alwayson-3d-accelerometer-and-3d-gyroscope-stmicroelectronics.pdf


    double roll = 0.00, pitch = 0.00,  yaw = 0.00;     //Roll & Pitch are the angles which rotate by the axis X and y
    long lastTrigger_SENSOR=0;
    long lastTrigger_DISPLAY=0;
    long elapsedTime = 0;
    #define UPDATERATE_SENSOR 5000
    #define UPDATERATE_DISPLAY 3000

const char* ssid = "ChangeME";
const char* password = "changeME";
const char* mqtt_server = "192.168.178.113";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//Wallswitch config
/*LSM6DS3 myIMU( I2C_MODE, 0x6A );
#define SDA2 42
#define SCL2 2
#define INT1_PIN 8
*/
//baseboard config
LSM6DS3 myIMU( I2C_MODE, 0x6B );
#define SDA2 21
#define SCL2 22
#define INT1_PIN 12


uint8_t int1Status;


void IRAM_ATTR int1Handler() {
    int1Status++;
}

void setup()
{
    
    Serial.begin(115200);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector
    delay(1000); //relax...
    pinMode(5, OUTPUT);
    delay(200); //relax...
    digitalWrite(5, HIGH); 
    Serial.println("Processor started.\n");
  	
    //MQTT setup
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    Wire.begin(SDA2, SCL2);


    //Call .beginCore() to configure the IMU
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
    dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_50Hz;//LSM6DS3_ACC_GYRO_BW_XL_200Hz;
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



bool checkForDoubleTap2()
{
    
    uint8_t reg;
    myIMU.readRegister(&reg, LSM6DS3_ACC_GYRO_TAP_SRC);

    

      if (((reg & 0x40) == 0x40) && ((reg & 0x10) == 0x10)) {     // double tap
        Serial.print("double tap ");
        Serial.println(millis());
        Serial.print("Reg: ");Serial.print(reg);
        return true;
      }
      else{
        Serial.print("single tap ");
        Serial.println(millis());
        Serial.print("Reg: ");Serial.print(reg);
        return true;
        
      }
    
    return false;
}

void loop()
{
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

    
  unsigned long now = millis();
  if (int1Status > 0) {
        checkForDoubleTap2();
        int1Status = 0;
  }
  if(now - lastTrigger_SENSOR > (UPDATERATE_SENSOR)){
      check_gyro_acc();
      elapsedTime = now - lastTrigger_SENSOR;
      lastTrigger_SENSOR = millis();  
      char rollString[8];
      dtostrf(roll, 1, 2, rollString);
      client.publish("esp32/roll", rollString);
      
      char pitchString[8];
      dtostrf(pitch, 1, 2, pitchString);
      client.publish("esp32/pitch", pitchString);
   } 
}

void check_gyro_acc(void){
  //int16_t temp;
  /*//Get all parameters
  Serial.print("ACC X:");
  myIMU.readRegisterInt16(&temp, LSM6DS3_ACC_GYRO_OUTX_L_XL);
  Serial.print(temp); 
  
  Serial.print(",ACC Y:");
  myIMU.readRegisterInt16(&temp, LSM6DS3_ACC_GYRO_OUTY_L_XL);
  Serial.print(temp); 
  
  Serial.print(",ACC Z:");
  myIMU.readRegisterInt16(&temp, LSM6DS3_ACC_GYRO_OUTZ_L_XL);*/
  //Serial.print(temp);
/*
  Serial.print("X-GYRO:");
  myIMU.readRegisterInt16(&temp, LSM6DS3_ACC_GYRO_OUTX_L_G);
  Serial.print(temp); 

  Serial.print(",Y-GYRO:");
  myIMU.readRegisterInt16(&temp, LSM6DS3_ACC_GYRO_OUTY_L_G);
  Serial.print(temp); 

  Serial.print(",Z-GYRO:");
  myIMU.readRegisterInt16(&temp, LSM6DS3_ACC_GYRO_OUTZ_L_G);
  Serial.print(temp); 
  */
  Serial.print("ACCX: ");
  Serial.print(myIMU.readFloatAccelX(), 4);
  Serial.print(",ACCY: ");
  Serial.print(myIMU.readFloatAccelY(), 4);
  Serial.print(",ACCZ: ");
  Serial.println(myIMU.readFloatAccelZ(), 4);
  float ACCX = myIMU.readFloatAccelX();
  float ACCY = myIMU.readFloatAccelY();
  float ACCZ = myIMU.readFloatAccelZ();
  double x_Buff = float(ACCX);
  double y_Buff = float(ACCY);
  double z_Buff = float(ACCZ);
  
  

 /* Serial.print("GyroX:");
  Serial.print(myIMU.readFloatGyroX(), 4);
  Serial.print(",GyroY:");
  Serial.print(myIMU.readFloatGyroY(), 4);
  Serial.print(",GyroZ:");
  Serial.println(myIMU.readFloatGyroZ(), 4);*/

  float Gyrox = myIMU.readFloatGyroX();
  float Gyroy = myIMU.readFloatGyroY();
  float Gyroz = myIMU.readFloatGyroZ();
  Serial.print("\t\tGyro X: ");
  Serial.print((Gyrox * 57.2958), 2);
  Serial.print(" \tY: ");
  Serial.print((Gyroy * 57.2958), 2);
  Serial.print(" \tZ: ");
  Serial.print((Gyroz * 57.2958), 2);
  Serial.println(" GRAD ");
  Serial.println();
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
  //yaw =  yaw + Gyroz * elapsedTime;//https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
  elapsedTime = 0;
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(" Pitch: "); Serial.print(pitch);
  //Serial.print(" Yaw: "); Serial.println(yaw);
  
  /*
  Serial.print("GyroX:");
  Serial.print(myIMU.readRawGyroX());
  Serial.print(",GyroY:");
  Serial.print(myIMU.readRawGyroY());
  Serial.print(",GyroZ:");
  Serial.print(myIMU.readRawGyroZ());*/
  

  Serial.print(",Thermometer:");
  Serial.println(myIMU.readTempC(), 2);
  //Serial.print(",Thermometer RAW:");
  //Serial.println(myIMU.readRawTemp(), 2);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      //digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}