#ifndef SPS30_SS_H
#define SPS30_SS_H
#include <sensorbase.h>
#include <Arduino.h>
#include "sps30.h"
#define INCLUDE_I2C // define I2C communication for SPS30

class SPS30_SS : public sensorBase, private SPS30
{
public:
    enum Sensors
    {
        SPS_PM1,
        SPS_PM2_5,
        SPS_PM10_0
    };

    struct sps_values pm;

    SPS30_SS(int enablepin, String sensorname[], String unit[], int numberOfSamples = 10, long sampleRead_delay = 50, int decimals = 0) : SPS30()
    {
        ENABLEPIN = enablepin;
        averagingSamples = numberOfSamples;
        checkValueInRange = true;
        sampleReadDelay = sampleReadDelay;
        SENSOR_ENABLE_STATE = HIGH;
        sensorPwrDelay = 500;
        numberOfreadings = 3;
        for (int i = 0; i < numberOfreadings; i++)
        {
            sensorName[i] = sensorname[i];
            units[i] = unit[i];
            sensorStabilizeDelay[i] = 30000;
            sensorReadingDecimals[i] = decimals;
            EXPECTED_VALUE_MIN[i] = 0;
            EXPECTED_VALUE_MAX[i] = 1000000;
            pinMode(ENABLEPIN, OUTPUT);
        }
        if (ENABLEPIN != 0)
        {
            pinMode(ENABLEPIN, OUTPUT);
            digitalWrite(ENABLEPIN, HIGH);
        }
        if (ENABLEPIN != 0)
        {
            pinMode(ENABLEPIN, OUTPUT);
        }
    }

    int readSensorImpl(float *buffer, int *sensorstatus, long delay_)
    {
        uint8_t ret = GetValues(&pm);
        if (ret != SPS30_ERR_OK)
        {
            buffer[0] = pm.MassPM1;
            buffer[1] = pm.MassPM2;
            buffer[2] = pm.MassPM10;
            sensorStatus[0] = SENSOR_BASE_SUCCESS;
            sensorStatus[1] = SENSOR_BASE_SUCCESS;
            sensorStatus[2] = SENSOR_BASE_SUCCESS;
            return SENSOR_BASE_SUCCESS;
        }
        else
        {
            sensorStatus[0] = SENSOR_BASE_FAIL;
            sensorStatus[1] = SENSOR_BASE_FAIL;
            sensorStatus[2] = SENSOR_BASE_FAIL;
            return SENSOR_BASE_FAIL;
        }
    }

    int enableSensorsImpl(int *sensorstatus)
    {
        digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
        delay(sensorPwrDelay);
        int status_ = readSensorImpl(samplesBufferTemp, sensorstatus, 0);
        for (int i = 0; i < numberOfreadings; i++)
        {
            sensorstatus[i] = status_;
        }
        return status_;
    }
    int disableSensorsImpl(int *sensorstatus)
    {
        digitalWrite(ENABLEPIN, !SENSOR_ENABLE_STATE);
        delay(sensorPwrDelay);
        int status_ = readSensorImpl(samplesBufferTemp, sensorstatus, 0) == SENSOR_BASE_FAIL ? SENSOR_BASE_SUCCESS : SENSOR_BASE_FAIL;
        for (int i = 0; i < numberOfreadings; i++)
        {
            sensorstatus[i] = status_;
        }
        return status_;
    }

    int calibrateSensorsImpl(int statusLed, int *sensorstatus)
    {
        for (int i = 0; i < numberOfreadings; i++)
        {
            sensorstatus[i] = SENSOR_BASE_SUCCESS;
        }
        return SENSOR_BASE_SUCCESS;
    }
};

#endif