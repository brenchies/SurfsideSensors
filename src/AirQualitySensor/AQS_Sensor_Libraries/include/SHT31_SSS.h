#ifndef SHT31_SSS_H
#define SHT31_SSS_H
#define SHT_DEFAULT_ADDRESS 0x44  
#define TYPE_MEASUREMENTS 2  //Temperature, Humidity
#define TEMPERRATURE 0
#define HUMIDITY 1
#include "SHT31.h"

struct Measurement
{
    String name;
    String unit;
    String value;
};

struct SampleMeasurement
{
    float value[TYPE_MEASUREMENTS];
};

enum ERROR
{
    ERROR_CONNECTION = -1,
    ERROR_VALUE_RANGE = -2,
};

class SHT31_SSS 
{
private:
SHT31 *_sht31;
float *_sensorSamples[TYPE_MEASUREMENTS];
int _errorStatus;
int _numberOfMeasurements;
int _total_oversamples;
Measurement _measurements[TYPE_MEASUREMENTS];
SampleMeasurement _sensorSamples;


public:
    SHT31_SSS(int);
    void begin();
    void enableSensors();
    bool valueInRange(float, float, float);
    struct SampleMeasurement getSensorSamples();
};

#endif