#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>
#include <ezo_rtd_i2c.h>
#include <ezo_do_i2c.h>
#include <ezo_ec_i2c.h>
#include <ezo_ph_i2c.h>


surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;
ezo_rtd_i2c myRTD;
ezo_ec_i2c myEC;
ezo_ph_i2c myPH;
ezo_do_i2c myDO;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  mysim.begin();
  mylogger.begin();
  myRTD.begin();
  // myDO.begin();
  // myPH.begin();
  // myEC.begin();
  
  // myscience.processSensors(myRTD, myDO, myPH, myEC);
   myscience.processSensors(myRTD);
  Serial.println(myRTD.samplesBuffer[0]);
  Serial.println(myRTD.errorBuffer[0]);
  myscience.postData(mysim);
  // myscience.log(mylogger);
}


void loop() {
  // put your main code here, to run repeatedly:

}