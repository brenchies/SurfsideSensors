//
//    FILE: SHT31_isConnected.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo  (needs 0.2.4 or up)
//     URL: https://github.com/RobTillaart/SHT31

#include <Arduino.h>
#include "Wire.h"
#include "SHT31.h"

#define SHT31_ADDRESS   0x44

uint32_t start;
uint32_t stop;

SHT31 sht;


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sht.begin(SHT31_ADDRESS);

}


void loop()
{
  if ( sht.isConnected()  )
  {
    sht.read();         // default = true/fast       slow = false

    Serial.println(sht.getTemperature());
    Serial.println(sht.getHumidity());
  }
  else
  {
    Serial.println("\tNot connected:\t");
    // sht.reset();
  }
  delay(1000);
}


// -- END OF FILE --

