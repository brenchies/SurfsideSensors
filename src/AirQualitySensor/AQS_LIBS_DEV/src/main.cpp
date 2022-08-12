#include <Arduino.h>

//
//    FILE: SHT31_isConnected.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo  (needs 0.2.4 or up)
//     URL: https://github.com/RobTillaart/SHT31

#include "Wire.h"
#include "SHT31_SS.h"

#define SHT31_ADDRESS 0x44
uint32_t start;
uint32_t stop;

SHT31_SS sht31(5);
uint32_t connectionFails = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sht31.begin();
}

void loop()
{
  // sht31.enableSensors(3);
  // Serial.println("Temp:" + String(sht31.processAvgSensorMeasurements().data[TEMPERRATURE].value));
  // Serial.println();
  // Serial.println("Hum:" + String(sht31.getSensor().data[HUMIDITY].value));

  Serial.println(sht31.getSensorPayload());
  delay(1000);
}



// -- END OF FILE --
