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
// String databasePayload(SHT31_SSS *sht31);
// String sensorErrorProtocol(String name, String errorType);
// String sensorProtocol(String name, String value, String unit);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sht31.begin();
}

void loop()
{
 sht31.getValue();
 
  Serial.println(sht31.getData());
  delay(2000);
}


// String databasePayload(SHT31_SSS *sht31)
// {
//   int max = sht31->getNumberOfMeasurements();
//   String sensorData = ",";

//   for (int i = 0; i > max; i++)
//   {
//     switch (sht31->getAvgSensorReadings())
//     {
//     case SUCCESS_CONNECTED: //"{'sensorName':'" + name + "','value':" + val + ",'unit':'" + unit + "'}";
//       if (i < max)
//       {
//         if (sht31->getSensors().data[i]->status == String(ERROR_VALUE_OUT_OF_RANGE))
//         {
//           sensorData += sensorErrorProtocol(sht31->getSensors().data[i]->name, sht31->getSensors().data[i]->status) + ",";
//         }
//         else
//         {
//           sensorData += sensorProtocol(sht31->getSensors().data[i]->name, String(sht31->getSensors().data[i]->value), sht31->getSensors().data[i]->unit) + ",";
//         }
//       }
//       else if (i == max)
//       {
//         if (sht31->getSensors().data[i]->status == String(ERROR_VALUE_OUT_OF_RANGE))
//         {
//           sensorData += sensorErrorProtocol(sht31->getSensors().data[i]->name, sht31->getSensors().data[i]->status);
//         }
//         else
//         {
//           sensorData += sensorProtocol(sht31->getSensors().data[i]->name, String(sht31->getSensors().data[i]->value), sht31->getSensors().data[i]->unit);
//         }
//       }
//       break;
//     case ERROR_CONNECTION:
//       if (i < max)
//       {
//         sensorData += sensorErrorProtocol(sht31->getSensors().data[i]->name, sht31->getSensors().data[i]->status) + ",";
//       }
//       else if (i == max)
//       {
//         sensorData += sensorErrorProtocol(sht31->getSensors().data[i]->name, sht31->getSensors().data[i]->status);
//       }
//       break;
//     }
//   }

//   return sensorData;
// }

// String sensorProtocol(String name, String value, String unit)
// {
//   return "{'sensorName':'" + name + "'," +
//          "  'value':" + value + "," +
//          "  'unit':'" + unit + "'" +
//          "}";
// }

// String sensorErrorProtocol(String name, String errorType)
// {
//   return "{'sensorName':'" + name + "'," +
//          "  'error':" + errorType + "," +
//          "}";
// }

// // -- END OF FILE --
