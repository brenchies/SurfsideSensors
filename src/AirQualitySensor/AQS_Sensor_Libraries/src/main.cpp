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
  switch (sht31.getAvgSensorReadings())
  {
  case SUCCESS:
    Serial.println()
    break;
  
  default err:
    break;
  }
  Serial.println(data);
  delay(2000);
}

String databasePayload(String name, String unit, String val)
{
 for(int i = 0; i)
}

void sampleSensor(sensorType sensor){
        float *data;
        sensor.getSamples();
        int numberOfreadings = sensor.numberOfreadings;
        for (int i = 0; i < numberOfreadings; i++){
            if(sensor.sensorStatus[i] != SUCCESS){
                processErrorBuffer("{sensorName: "+sensor.sensorName[i]+","+sensor.errorBuffer[i]+"}");
                sensor.errorBuffer[i] = "";
            }else{
                if(sensorsData.length() > 0){sensorsData += ",";}
                sensorsData += "{'sensorName':'"+sensor.sensorName[i]+"','value':"+sensor.samplesBuffer[i]+",'unit':'"+sensor.units[i]+"'}";
            }
        }
    }


// -- END OF FILE --

