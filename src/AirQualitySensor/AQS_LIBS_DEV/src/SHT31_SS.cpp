#include "SHT31_SS.h"

/**
 * @brief Construct a new sht31 ss::sht31_SS object
 * This default constructor also creates a new SHT31 object. 
 * @param totalSamples Input ammount of samples to average per reading.
 */
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
            this->_sensor.data[TEMPERRATURE].statusCode = Status_code[ERROR_CONNECTION];
            this->_sensor.data[HUMIDITY].statusCode = Status_code[ERROR_CONNECTION];
        }
        delay(1000);
    }
}

/**
 * @brief This function processess sensor measurements samples
 * and gets the average of the total samples selected.
 * Alos sets sensor statuses if there are any errors.
 * 
 */
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
            this->_sensor.data[TEMPERRATURE].statusCode = Status_code[ERROR_VALUE_OUT_OF_RANGE];
        }
        if (!valueInRange(this->_sensor.data[HUMIDITY].value, HUM_MIN, HUM_MAX))
        {
            this->_sensor.data[HUMIDITY].statusCode = Status_code[ERROR_VALUE_OUT_OF_RANGE];
        }
    }
}

/**
 * @brief This functions allows you to acces sensors and their data types.
 * 
 * @return Sensor and their data types.
 */
Sensor SHT31_SS::getSensor()
{
    processAvgSensorMeasurements();

    return this->_sensor;
}

/**
 * @brief Checks if input value is in range
 * 
 * @param val input value
 * @param min input min threshold
 * @param max input max threshold
 * @return true :if value is within range.
 * @return false :if value is not within range.
 */
bool SHT31_SS::valueInRange(float val, float min, float max)
{
    return ((val - min) <= (max - min)); // true, else false
}

/**
 * @brief Processes the sensors data and compiles them in a JSON format stype of protocol.
 * Reports sensors readings and error reports if there are any.
 * @return String JSON in format.
 */
String SHT31_SS::getSensorPayload()
{
    processAvgSensorMeasurements();
    String sensorData = ",";
    for (int i = 0; i < TOTAL_SENSORS; i++)
    {
        if (!this->_sht31->isConnected())
        {
            this->_sensor.data[i].statusCode = Status_code[ERROR_CONNECTION];
            sensorData += sensorErrorProtocol(this->_sensor.data[i].name, this->_sensor.data[i].statusCode, i);
        }
        else
        {
            if (this->_sensor.data[i].statusCode == Status_code[ERROR_VALUE_OUT_OF_RANGE])
            {
                sensorData += sensorErrorProtocol(this->_sensor.data[i].name, this->_sensor.data[i].statusCode, i);
            }
            else
            {
                 this->_sensor.data[i].statusCode = Status_code[SUCCESS_CONNECTED];
                sensorData += sensorProtocol(this->_sensor.data[i].name, String(this->_sensor.data[i].value), this->_sensor.data[i].unit, i);
            }
        }
    }

    return sensorData;
}

/**
 * @brief Formats sensor protocol in JSON.
 * 
 * @param name input sensor data type name.
 * @param value input sensor value.
 * @param unit input sensor unit type.
 * @param index input sensor buffer index.
 * @return String JSON string format of sensor data.
 */
String SHT31_SS::sensorProtocol(String name, String value, String unit, int index)
{
    String sensorData;

    if (index < HUMIDITY)
    {
        sensorData = "{'sensorName':'"+name+"',"+
                     "'value':"+value+","+
                     "'unit':'"+unit+"'"+
                     "},";
    }
    else
    {
        sensorData = "{'sensorName':'"+name+"',"+
                     "'value':"+value+","+
                     "'unit':'"+unit+"'"+
                     "}";
    }
    return sensorData;
}

/**
 * @brief Formats sensor error reporting protocol in JSON.
 * 
 * @param name input sensor data type name.
 * @param errorType input sensor error type.
 * @param index input sensor buffer index.
 * @return String JSON string format of sensor error data.
 */
String SHT31_SS::sensorErrorProtocol(String name, String errorType, int index)
{
    String sensorData;
    if (index < HUMIDITY)
    {
        sensorData = "{'sensorName':'"+name+"',"+
                     "'error':"+errorType+","+
                     "},";
    }
    else
    {
        sensorData = "{'sensorName':'"+name+"',"+
                     "'error':"+errorType+","+
                     "}";
    }
    return sensorData;
}