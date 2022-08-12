#ifndef SHT31_SSS_H
#define SHT31_SSS_H
#define SHT_DEFAULT_ADDRESS 0x44  
#define TOTAL_SENSORS 2  // Temperature, Humidity
#define TEMP_MIN -25
#define TEMP_MAX 100
#define HUM_MIN 0
#define HUM_MAX 100
#include "SHT31.h"

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

class SHT31_SS
{
private: //Private attributes are only accesible in the class/ library

    SHT31 *_sht31;
    int _totalOverSamples;
    int _connectionStatus;
    Sensor _sensor;

public: //Public attributes and functions that are accesible everywhere that this class / library is implemented

    SHT31_SS(int);
    void begin();
    Sensor getSensor();
    void enableSensors(int);
    void processAvgSensorMeasurements();
    String getSensorPayload();
    String sensorProtocol(String name, String value, String unit, int index);
    String sensorErrorProtocol(String name, String errorType, int index);
    bool valueInRange(float val, float min, float max);
    const String Status_code[3] = {"SUCCESS_CONNECTED", "I2C_CONNECTION_FAILED", "VALUE_OUT_OF_RANGE"};
};


#endif