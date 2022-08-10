#include "SHT31_SSS.h"

void SHT31_SSS::enableSensors()
{
    _measurements[TEMPERRATURE].name = "Temperature";
    _measurements[TEMPERRATURE].unit = "°C";
    _measurements[HUMIDITY].name = "Humidity";
    _measurements[HUMIDITY].unit = "%";
}

// void SHT31_SSS::getSensorReadings()
// {

// }