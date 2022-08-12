#include <Arduino.h>
#include "Wire.h"
#include "SHT31_SS.h"


SHT31_SS sht31(5);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sht31.begin();
}

void loop()
{
  Serial.println(sht31.getSensorPayload());
  delay(1000);
}


/*
/**
 * @brief While UART connection between ESP32 and PMS sensor is stable.
 * A " read request " is sent to the PMS sensor. The PMS sensor then sends
 * data containing particle measurements, and this gets printed on to the serial terminal monitor.
 * 
 * @param pms selected PMS sensor object
 * @param Serial_PM selected SoftwareSerial port object
 */
/*

String readData(PMS *pms, SoftwareSerial *Serial_PM)
{
  String message;
  PMS::DATA data;
  //Clear buffer (does a burst read to remove potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial_PM->available()) { Serial_PM->read(); }
  Serial.println("Send read request...");
  pms->requestRead();

  Serial.println("Reading data...");
  if (pms->readUntil(data, SAMPLES))
  {

    Serial.println();
    String pm1_0 = String(data.PM_SP_UG_1_0);
    String pm2_5 = String(data.PM_SP_UG_2_5);
    String pm10_0 = String(data.PM_SP_UG_10_0);

    message +="{'sensorName':'PM 1.0("+String(pms->getName())+")','value':"+pm1_0+",'unit':'μg/m³'},"
              +"{'sensorName':'PM 2.5("+String(pms->getName())+")','value':"+pm2_5+",'unit':'μg/m³'},"
              +"{'sensorName':'PM 10.0("+String(pms->getName())+")','value':"+pm2_5+",'unit':'μg/m³'}";
  }
  else
  {
    message = "no data";
  }
  
  return message;
}

*/
