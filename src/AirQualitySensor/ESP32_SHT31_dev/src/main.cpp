#include<Arduino.h>
#include "Wire.h"
#include "SHT31.h"

#define SHT31_ADDRESS   0x44

uint32_t start;
uint32_t stop;

SHT31 sht;

String ReadSHT31_data(SHT31 *sht31, uint32_t *start, uint32_t *stop);

void setup()
{
  Serial.begin(115200);

  Wire.begin();
  sht.begin(SHT31_ADDRESS);
}


void loop()
{
  String data3 = ReadSHT31_data(&sht, &start, &stop);

  Serial.println(data3);
  delay(1000);
}

String ReadSHT31_data(SHT31 *sht31, uint32_t *start, uint32_t *stop)
{
  String message;
  if (sht31->isConnected())
  {
    *start = micros();
    bool b = sht31->read();         // default = true/fast       slow = false
    *stop = micros();
    message +="{'sensorName':'Temperature','value':"+String(sht31->getTemperature())+",'unit':'°C'},"
             +"{'sensorName':'Humiditty','value':"+String(sht31->getHumidity())+",'unit':'%'}";
  }
  else
  {
    message ="Not connected";
  }
  return message;
}
