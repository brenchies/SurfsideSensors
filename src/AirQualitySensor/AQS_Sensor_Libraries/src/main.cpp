//
//    FILE: SHT31_isConnected.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo  (needs 0.2.4 or up)
//     URL: https://github.com/RobTillaart/SHT31

#include <Arduino.h>
#include "Wire.h"
#include "SHT31.h"
#include "SHT31_SSS.h"

#define SHT31_ADDRESS 0x44

SHT31_SSS sht31(5);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sht31.begin();
}

void loop()
{
  String data;
  sht31.enableSensors();
  data = sht31.getSensorSamples();
  Serial.println(data);
  delay(2000);
}


// -- END OF FILE --

