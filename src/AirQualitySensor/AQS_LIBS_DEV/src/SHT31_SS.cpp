#include "SHT31_SS.h"

SHT31_SS::SHT31_SS(int totalSamples)
{
    SHT31 sht31;
    this->_sht31 = &sht31;
    this->_totalOverSamples = totalSamples;
    this->_sensor.data[TEMPERRATURE].name = "Temperature";
    this->_sensor.data[TEMPERRATURE].unit = "°C";
    this->_sensor.data[HUMIDITY].name = "Humidity";
    this->_sensor.data[HUMIDITY].unit = "%";
}

void SHT31_SS::begin()
{
    this->_sht31->begin();
}

void SHT31_SS::enableSensors(int trials = 3)
{
    for (int i = 0; i < trials; i++)
    {
        if (this->_sht31->isConnected())
        {
            this->_connectionStatus = SUCCESS_CONNECTED;
            break;
        }
        else
        {
            this->_connectionStatus = ERROR_CONNECTION;
            this->_sensor.data[TEMPERRATURE].errorStatus = Error_code[ERROR_CONNECTION];
            this->_sensor.data[HUMIDITY].errorStatus = Error_code[ERROR_CONNECTION];
        }
    }
}

void SHT31_SS::processAvgSensorMeasurements()
{
    this->_sht31->read();
    SampleMeasurement samples;
    if (this->_connectionStatus == SUCCESS_CONNECTED)
    {
        for (int i = 0; i < this->_totalOverSamples; i++)
        {
            samples.value[TEMPERRATURE] += this->_sht31->getTemperature();
            samples.value[HUMIDITY] += this->_sht31->getHumidity();
        }
        this->_sensor.data[TEMPERRATURE].value = samples.value[TEMPERRATURE] / this->_totalOverSamples;
        this->_sensor.data[HUMIDITY].value = samples.value[HUMIDITY] / this->_totalOverSamples;

        if (!valueInRange(this->_sensor.data[TEMPERRATURE].value, TEMP_MIN, TEMP_MAX))
        {
            this->_sensor.data[TEMPERRATURE].errorStatus = Error_code[ERROR_VALUE_OUT_OF_RANGE];
        }
        if (!valueInRange(this->_sensor.data[HUMIDITY].value, HUM_MIN, HUM_MAX))
        {
            this->_sensor.data[HUMIDITY].errorStatus = Error_code[ERROR_VALUE_OUT_OF_RANGE];
        }
    }
}

bool SHT31_SS::valueInRange(float val, float min, float max)
{
    return ((val - min) <= (max - min)); // true, else false
}

// Sensor SHT31_SS::getSensor()
// {
//     this->_sensor.data[TEMPERRATURE].value = getAvgSensorSamples().value[TEMPERRATURE];
//     this->_sensor.data[HUMIDITY].value = getAvgSensorSamples().value[HUMIDITY];

//     if (valueInRange(this->_sensor.data[TEMPERRATURE].value, TEMP_MIN, TEMP_MAX))
//     {
//         this->_sensor.data[TEMPERRATURE].errorStatus = Error_code[ERROR_VALUE_OUT_OF_RANGE];
//     }
//     if (valueInRange(this->_sensor.data[HUMIDITY].value, HUM_MIN, HUM_MAX))
//     {
//         this->_sensor.data[HUMIDITY].errorStatus = Error_code[ERROR_VALUE_OUT_OF_RANGE];
//     }

//     return _sensor;
// }

String SHT31_SS::getSensorPayload()
{
    processAvgSensorMeasurements();
    String sensorData = ",";
    for (int i = 0; i < TYPE_MEASUREMENTS; i++)
    {
        if (this->_connectionStatus == ERROR_CONNECTION)
        {
            sensorData += sensorErrorProtocol(this->_sensor.data[i].name, this->_sensor.data[i].errorStatus, i);
        }
        else
        {
            if (this->_sensor.data[i].errorStatus == Error_code[ERROR_VALUE_OUT_OF_RANGE])
            {
                sensorData += sensorErrorProtocol(this->_sensor.data[i].name, this->_sensor.data[i].errorStatus, i);
            }
            else
            {
                sensorData += sensorProtocol(this->_sensor.data[i].name, String(this->_sensor.data[i].value), this->_sensor.data[i].errorStatus, i);
            }
        }
        delay(1000);
    }

    return sensorData;
}

String SHT31_SS::sensorProtocol(String name, String value, String unit, int index)
{
    String sensorData;

    if (index < HUMIDITY)
    {
        sensorData = "{'sensorName':'" + name + "'," +
                     "  'value':" + value + "," +
                     "  'unit':'" + unit + "'" +
                     "},";
    }
    else
    {
        sensorData = "{'sensorName':'" + name + "'," +
                     "  'value':" + value + "," +
                     "  'unit':'" + unit + "'" +
                     "}";
    }
    return sensorData;
}

String SHT31_SS::sensorErrorProtocol(String name, String errorType, int index)
{
    String sensorData;
    if (index < HUMIDITY)
    {
        sensorData = "{'sensorName':'" + name + "'," +
                     "  'error':" + errorType + "," +
                     "},";
    }
    else
    {
        sensorData = "{'sensorName':'" + name + "'," +
                     "  'error':" + errorType + "," +
                     "}";
    }
    return sensorData;
}