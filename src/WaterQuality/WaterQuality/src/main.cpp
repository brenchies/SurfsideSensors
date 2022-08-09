#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>

surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim("www.google.com", "/", "web.digicelaruba.com", "","", 115200, 25, 27, 26, 4, 80);

void setup() {
  // put your setup code here, to run once:
  myscience.uploadData<TinyGSMWrapper>(mysim);
}


void loop() {
  // put your main code here, to run repeatedly:


}