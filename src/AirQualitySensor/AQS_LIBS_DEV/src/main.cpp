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

