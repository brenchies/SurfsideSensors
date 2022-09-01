#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>

#define TINY_GSM_MODEM_SIM7000      // define  TinyGsmClient model
#define TinyGSMDEBUG
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define TINY_GSM_RX_BUFFER 650

//Include libraries 
#include <sdlogger.h>
#include <tinygsmwrapper.h>
#include <voltagesensor.h>
#include "esp_task_wdt.h"
#include "SHT31_S.h"
#include "PMS_SS.h"
#include "PMS_SSS.h"
//#include "SPS30_SS.h" (Open for future implementation)

surfSideScience myscience("AIR_QUALITY_01"); // Change module name here if you are re creating anothe module ex: "AIR_QUALITY_02"
TinyGSMWrapper mysim; // tinyGSMwrapper objcet for communication
sdlogger mylogger;// SDlogger for logging data in SD card


int enablepin = 12; // Enable pin

// Voltage sensor attributes for solar panel and battery voltage
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

// SHT31 sensor imput parameter attributes
String sensornameSht[] = {"Temperature", "Humidity"};
String unitSht[] = {"°C", "%"};
long sampleRead_delaySht = 50;

//PMS5003 sensor imput parameter attributes
String sensornamePM1[] = {"PM 1.0(PMS 1)", "PM 2.5(PMS 1)", "PM 10.0(PMS 1)"};
String unitPM[] = {"μg/m3", "μg/m3", "μg/m3"};
String sensornamePM2[] = {"PM 1.0(PMS 2)", "PM 2.5(PMS 2)", "PM 10.0(PMS 2)"};
String sensornameSps30[] = {"PM 1.0(SPS 1)", "PM 2.5(SPS 1)", "PM 10.0(SPS 1)"};

// Creating sensor objects
voltageSensor voltageSensors(numberOfSensors, pinNumber, sensorname, voltageSenseFactor, min_, max_, unit, numberOfSamples, sampleRead_delay, decimals);
SHT31_S sht31(enablepin, sensornameSht, unitSht, numberOfSamples,sampleRead_delaySht, decimals);
PMS_SS pms1;
PMS_SSS pms2;
//SPS30_SS sps30(enablepin, sensornameSps30, unitPM); //(Open for future implementation)

void go_to_sleep(int minutes);
void enableWDT(int minutes);
void disableWDT();

void setup()
{
  // Enable UART and I2C communication.
  Wire.begin();
  Serial.begin(115200);

  // Call enable watchdog timer
  enableWDT(10);

  // Call begin methods
  mysim.begin();
  mylogger.begin();
  sht31.begin();
  pms1.begin(33, 32, enablepin, sensornamePM1, unitPM, numberOfSamples, sampleRead_delay);
  pms2.begin(35, 34, enablepin, sensornamePM2, unitPM, numberOfSamples, sampleRead_delay);
  // sps30.begin(&Wire);

  // Pass the sensor, communication and logger objects to the sequencer
  myscience.processSensors(sht31, pms1, pms2, voltageSensors); // must go at last
  myscience.postData(mysim);
  myscience.log(mylogger);

  // Disable watchdog timer
  disableWDT();

  // Go to sleep
  go_to_sleep(60);
}

void loop()
{
}


/**
 * @brief Enable sleepmode cycle
 * 
 * @param minutes Input sleep cycle in minutes.
 */
void go_to_sleep(int minutes)
{
  ESP.deepSleep(1000000 * 60 * minutes);
}


/**
 * @brief Enable Watch Dog Timer so ESP32 restarts.
 * 
 * @param minutes Input timer in minutes.
 */
void enableWDT(int minutes)
{
  esp_task_wdt_init(60 * minutes, true);
  esp_task_wdt_add(NULL); // Add current thread to WDT watch.
}

/**
 * @brief Disable Watch Dog Timer.
 * 
 */
void disableWDT()
{
  esp_task_wdt_deinit();
}