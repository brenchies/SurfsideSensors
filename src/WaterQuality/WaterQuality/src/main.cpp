#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>
#include <voltagesensor.h>
#include "SHT31_S.h"

//t=7:49pm  vbat: 4.21 //8am vBat=4.15  approx blife==20*12hrs=10days
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


int enablepin = 13;
String sensorname2[] = {"Temperature", "Humidity"}; 
String unit2[] = {"°C","%"};
int numberOfSamples2 = 10;
long sampleRead_delay2 = 50;
surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;
voltageSensor voltageSensors(numberOfSensors,pinNumber,sensorname,voltageSenseFactor,min_,max_,unit,numberOfSamples,sampleRead_delay, decimals);
SHT31_S sht31(enablepin,sensorname2,unit,numberOfSamples2, sampleRead_delay2,decimals);

void go_to_sleep(){
  ESP.deepSleep(1000000*60*60);
}
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  mysim.begin();
  mylogger.begin();
  sht31.begin();
  // sht31.getSamples();
  myscience.processSensors(voltageSensors, sht31);
  myscience.postData(mysim);
  myscience.log(mylogger);

  Serial.println("going to sleep");
   
  go_to_sleep();
}


void loop() {
  // put your main code here, to run repeatedly:

}