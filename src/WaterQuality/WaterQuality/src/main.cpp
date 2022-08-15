#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>
#include <voltagesensor.h>
#include "SHT31_S.h"
#include "PMS_SS.h"

//t=7:49pm  vbat: 4.21 //8am vBat=4.15  approx blife==20*12hrs=10days

surfSideScience myscience("AIR_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;


int numberOfSensors = 2;
int pinNumber[] = {36, 35};
String sensorname[] = {"SOLAR_VIN", "BATTERY_VIN"};
float voltageSenseFactor[] = {1,1};
float max_[] = {6000, 5000};
float min_[] = {0,0};
String unit[] = {"mV", "mV"};
int numberOfSamples=10;
long sampleRead_delay=50;
int decimals=3;


int enablepin = 12;

String sensornameSht[] = {"Temperature", "Humidity"}; 
String unitSht[] = {"°C","%"};
int numberOfSamplesSht = 10;
long sampleRead_delaySht = 50;

String sensornamePM1[] = {"PM 1.0(PMS 1)", "PM 2.5(PMS 1)", "PM 10.0(PMS 1)"}; 
String unitPM[] = {"μg/m3","μg/m3","μg/m3"};
int numberOfSamplesPM = 10;
long sampleRead_delayPM = 50;


voltageSensor voltageSensors(numberOfSensors,pinNumber,sensorname,voltageSenseFactor,min_,max_,unit,numberOfSamples,sampleRead_delay, decimals);
SHT31_S sht31(enablepin,sensornameSht,unitSht,numberOfSamplesSht, sampleRead_delaySht,decimals);
PMS_SS pmss;



#include "PMS.h"
#include"SoftwareSerial.h"

SoftwareSerial pmsSerial;
PMS pms1(pmsSerial);
PMS ::DATA data1;
void go_to_sleep(){
  ESP.deepSleep(1000000*60*1);
}
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  mysim.begin();
  mylogger.begin();
  sht31.begin();
  pmss.begin(33, 32, enablepin,sensornamePM1,unitPM,numberOfSamplesPM, sampleRead_delayPM,decimals);
  // myscience.processSensors(voltageSensors);
  myscience.processSensors(pmss, sht31);
  myscience.postData(mysim);
  myscience.log(mylogger);

  Serial.println("going to sleep");
  go_to_sleep();
}


void loop() {

  delay(2000);
}

//  voltage:   4,09
//  Time:     9:22 pm
//  Date:     12 august 2022