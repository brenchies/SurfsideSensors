#ifndef PMS_SS_H
#define PMS_SS_H
#include "PMS.h"
#define TOTAL_SENSORS 2  // Temperature, Humidity
#define TEMP_MIN -25
#define TEMP_MAX 100
#define HUM_MIN 0
#define HUM_MAX 100


/**
 * @brief Data types.
 */
struct Data
{
    String name;
    String unit;
    float value;
    String statusCode;
};

/**
 * @brief Accesible data types for specefic Sensor.
 */
struct Sensor 
{
    Data data[TOTAL_SENSORS];
};

/**
 * @brief Measurement buffer for different types of measurements.
 */
struct SampleMeasurement
{
    float value[TOTAL_SENSORS]; 
};

/**
 * @brief Sensor types
 */
enum Sensors
{
    TEMPERRATURE, 
    HUMIDITY 
};

/**
 * @brief Status codes
 */
enum STATUS_CODES
{
    SUCCESS_CONNECTED, 
    ERROR_CONNECTION, 
    ERROR_VALUE_OUT_OF_RANGE 
};

class PMS_SS : public PMS
{
private:
//String getName();
int _numberOfMeasurements;
int _totalOverSamples;
int _connectionStatus;
Sensor _sensor;

public:
    PMS_SS(Stream&, String name, int txPin, int rxPin);
    bool readUntil(DATA& data, uint16_t timeout = SINGLE_RESPONSE_TIME);
     //bool readUntil(DATA &data, int total_samples = 3, uint16_t timeout = SINGLE_RESPONSE_TIME);
};

#endif