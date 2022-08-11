#ifndef SHT31_SSS_H
#define SHT31_SSS_H
#define SHT_DEFAULT_ADDRESS 0x44  
#define TYPE_MEASUREMENTS 2  //Temperature, Humidity
#define TEMPERRATURE 0
#define HUMIDITY 1
#define TEMP_MIN -25
#define TEMP_MAX 100
#define HUM_MIN 0
#define HUM_MAX 100
#include "SHT31.h"

struct SensorData
{
    String name;
    String unit;
    String value;
};

struct SampleMeasurement
{
    float value[TYPE_MEASUREMENTS];
};

enum STATUS_CODES
{
    SUCCESS = 1,
    ERROR_CONNECTION = -1,
    ERROR_VALUE_OUT_OF_RANGE = -2
};

class SHT31_SSS 
{
private:
SHT31 *_sht31;
float *_sensorSamples[TYPE_MEASUREMENTS];
int _numberOfMeasurements;
int _total_oversamples;
SensorData *_sensorData[TYPE_MEASUREMENTS]; //measurements


public:
    SHT31_SSS(int);
    void begin();
    void enableSensors();
    bool valueInRange(float, float, float);
    struct SampleMeasurement getSensorSamples();
    int getAvgSensorReadings();
};

#endif