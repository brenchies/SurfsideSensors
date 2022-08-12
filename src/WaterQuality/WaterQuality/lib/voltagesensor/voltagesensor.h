#ifndef VOLTAGESENSOR_H
    #define VOLTAGESENSOR_H
    #include <sensorbase.h>
    #include <Arduino.h>

    class voltageSensor: public sensorBase{
        public:
        int sensorPin;

        void begin(int pinNumber=36, String sensorname="SOLAR_VIN", String unit="mV", int numberOfSamples=10, long sampleRead_delay=50, float voltageSenseFactor=3300 / 4096.0 * 3300.0 / 1508.20){
                ENABLEPIN=0;
                averagingSamples=numberOfSamples;
                checkValueInRange=true;
                sensorPwrDelay=0;
                sampleReadDelay=sampleReadDelay;
                SENSOR_ENABLE_STATE=HIGH;
                sensorName[0] = sensorname;
                units[0] = unit;
                numberOfreadings = 1;
                sensorStabilizeDelay[0] = 0;
                sensorReadingDecimals[0] = 3;
                EXPECTED_VALUE_RANGE[0][0] = 0;
                EXPECTED_VALUE_RANGE[0][1] = 5000;   

                sensorPin = pinNumber;
        }

        int readSensorImpl(float *buffer, int *sensorstatus, long delay_){
            float val = analogRead(sensorPin);
            buffer[0] += val;
            sensorStatus[0] = 1;
            return 1;
        }
        int enableSensorsImpl(int *sensorstatus){
            sensorstatus[0] = 1;
            return 1;
        }
        int disableSensorsImpl(int *sensorstatus){
            sensorstatus[0] = 1;
            return 1;
        }
        int calibrateSensorsImpl(int statusLed){

        }
    };


#endif