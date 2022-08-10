#ifndef SHT31_SSS_H
#define SHT31_SSS_H
#define SHT_DEFAULT_ADDRESS 0x44  
#define NUM_MEASUREMENTS 2
#define TEMPERRATURE 0
#define HUMIDITY 1
#include "SHT31.h"

struct Measurement
{
    String name;
    String unit;
    String value;
};

class SHT31_SSS 
{
private:
SHT31 *_sht31;
uint32_t *_start;
uint32_t *_stop;
int _numberOfMeasurements;
int _total_oversamples;
Measurement _measurements[NUM_MEASUREMENTS];


public:
    SHT31_SSS(int total_oversamples, int numberOfMeasurements = NUM_MEASUREMENTS);
    void enableSensors();
    String getSensorReadings();
};

SHT31_SSS::SHT31_SSS(int total_oversamples, int numberOfMeasurements = NUM_MEASUREMENTS)
{
    SHT31 sht31;
    *_sht31 = sht31;
    _numberOfMeasurements = numberOfMeasurements;
    _total_oversamples = total_oversamples;

    _measurements[TEMPERRATURE].name = "Temperature";
    _measurements[TEMPERRATURE].unit = "°C";
    _measurements[HUMIDITY].name = "Humidity";
    _measurements[HUMIDITY].unit = "%";
}
#endif