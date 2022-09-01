#ifndef PMS_SS_H
#define PMS_SS_H
#include <sensorbase.h>
#include <Arduino.h>
#include "PMS.h"
#include "SoftwareSerial.h"
SoftwareSerial serial;

enum Particulates
{
    PM1_0,
    PM2_5,
    PM10_0
};

class PMS_SS : public sensorBase
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
        serial.begin(9600, SWSERIAL_8N1, rxPin, txPin, false, 192);
        PMS pms_(serial);
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

    /**
     * @brief Setting up iplementation for sensor readings.
     * 
     * @param buffer Array of sensor measurements.
     * @param sensorstatus Sensor status code.
     * @param delay_ Delay.
     * @return Int Status code.
     */
    int readSensorImpl(float *buffer, int *sensorstatus, long delay_)
    {
        pms->requestRead();
        if (pms->readUntil(pms_data, 2000))
        {
            buffer[PM1_0] = pms_data.PM_SP_UG_1_0;
            buffer[PM2_5] = pms_data.PM_SP_UG_2_5;
            buffer[PM10_0] = pms_data.PM_SP_UG_10_0;
            sensorStatus[PM1_0] = SENSOR_BASE_SUCCESS;
            sensorStatus[PM2_5] = SENSOR_BASE_SUCCESS;
            sensorStatus[PM10_0] = SENSOR_BASE_SUCCESS;
            return SENSOR_BASE_SUCCESS;
        }
        else
        {
            sensorStatus[PM1_0] = SENSOR_BASE_FAIL;
            sensorStatus[PM2_5] = SENSOR_BASE_FAIL;
            sensorStatus[PM10_0] = SENSOR_BASE_FAIL;
            return SENSOR_BASE_FAIL;
        }
    }

    /**
     * @brief Setting up iplementation for enabling the sensor.
     * 
     * @param sensorstatus Sensor status.
     * @return int Status.
     */
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

    /**
     * @brief Setting up iplementation for disabling the sensor.
     * 
     * @param sensorstatus Sensor status.
     * @return int Status.
     */
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

    /**
     * @brief Setting up iplementation for calibrating the sensor.
     * 
     * @param statusLed Imput status LED pin.
     * @param sensorstatus Sensor status.
     * @return int status.
     */
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