#include <Arduino.h>
#include "PMS.h"
#include "SoftwareSerial.h"
#include "StreamDebugger.h"
#include "Wire.h"


#define SAMPLES 5
#define mS_TO_S_FACTOR 1000  /* Conversion factor for micro seconds to seconds */
#define ESP_SLEEP_INTERVAL_IN_MIN(x) (x * 60 * mS_TO_S_FACTOR)   
#define PMS_STARTUP_DELAY_IN_SEC(x) delay((x * mS_TO_S_FACTOR))  

//#define Serial Serial //For debugging on serial monitor
const int RESET = 13;

#define PMS1
#define PMS2

#ifdef PMS1
//Set ESP32 pins to receive/transfer data
#define TX1 32 //GPIO32
#define RX1 33 //GPIO33
//Creation of espSoftwareSerial objects
SoftwareSerial Serial_PM1;
//PM Sensor names
String name_pms1 = "PMS 1";
//Creation of PM sensor objects
PMS pms1(Serial_PM1, &name_pms1);
#endif


#ifdef PMS2 //Uncomment PMS2 if you are using an second PMS sensor
//Set ESP32 pins to receive/transfer data
#define TX2 34 //GPIO34
#define RX2 35 //GPIO35
//Creation of espSoftwareSerial objects
SoftwareSerial Serial_PM2;
//PM Sensor names
String name_pms2 = "PMS 2";
//Creation of PM sensor objects
PMS pms2(Serial_PM2, &name_pms2);
#endif

String readData(PMS*, SoftwareSerial*);


#include "SHT31.h"

#define SHT31_ADDRESS   0x44

uint32_t start;
uint32_t stop;

SHT31 sht;

String ReadSHT31_data(SHT31 *sht31, uint32_t *start, uint32_t *stop);


#include "tinygsmwrapper.h"
#define SENSOR_SAMPLES 10
#define MONITORID "AIR_QUALITY_01"
//voltage sensors
#define HAS_SOLAR_VSENSE
#define HAS_BATTERY_VSENSE
#define ANALOG_VREF 3300.0 //mV
#define ANALOG_RESOLUTION 4096.0
#define SOLAR_VSENSE_PIN 36
#define SOLAR_VSENSE_FACTOR ANALOG_VREF / ANALOG_RESOLUTION * 3300.0 / 1508.20
#define BATTERY_VSENSE_PIN 35
#define BATTERY_VSENSE_FACTOR ANALOG_VREF / ANALOG_RESOLUTION * 3300.0 / 1525.93
//#define LOW_BATTERY_SHUTDOWN
#define LOW_BATTERY_THRESHOLD 3300.0 //mV

//define sensor enable 
#define HAS_SENSOR_ENABLE
#ifdef HAS_SENSOR_ENABLE
  #define SENSOR_ENABLE_PIN 13
  #define SENSOR_STABILIZE_PERIOD_US 30000
#endif

// battery
#ifdef BATTERY_VSENSE_PIN
  float getBatteryVoltage(float samples=10){
    float voltage = 0.0;
    for (int i = 0; i < samples; i++){
      voltage += analogRead(BATTERY_VSENSE_PIN);
      delay(10);
    }
    voltage = voltage * BATTERY_VSENSE_FACTOR / samples;
    return voltage;
  }
#endif

// solar
#ifdef SOLAR_VSENSE_PIN
  float getSolarVoltage(float samples=10){
    float voltage = 0.0;
    for (int i = 0; i < samples; i++){
      voltage += analogRead(SOLAR_VSENSE_PIN);
      delay(10);
    }
    voltage = voltage * SOLAR_VSENSE_FACTOR / samples;
    return voltage;
  }
#endif


// GPRS RSSI
#define REPORT_GSM_RSSI

//sdcard
#define HAS_SDCARD
#ifdef HAS_SDCARD
  #include <SPI.h>
  #include <SD.h>
  #define SD_MISO     2
  #define SD_MOSI     15
  #define SD_SCLK     14
  #define SD_CS       13
  #define LED_PIN     12
  bool SD_CARD_MOUNTED = false;
  String FileName = String(MONITORID)+".txt";
  File myFile;

  int writeToSD(String data){
    if (SD_CARD_MOUNTED){
      Serial.println("writing to sd card");
      myFile = SD.open("/"+String(MONITORID)+"_SAMPLES.txt", FILE_APPEND);
      myFile.println(data);
      myFile.close();
    }else{
      return -1;
    }
    return 1;
  }
  int writeToSDTemp(String data){
    if (SD_CARD_MOUNTED){
      Serial.println("writing to sd card");
      myFile = SD.open("/"+String(MONITORID)+"_TEMP.txt", FILE_APPEND);
      myFile.println(data);
      myFile.close();
    }else{
      return -1;
    }
    return 1;
  }
    int readFromSDTemp(String data, bool delLine){
      // delete line and return temp data line if available;
    if (SD_CARD_MOUNTED){
      Serial.println("writing to sd card");
      myFile = SD.open("/"+String(MONITORID)+"_TEMP.txt", FILE_APPEND);
      myFile.println(data);
      myFile.close();
    }else{
      return -1;
    }
    return 1;
  }

  int writeToSDlog(String data){
      // delete line and return temp data line if available;
    if (SD_CARD_MOUNTED){
      Serial.println("writing to sd card");
      myFile = SD.open("/"+String(MONITORID)+"_LOG.txt", FILE_APPEND);
      myFile.println(data);
      myFile.close();
    }else{
      return -1;
    }
    return 1;
  }
#endif


TinyGSMWrapper mysim("www.google.com", "/", "web.digicelaruba.com", "","", 115200, 25, 27, 26, 4, 80);

#ifdef ESP32
void SET_TO_SLEEP(uint32_t time_us){
}
#endif



void doSleepCycle(uint32_t time_us = 1000000 * 5)
{// close all communication busses before sleeping
  SPI.end();
  mysim.modemPowerOff();
  Serial.end();
  Serial1.end();
  #ifdef HAS_SENSOR_ENABLE
    digitalWrite(SENSOR_ENABLE_PIN, LOW);
  #endif
  // SET_TO_SLEEP(time_us);
  ESP.deepSleep(time_us);
}


void setup()
{
  sht.begin(SHT31_ADDRESS);

Serial.begin(115200);
  // sensor enable power
#ifdef HAS_SENSOR_ENABLE
  pinMode(SENSOR_ENABLE_PIN, OUTPUT);
  digitalWrite(SENSOR_ENABLE_PIN, LOW);
#endif

// has sdcard
#ifdef HAS_SDCARD
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS)) {
        Serial.println("SDCard MOUNT FAIL");
    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        String str = "SDCard Size: " + String(cardSize) + "MB";
        Serial.println(str);
        SD_CARD_MOUNTED = true;
    }
#endif

// voltage sensors
#ifdef HAS_SOLAR_VSENSE
  pinMode(SOLAR_VSENSE_PIN, INPUT);
#endif

#ifdef HAS_BATTERY_VSENSE
  pinMode(BATTERY_VSENSE_PIN, INPUT);
  #ifdef LOW_BATTERY_SHUTDOWN
    float voltage = getBatteryVoltage();
    if(voltage <= LOW_BATTERY_THRESHOLD){
      String err = "SHUTDOWN LOW BATTERY VOLTAGE: "+String(voltage)+" Solar Voltage: "+String(getSolarVoltage());
      Serial.println(err);
      writeToSDlog(err);
      doSleepCycle();
    }
  #endif
#endif

  String measurements;
  Serial.println("plug in sensors");
  delay(30000);
  Serial.println("Initializing...");

  #ifdef PMS1
  Serial_PM1.begin(9600, SWSERIAL_8N1, RX1, TX1, false, 192);
  #endif

  #ifdef PMS2
  Serial_PM2.begin(9600, SWSERIAL_8N1, RX2, TX2, false, 192);
  #endif
  
  #ifdef PMS1
  //Switch to passive mode.
  pms1.passiveMode();
  pms1.wakeUp();
  // PMS_STARTUP_DELAY_IN_SEC(30);
  #endif
 
  #ifdef PMS2
  pms2.passiveMode();
  pms2.wakeUp();
  #endif

//Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
//Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
 
}

void loop()
{

  #ifdef HAS_SENSOR_ENABLE
    digitalWrite(SENSOR_ENABLE_PIN, HIGH);
    delay(SENSOR_STABILIZE_PERIOD_US);
  #endif
  String data;
  uint32_t timer = millis();
  int err = 0;
  String data1 = readData(&pms1, &Serial_PM1);
  String data2 = readData(&pms2, &Serial_PM2);
  String data3 = ReadSHT31_data(&sht, &start, &stop);
  Serial.println("Sensor read delay: "+String(millis() - timer));
  #ifdef HAS_SENSOR_ENABLE
    digitalWrite(SENSOR_ENABLE_PIN, LOW);
  #endif
  if (err != 0){
    //report_error();
  }
    String datetime;
    timer = millis();
    err = mysim.begingprs();
    Serial.println("gprs connection delay: "+String(millis() - timer));
    err = mysim.getTime(&datetime, 0, 0, 0);
    String payload = "{";
    payload += "'deviceName':'"+String(MONITORID)+"',";
    payload += "'timestamp':'"+datetime+"',";
    payload += "'sensors':[";
    payload += data1;
    payload += ",";
    payload += data2;
    payload += ",";
    payload += data3;
    payload += "]}";
    payload.replace("'", String('"'));
    err = mysim.post("surfside-db.brenchies.com",80,"/observations", payload,"application/json");
    
    Serial.println("http return code: "+String(err));
    mysim.gprsDisconnect();
    Serial.println(payload);

  #ifdef HAS_SDCARD
    writeToSD(payload);
    if (err == -1){
      writeToSDTemp(payload);
      writeToSDlog("ERROR http request returned code: "+String(err));
    }
  #endif
 doSleepCycle();
}

/**
 * @brief While UART connection between ESP32 and PMS sensor is stable.
 * A " read request " is sent to the PMS sensor. The PMS sensor then sends
 * data containing particle measurements, and this gets printed on to the serial terminal monitor.
 * 
 * @param pms selected PMS sensor object
 * @param Serial_PM selected SoftwareSerial port object
 */
String readData(PMS *pms, SoftwareSerial *Serial_PM)
{
  String message;
  PMS::DATA data;
  //Clear buffer (does a burst read to remove potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial_PM->available()) { Serial_PM->read(); }
  Serial.println("Send read request...");
  pms->requestRead();

  Serial.println("Reading data...");
  if (pms->readUntil(data, SAMPLES))
  {

    Serial.println();
    String pm1_0 = String(data.PM_SP_UG_1_0);
    String pm2_5 = String(data.PM_SP_UG_2_5);
    String pm10_0 = String(data.PM_SP_UG_10_0);

    message +="{'sensorName':'PM 1.0("+String(pms->getName())+")','value':"+pm1_0+",'unit':'μg/m³'},"
              +"{'sensorName':'PM 2.5("+String(pms->getName())+")','value':"+pm2_5+",'unit':'μg/m³'},"
              +"{'sensorName':'PM 10.0("+String(pms->getName())+")','value':"+pm2_5+",'unit':'μg/m³'}";
  }
  else
  {
    message = "no data";
  }
  
  return message;
}

String ReadSHT31_data(SHT31 *sht31, uint32_t *start, uint32_t *stop)
{
  String message;
  if (sht31->isConnected())
  {
    *start = micros();
    bool b = sht31->read();         // default = true/fast       slow = false
    *stop = micros();
    message +="{'sensorName':'Temperature','value':"+String(sht31->getTemperature())+",'unit':'°C'},"
             +"{'sensorName':'Humidity','value':"+String(sht31->getHumidity())+",'unit':'%'}";
  }
  else
  {
    message ="Not connected";
  }
  return message;
}