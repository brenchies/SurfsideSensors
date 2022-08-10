#include "SHT31_SSS.h"

SHT31_SSS::SHT31_SSS(int total_oversamples)
{
     SHT31 sht31;
     this->_sht31 = &sht31;
    _numberOfMeasurements = NUM_MEASUREMENTS;
    _total_oversamples = total_oversamples;

    _measurements[TEMPERRATURE].name = "Temperature";
    _measurements[TEMPERRATURE].unit = "°C";
    _measurements[HUMIDITY].name = "Humidity";
    _measurements[HUMIDITY].unit = "%";
}

void SHT31_SSS:: begin()
{
    this->_sht31->begin(SHT_DEFAULT_ADDRESS);
}
void SHT31_SSS::enableSensors()
{
    _measurements[TEMPERRATURE].name = "Temperature";
    _measurements[TEMPERRATURE].unit = "°C";
    _measurements[HUMIDITY].name = "Humidity";
    _measurements[HUMIDITY].unit = "%";
}

bool SHT31_SSS::valueInRange(float val, float min, float max)
{
    return ((val-min) <= (max-min));
}

struct SampleMeasurement SHT31_SSS::getSensorSamples()
{
    struct SampleMeasurement sample;

    if (this->_sht31->isConnected())
    {
        float temp, hum;

        this->_sht31->read(); // default = true/fast       slow = false
        for (int i = 0; i < this->_total_oversamples; i++)
        {
            temp += this->_sht31->getTemperature();
            hum += this->_sht31->getHumidity();
        }
        sample.value[TEMPERRATURE] = (temp / this->_total_oversamples);
        sample.value[HUMIDITY] = (hum / this->_total_oversamples);
        
        // value +="{'sensorName':'"+this->_measurements[TEMPERRATURE].name+"','value':"+this->_measurements[TEMPERRATURE].value+",'unit':'"+this->_measurements[TEMPERRATURE].unit+"'},"
        //          +"{'sensorName':'"+this->_measurements[HUMIDITY].name+"','value':"+this->_measurements[HUMIDITY].value+",'unit':'"+this->_measurements[HUMIDITY].unit+"'}";
    }
    else
    {
        err
    }

    return sample;
}

struct 