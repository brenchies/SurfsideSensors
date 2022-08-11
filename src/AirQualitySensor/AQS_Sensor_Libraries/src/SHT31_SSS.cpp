#include "SHT31_SSS.h"

SHT31_SSS::SHT31_SSS(int total_oversamples)
{
    SHT31 sht31;
    this->_sht31 = &sht31;
    _numberOfMeasurements = TYPE_MEASUREMENTS;
    _total_oversamples = total_oversamples;
    
    this->_sensors->data[TEMPERRATURE].name = "Temperature";
    this->_sensors->data[TEMPERRATURE].unit = "°C";
    this->_sensors->data[HUMIDITY].name = "Humidity";
    this->_sensors->data[HUMIDITY].unit  = "%";
}

void SHT31_SSS::begin()
{
    this->_sht31->begin(SHT_DEFAULT_ADDRESS);
}
void SHT31_SSS::enableSensors()
{
    this->_sht31->read(); // default = true/fast       slow = false
}

bool SHT31_SSS::valueInRange(float val, float min, float max)
{
    return ((val - min) <= (max - min)); // true, else false
}

struct SampleMeasurement SHT31_SSS::getSensorSamples()
{
    // struct SampleMeasurement sample;
    // sample.value[TEMPERRATURE] = this->_sht31->getTemperature();
    // sample.value[HUMIDITY] = this->_sht31->getHumidity();
    // // value +="{'sensorName':'"+this->_measurements[TEMPERRATURE].name+"','value':"+this->_measurements[TEMPERRATURE].value+",'unit':'"+this->_measurements[TEMPERRATURE].unit+"'},"
    // //          +"{'sensorName':'"+this->_measurements[HUMIDITY].name+"','value':"+this->_measurements[HUMIDITY].value+",'unit':'"+this->_measurements[HUMIDITY].unit+"'}";
    // return sample;
}

int SHT31_SSS::getAvgSensorReadings()
{
    int status;
    struct SampleMeasurement sample;
    if (this->_sht31->isConnected())
    {
        enableSensors();
        for (int i = 0; i < this->_total_oversamples; i++)
        {
            sample.value[TEMPERRATURE] += this->_sht31->getTemperature();
            sample.value[HUMIDITY] += this->_sht31->getHumidity();
        }
        sample.value[TEMPERRATURE] /= _total_oversamples;
        sample.value[HUMIDITY] /=  _total_oversamples;

        if (!valueInRange(sample.value[TEMPERRATURE],TEMP_MIN,TEMP_MAX))
        {
            this->_sensors->data[TEMPERRATURE].status = Status_code[ERROR_VALUE_OUT_OF_RANGE];
            this->_sensors->data[HUMIDITY].value = String(sample.value[HUMIDITY],3);
        }
        else if (!valueInRange(sample.value[HUMIDITY],HUM_MIN,HUM_MAX))
        {
            this->_sensors->data[HUMIDITY].status = Status_code[ERROR_VALUE_OUT_OF_RANGE];
            this->_sensors->data[TEMPERRATURE].value = String(sample.value[TEMPERRATURE],3);
        }
        else
        {
            this->_sensors->data[TEMPERRATURE].value = String(sample.value[TEMPERRATURE],3);
            this->_sensors->data[HUMIDITY].value = String(sample.value[HUMIDITY],3);
            this->_sensors->data[TEMPERRATURE].status = Status_code[SUCCESS_CONNECTED];
            this->_sensors->data[HUMIDITY].status = Status_code[SUCCESS_CONNECTED];
            status = SUCCESS_CONNECTED;
        }
    }
    else
    {
        this->_sensors->data[TEMPERRATURE].status = Status_code[ERROR_CONNECTION];
        this->_sensors->data[HUMIDITY].status = Status_code[ERROR_CONNECTION];
        status = ERROR_CONNECTION;
    }
    return status;
}
