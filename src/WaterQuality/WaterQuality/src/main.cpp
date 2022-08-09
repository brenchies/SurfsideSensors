#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>
#include <ezo_rtd_i2c.h>


surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;
ezo_rtd_i2c myRTD;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  mysim.begin();
  mylogger.begin();
  myRTD.begin();
  myscience.processSensors(myRTD, myRTD);
  myscience.postData(mysim);
  myscience.log(mylogger);
}


void loop() {
  // put your main code here, to run repeatedly:

}