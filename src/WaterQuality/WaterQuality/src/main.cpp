#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>
#include <ezo_rtd_i2c.h>
#include <voltagesensor.h>
// #include <ezo_do_i2c.h>
// #include <ezo_ec_i2c.h>
// #include <ezo_ph_i2c.h>

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

surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;
ezo_rtd_i2c myRTD;
// ezo_ec_i2c myEC;
// ezo_ph_i2c myPH;
// ezo_do_i2c myDO;
voltageSensor mySOLAR(numberOfSensors,pinNumber,sensorname,voltageSenseFactor,min_,max_,unit,numberOfSamples,sampleRead_delay, decimals);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  mysim.begin();
  mylogger.begin();
  #define sleep_ 1000000*60*60
  // myscience.processSensors<ezo_rtd_i2c, ezo_ec_i2c,ezo_ph_i2c,ezo_do_i2c>(myRTD, myEC,myPH, myDO);
   myscience.processSensors(myRTD, mySOLAR);
  // myscience.postData(mysim);
  // myscience.log(mylogger);

  Serial.println("going to sleep");
   
  ESP.deepSleep(sleep_);
}


void loop() {
  // put your main code here, to run repeatedly:

}