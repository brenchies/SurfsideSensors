#ifndef PMS_SSS_H
#define PMS_SSS_H
#include <sensorbase.h>
#include <Arduino.h>
#include "PMS.h"
#include "SoftwareSerial.h"
SoftwareSerial serial2;

enum 
{
    PM_1_0,
    PM_2_5,
    PM_10_0
};

class PMS_SSS : public sensorBase
{
public:
    PMS *pms;
    PMS::DATA pms_data;

    /**
     * @brief Begin function for PMS sensor class. All class attributes needed for this class will be defined.
     * 
     * @param rxPin Input RX pin
     * @param txPin Input TX pin
     * @param enablepin Input enable pin
     * @param sensorname Setup sensor name
     * @param unit Input units
     * @param numberOfSamples Input number of samples per reading
     * @param sampleRead_delay Input sample read delay
     * @param decimals Input measurement decimals ammount
     */
    void begin(int rxPin, int txPin, int enablepin, String sensorname[], String unit[], int numberOfSamples = 10, long sampleRead_delay = 50, int decimals = 0)
    {
        ENABLEPIN = enablepin;
        averagingSamples = numberOfSamples;
        checkValueInRange = true;
        sampleReadDelay = sampleReadDelay;
        SENSOR_ENABLE_STATE = HIGH;
        sensorPwrDelay = 500;
        numberOfreadings = 3;
        serial2.begin(9600, SWSERIAL_8N1, rxPin, txPin, false, 192);
        PMS pms_(serial2);
           pms = &pms_;
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
    }

    int readSensorImpl(float *buffer, int *sensorstatus, long delay_)
    {
        pms->requestRead();
        if (pms->readUntil(pms_data, 2000))
        {
            buffer[PM_1_0] = pms_data.PM_SP_UG_1_0;
            buffer[PM_2_5] = pms_data.PM_SP_UG_2_5;
            buffer[PM_10_0] = pms_data.PM_SP_UG_10_0;
            sensorStatus[PM_1_0] = SENSOR_BASE_SUCCESS;
            sensorStatus[PM_2_5] = SENSOR_BASE_SUCCESS;
            sensorStatus[PM_10_0] = SENSOR_BASE_SUCCESS;
            return SENSOR_BASE_SUCCESS;
        }
        else
        {
            sensorStatus[PM_1_0] = SENSOR_BASE_FAIL;
            sensorStatus[PM_2_5] = SENSOR_BASE_FAIL;
            sensorStatus[PM_10_0] = SENSOR_BASE_FAIL;
            return SENSOR_BASE_FAIL;
        }
    }

    int enableSensorsImpl(int *sensorstatus)
    {
        digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
        delay(sensorPwrDelay);
        pms->activeMode();
        pms->wakeUp();
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