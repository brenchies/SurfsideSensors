#include <Wire.h>
#include <Arduino.h>

// SurfSide Science sequencer object
#include <surfsidescience.h>

// Sensors libraries include
#include <voltagesensor.h>
#include "SHT31_S.h"
#include "PMS_SS.h"
#include "PMS_SSS.h"
#include "SPS30_SS.h"

// Communication
#include <tinygsmwrapper.h>

// MCU specific functions please change these if ESP32 is not used
#include "esp_task_wdt.h"

surfSideScience myscience("AIR_QUALITY_01"); // Change module name here if you are re creating anothe module ex: "AIR_QUALITY_02"
#define TINY_GSM_MODEM_SIM7000               // define  TinyGsmClient model
#define TinyGSMDEBUG
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define TINY_GSM_RX_BUFFER 650
TinyGSMWrapper mysim; // tinyGSMwrapper objcet for communication

// SDlogger for logging data in SD card
#include <sdlogger.h>
sdlogger mylogger;

// Voltage sensor attributes for solar panel and battery voltage
int numberOfSensors = 2;
int pinNumber[] = {36, 35};
String sensorname[] = {"SOLAR_VIN", "BATTERY_VIN"};
float voltageSenseFactor[] = {4200/2395.600, 4200/2395.600};
float max_[] = {6000, 5000};
float min_[] = {0, 0};
String unit[] = {"mV", "mV"};
int numberOfSamples = 10;
long sampleRead_delay = 50;
int decimals = 3;

int enablepin = 12; // Enable pin

// SHT31 sensor imput parameter attributes
String sensornameSht[] = {"Temperature", "Humidity"};
String unitSht[] = {"°C", "%"};
int numberOfSamplesSht = 10;
long sampleRead_delaySht = 50;

String sensornamePM1[] = {"PM 1.0(PMS 1)", "PM 2.5(PMS 1)", "PM 10.0(PMS 1)"};
String unitPM[] = {"μg/m3", "μg/m3", "μg/m3"};

String sensornamePM2[] = {"PM 1.0(PMS 2)", "PM 2.5(PMS 2)", "PM 10.0(PMS 2)"};

String sensornameSps30[] = {"PM 1.0(SPS 1)", "PM 2.5(SPS 1)", "PM 10.0(SPS 1)"};
;

// Creating sensor objects
voltageSensor voltageSensors(numberOfSensors, pinNumber, sensorname, voltageSenseFactor, min_, max_, unit, numberOfSamples, sampleRead_delay, decimals);
SHT31_S sht31(enablepin, sensornameSht, unitSht, numberOfSamplesSht, sampleRead_delaySht, decimals);
//SPS30_SS sps30(enablepin, sensornameSps30, unitPM); //(Not implemented yet)
PMS_SS pms1;
PMS_SSS pms2;

// Enable sleepmode cycle
void go_to_sleep(int minutes = 60) // Default time set to 60 minutes.
{
  ESP.deepSleep(1000000 * 60 * minutes);
}

// Enable panic so ESP32 restarts
void enableWDT(int minutes = 10)
{
  esp_task_wdt_init(60 * minutes, true);
  esp_task_wdt_add(NULL); // Add current thread to WDT watch.
}


void disableWDT()
{
  esp_task_wdt_deinit();
}

void setup()
{
  // Enable UART and I2C communication.
  Wire.begin();
  Serial.begin(115200);

  // Call enable watchdog timer
  enableWDT();

  // Call begin methods
  mysim.begin();
  mylogger.begin();
  sht31.begin();
  pms1.begin(33, 32, enablepin, sensornamePM1, unitPM, numberOfSamples, sampleRead_delay);
  pms2.begin(35, 34, enablepin, sensornamePM2, unitPM, numberOfSamples, sampleRead_delay);
  // sps30.begin(&Wire);

  // Pass the sensor, communication and logger objects to the sequencer
  myscience.processSensors(sht31, pms1, pms2); // must go at last
  myscience.postData(mysim);
  myscience.log(mylogger);

  // Disable watchdog timer
  disableWDT();

  // Go to sleep
  go_to_sleep();
}

void loop()
{
}