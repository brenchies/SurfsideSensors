#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>

surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mysim.begin();
  mylogger.begin();
  myscience.postData(mysim);
  myscience.log(mylogger);
}


void loop() {
  // put your main code here, to run repeatedly:

}