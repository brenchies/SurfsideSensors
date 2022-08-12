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
    String errorStatus;
};

struct Sensor 
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

class SHT31_SS
{
private:
    SHT31 *_sht31;
    int _totalOverSamples;
    int _connectionStatus;
    Sensor _sensor;
public:
    SHT31_SS(int);
    int getConnectionStatus(){return this->_connectionStatus;}
    void begin();
    void enableSensors(int);
   // Sensor getAvgSensorSamples();
    void processAvgSensorMeasurements();
    String getSensorPayload();
    String sensorProtocol(String name, String value, String unit, int index);
    String sensorErrorProtocol(String name, String errorType, int index);
    bool valueInRange(float val, float min, float max);
    const String Error_code[3] = {"SUCCESS_CONNECTED", "I2C_CONNECTION_FAILED", "VALUE_OUT_OF_RANGE"};
};


#endif