#ifndef VOLTAGESENSOR_H
    #define VOLTAGESENSOR_H
    #include <sensorbase.h>
    #include <Arduino.h>

    class voltageSensor: public sensorBase{
        public:
        int sensorPin[BASE_SENSORS_DEFAULT_NR_READINGS];

        voltageSensor(int numberOfSensors, int pinNumber[], String sensorname[], float voltageSenseFactor[], float min_[], float max_[], String unit[], int numberOfSamples=10, long sampleRead_delay=50, int decimals=3){
                ENABLEPIN=0;
                averagingSamples=numberOfSamples;
                checkValueInRange=true;
                sampleReadDelay=sampleReadDelay;
                SENSOR_ENABLE_STATE=HIGH;
                sensorPwrDelay=10;
                numberOfreadings = numberOfSensors;
                for(int i=0; i < numberOfreadings;i++){
                    sensorName[i] = sensorname[i];
                    units[i] = unit[i];
                    sensorStabilizeDelay[i] = sampleRead_delay;
                    sensorReadingDecimals[i] = decimals;
                    EXPECTED_VALUE_MIN[i] = min_[i];
                    EXPECTED_VALUE_MAX[i] = max_[i];  
                    sensorPin[i] = pinNumber[i];
                     pinMode(sensorPin[i], INPUT);
                }               
        }

        int readSensorImpl(float *buffer, int *sensorstatus, long delay_){
            for(int i=0; i < numberOfreadings; i++){
                buffer[i] = analogRead(sensorPin[i]);
                sensorStatus[i] = SENSOR_BASE_SUCCESS;
            }
            return SENSOR_BASE_SUCCESS;
        }

        int enableSensorsImpl(int *sensorstatus){
            for(int i=0; i < numberOfreadings; i++){
                sensorstatus[i] = SENSOR_BASE_SUCCESS;
            }
            return SENSOR_BASE_SUCCESS;
        }
        int disableSensorsImpl(int *sensorstatus){
            for(int i=0; i < numberOfreadings; i++){
                sensorstatus[i] = SENSOR_BASE_SUCCESS;
            }
            return SENSOR_BASE_SUCCESS;
        }
        int calibrateSensorsImpl(int statusLed, int *sensorstatus){
           for(int i=0; i < numberOfreadings; i++){
                sensorstatus[i] = SENSOR_BASE_SUCCESS;
            }
            return SENSOR_BASE_SUCCESS;
        }
    };


#endif