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



struct Data
{
    String name;
    String unit;
    float value;
    String status;
};

struct Sensors 
{
    Data data[TYPE_MEASUREMENTS];
};

struct SampleMeasurement
{
    float value[TYPE_MEASUREMENTS];
};


enum STATUS_CODES
{
    SUCCESS_CONNECTED = 0,
    ERROR_CONNECTION = 1,
    ERROR_VALUE_OUT_OF_RANGE = 2
};


class SHT31_SSS 
{
private:
SHT31 *_sht31;
float *_sensorSamples[TYPE_MEASUREMENTS];
int _numberOfMeasurements;
int _total_oversamples;
 SampleMeasurement *_sample;
Sensors *_sensors; //measurements changed to non *

String *_data;


public:
    
    SHT31_SSS(int);
    String getData() {return *this->_data;}
    ///SHT31 getSHT(){return *this->_sht31;}
    void getValue();
    Sensors getSensors() {return *this->_sensors;}
    //int getNumberOfMeasurements() {return this->_numberOfMeasurements;}
    void begin();
    //void enableSensors();
    bool valueInRange(float, float, float);
    //struct SampleMeasurement getSensorSamples();
    //int getAvgSensorReadings();
    String Status_code[3] = {"SUCCESS_CONNECTED", "I2C_CONNECTION_FAILED", "VALUE_OUT_OF_RANGE"};
};

#endif