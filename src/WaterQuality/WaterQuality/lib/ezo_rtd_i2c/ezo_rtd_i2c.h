#ifndef EZO_RTD_I2C_H
#define EZO_RTD_I2C_H
#endif
#include <Arduino.h>
#include <Ezo_i2c.h>
#include <Wire.h>

#define ezo_rtd_sensor_readings 1
class ezo_rtd_i2c: public Ezo_board{
    public:
    const int numberOfreadings = ezo_rtd_sensor_readings;
    String sensorName[ezo_rtd_sensor_readings];
    String sensorName[ezo_rtd_sensor_readings];
    String samplesBuffer[ezo_rtd_sensor_readings];
    String unit[ezo_rtd_sensor_readings];
    unsigned long sensorStabilizeDelay = 30000;
    String errorBuffer[ezo_rtd_sensor_readings];
    int status;
    int sampleStatus[ezo_rtd_sensor_readings];

    /**
     * @brief enable pin zero for non
     * 
     */
    int ENABLEPIN = 0;
    int averagingSamples = 1;

    void begin(int enablePin=13, uint8_t address=0x66, oversamples=10, String sensorName=""){
        ENABLEPIN = enablePin;
        i2c_address = address;
        averagingSamples = 
    };
    void getSamples();
    void calibrate();
    void enableSensor();
    void disableSensor();
};