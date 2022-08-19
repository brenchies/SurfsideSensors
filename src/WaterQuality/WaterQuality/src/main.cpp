#include <Wire.h>
#include <Arduino.h>
#include <surfsidescience.h>
#include <tinygsmwrapper.h>
#include <sdlogger.h>
#include <ezo_rtd_i2c.h>
#include <voltagesensor.h>
#include <ezo_do_i2c.h>
#include <ezo_ec_i2c.h>
#include <ezo_ph_i2c.h>
#include <esp_task_wdt.h>

//t=7:49pm  vbat: 4.21 //8am vBat=4.15  approx blife==20*12hrs=10days
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

surfSideScience myscience("WATER_QUALITY_01");
TinyGSMWrapper mysim;
sdlogger mylogger;
ezo_rtd_i2c myRTD;
ezo_ec_i2c myEC;
ezo_ph_i2c myPH;
ezo_do_i2c myDO;
voltageSensor voltageSensors(numberOfSensors,pinNumber,sensorname,voltageSenseFactor,min_,max_,unit,numberOfSamples,sampleRead_delay, decimals);


void go_to_sleep(){
  Serial.println("Sleeping for: 3600 seconds");
  ESP.deepSleep(1000000*60*60);
}
void setup() {
  // pinMode(32, OUTPUT);
  // digitalWrite(32, HIGH);
  
  uint32_t timer1, timer2, timer3;
  Wire.begin();
  Serial.begin(115200);

  esp_task_wdt_init(60*10, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
  Serial.println("WDT enabled timeoute: "+String(60*10)+" s");


  mysim.begin();
  mylogger.begin();
  timer2 = millis();
  myscience.processSensors(voltageSensors, myEC,myDO,myPH, myRTD);
  timer2 = millis() - timer2;
  timer3 = millis();
  myscience.postData(mysim);
  myscience.log(mylogger);
  timer3 = millis() - timer3;

  Serial.println("going to sleep");
  mylogger.writeToSD("timer1: "+String(timer1)+" timer2: "+String(timer2)+" timer3: "+String(timer3), "timerOn.txt");
  esp_task_wdt_deinit();
  go_to_sleep();
}


void loop() {
  // put your main code here, to run repeatedly:

}