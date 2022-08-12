#ifndef SHT31_S_H
#define SHT31_S_H
#include <sensorbase.h>
#include <Arduino.h>
#include "SHT31.h"

class SHT31_S : public sensorBase, public SHT31
{
public:
    enum Sensors
    {
        TEMPERATURE,
        HUMIDITY
    };

    SHT31_S(int enablepin, String sensorname[], String unit[], int numberOfSamples = 10, long sampleRead_delay = 50, int decimals = 3):SHT31()
    {
        ENABLEPIN = enablepin;
        averagingSamples = numberOfSamples;
        checkValueInRange = true;
        sampleReadDelay = sampleReadDelay;
        SENSOR_ENABLE_STATE = HIGH;
        sensorPwrDelay = 500;
        numberOfreadings = 2;
        sensorName[TEMPERATURE] = sensorname[TEMPERATURE];
        sensorName[HUMIDITY] = sensorname[HUMIDITY];
        units[TEMPERATURE] = unit[TEMPERATURE];
        units[HUMIDITY] = unit[HUMIDITY];
        sensorStabilizeDelay[TEMPERATURE] = sampleRead_delay;
        sensorStabilizeDelay[HUMIDITY] = sampleRead_delay;
        sensorReadingDecimals[TEMPERATURE] = decimals;
        sensorReadingDecimals[HUMIDITY] = decimals;
        EXPECTED_VALUE_MIN[TEMPERATURE] = -20;
        EXPECTED_VALUE_MIN[HUMIDITY] = 0;
        EXPECTED_VALUE_MAX[TEMPERATURE] = 60;
        EXPECTED_VALUE_MAX[HUMIDITY] = 110;
        if (ENABLEPIN != 0)
        {
            pinMode(ENABLEPIN, OUTPUT);
        }
    }

    int readSensorImpl(float *buffer, int *sensorstatus, long delay_)
    {
        Serial.println("reading");
        if (isConnected())
        {
            Serial.println("enabling");
            read();
            Serial.println("reading2");
            buffer[0] = getTemperature();
            buffer[1] = getTemperature();
            sensorStatus[0] = SENSOR_BASE_SUCCESS;
            sensorStatus[1] = SENSOR_BASE_SUCCESS;
            return SENSOR_BASE_SUCCESS;
        }
        else
        {
            sensorStatus[0] = SENSOR_BASE_FAIL;
            sensorStatus[1] = SENSOR_BASE_FAIL;
            return SENSOR_BASE_FAIL;
        }
    }

    int enableSensorsImpl(int *sensorstatus)
    {
        digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
        delay(sensorPwrDelay);
        Serial.println("enabling");
        int status_ = readSensorImpl(samplesBufferTemp, sensorStatus, 0);
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
        Serial.println("disabling");
        int status_ = readSensorImpl(samplesBufferTemp, sensorStatus, 0);
        for (int i = 0; i < numberOfreadings; i++)
        {
            sensorstatus[i] = !status_;
        }
        return !status_;
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