#include <Wire.h>
#include <Arduino.h>

//surfside science sequencer object
#include <surfsidescience.h>
surfSideScience myscience("WATER_QUALITY_01");

//tinyGSMwrapper objcet for communication
#define TINY_GSM_MODEM_SIM7000 // define  TinyGsmClient model
#define TinyGSMDEBUG
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define TINY_GSM_RX_BUFFER 650
#include <tinygsmwrapper.h>
const char* apn="web.digicelaruba.com";
const char* gprsuser="";
const char* gprspass="";
const char* server="surfside-db.brenchies.com";
const char* postPath="/observations";
const char* contentType= "application/json";
String devicename="SIMCom SIM7000";
long successCode=201;
long uart_baud=115200;
long pin_dtr=25;
long pin_tx = 27;
long pin_rx=26;
long pin_pwr=4;
TinyGSMWrapper mysim;

// SDlogger for sd data log
#include <sdlogger.h>
sdlogger mylogger;

//voltage sensor object for solar pannel and battery voltage
#include <voltagesensor.h>
int numberOfSensors = 2;
int pinNumber[] = {36, 35};
String sensorname[] = {"SOLAR_VIN", "BATTERY_VIN"};
float voltageSenseFactor[] = {4200/2395.600, 4200/2395.600};
float max_[] = {6000, 5000};
float min_[] = {0,0};
String unit[] = {"mV", "mV"};
int numberOfSamples=10;
long sampleRead_delay=50;
int decimals=3;
voltageSensor voltageSensors(numberOfSensors,pinNumber,sensorname,voltageSenseFactor,min_,max_,unit,numberOfSamples,sampleRead_delay, decimals);

//ESO_RTD sensor
#include <ezo_rtd_i2c.h>
ezo_rtd_i2c myRTD;

//EZO_EC sensor
#include <ezo_ec_i2c.h>
ezo_ec_i2c myEC;

//EZO_PH sensor
#include <ezo_ph_i2c.h>
ezo_ph_i2c myPH;

//EZO_DO sensor
#include <ezo_do_i2c.h>
ezo_do_i2c myDO;

//MCU specific functions please change these if ESP32 is not used
#include <esp_task_wdt.h>
void go_to_sleep(int minutes=60){
  ESP.deepSleep(1000000*60*minutes);
}
void enableWDT(){
    esp_task_wdt_init(60*10, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
}

void disableWDT(){
    esp_task_wdt_deinit();
}

void setup() {
    // enable serial communication
    Wire.begin();
    Serial.begin(115200);

    // enable watchdog timer
    enableWDT();

    // call begin methods
    mysim.begin(apn, gprsuser, gprspass, server, postPath, successCode, contentType, uart_baud, pin_dtr, pin_tx, pin_rx, pin_pwr, devicename);
    mylogger.begin();

    //pass the sensor, communication and logger objects to the sequencer
    myscience.processSensors(voltageSensors, myEC,myDO,myPH, myRTD);
    myscience.postData(mysim);
    myscience.log(mylogger);

    // disable watchdog timer
    disableWDT();

    // go to sleep
    go_to_sleep();
}


void loop() {

}