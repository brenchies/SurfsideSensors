#ifndef SENSOR_BASE_H
    #define SENSOR_BASE_I2C_H
    #include <Arduino.h>
    #pragma once
    #ifndef BASE_SENSORS_DEFAULT_NR_SENSORS
        #define BASE_SENSORS_DEFAULT_NR_READINGS 10
    #endif

    /**
     * @brief sensor base class
     * The following variables have to be modified in your initialization funtion
     * 
     * int ENABLEPIN;
     * int averagingSamples;
     * bool checkValueInRange;
     * unsinged long sensorPwrDelay;
     * unsinged long sampleReadDelay;
     * bool SENSOR_ENABLE_STATE;
     * String sensorName[i];
     * String units[i];
     * int numberOfreadings;
     * unsinged long sensorStabilizeDelay[i];
     * int sensorReadingDecimals[i];
     * String EXPECTED_VALUE_RANGE[i][0]=low;
     * EXPECTED_VALUE_RANGE[i][1]=max;
     * 
     * where i is the elemnet index 
     */
    class sensorBase{
        public:
        /**
         * @brief number of individual sensor values to return set to the 
         * define BASE_SENSORS_DEFAULT_NR_READINGS to increase the number as well
         * so numberOfreadings <= BASE_SENSORS_DEFAULT_NR_READINGS
         * 
         */
        int numberOfreadings = 0;

        /**
         * @brief name of sensor array
         * 
         */
        String sensorName[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief samples of sensor readings
         * 
         */
        String samplesBuffer[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief units of each sensor
         * 
         */
        String units[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief delay to wait for sensor to stabilize
         * 
         */
        unsigned long sensorStabilizeDelay[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief error roport from specific sensor reading
         * 
         */
        String errorBuffer[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief status SUCCESS = 1, ERROR = -1
         * 
         */
        int status;

        /**
         * @brief status of each sample/sensor
         * 
         */
        int sensorStatus[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief delay to wait for each sample
         * 
         */
        long sampleReadDelay = 1000;

        /**
         * @brief power state
         * 
         */
        bool SENSOR_ENABLE_STATE = HIGH;
        
        /**
         * @brief range of expected values
         * 
         */
        float EXPECTED_VALUE_RANGE[BASE_SENSORS_DEFAULT_NR_READINGS][2];;

        /**
         * @brief condition to check if value is in expected range
         * 
         */
        bool checkValueInRange = true;
        
        /**
         * @brief power delay before probing if sensor is enabled or disabled
         * 
         */
        long sensorPwrDelay = 2000;

        /**
         * @brief enable pin zero for non enable pin
         * 
         */
        int ENABLEPIN = 0;

        /**
         * @brief samples used for oversampling
         * 
         */
        float averagingSamples = 1;

        /**
         * @brief decimals to use
         * 
         */
        int sensorReadingDecimals[BASE_SENSORS_DEFAULT_NR_READINGS] = {3};

        /**
         * @brief buffer
         * 
         */
        float samplesTemp[BASE_SENSORS_DEFAULT_NR_READINGS];

        /**
         * @brief check if value is in range
         * 
         * @param val 
         * @param min_range 
         * @param max_range 
         * @return true 
         * @return false 
         */
        bool valueInRange(float val, float min_range=-100000, float max_range=100000){
            float min_ = min_range, max_ = max_range;
            if(min_range < 0){
                min_ = 0;
                max_ = max_range - min_range;
                val -= min_range;
            }
            if (val >= min_ && val <= max_){
                return true;
            }else{
                return false;
            }
        }

        /**
         * @brief read sensor implementation
         * sum the current value to the buffer
         * report sensor status
         * 
         * @param delay_ 
         * @return int
         */
        virtual int readSensorImpl(float *buffer, int *sensorstatus, long delay_){return 0;};

        /**
         * @brief add report to a specific error buffer
         * 
         * @param bufferNr 
         * @param cause 
         */
        void processErrorBuffer(int bufferNr, String cause){
            if(errorBuffer[bufferNr].length() > 0){errorBuffer[bufferNr] += ",";}
            errorBuffer[bufferNr] += "|"+cause+"|";
        }

        /**
         * @brief get the samples, do average, report read error
         * 
         * @return int [1, -1]
         */
        int getSamples(){
            for(int i=0; i < numberOfreadings; i++){samplesTemp[i] = 0;}
            status = 1;
            //read samples
            for(int i = 0; i < averagingSamples; i++){
                readSensorImpl(samplesTemp, sensorStatus, sampleReadDelay);
            }
            //process samples and sensor status
            for(int i=0; i < numberOfreadings; i++){
                if (sensorStatus[i] == 1)
                {
                    samplesTemp[i] /= averagingSamples;
                    samplesBuffer[i] = String(samplesTemp[i], sensorReadingDecimals[i]);
                    if(checkValueInRange && !valueInRange(samplesTemp[i], EXPECTED_VALUE_RANGE[i][0], EXPECTED_VALUE_RANGE[i][1])){
                        sensorStatus[i] = -1;
                        status = -1;
                        processErrorBuffer(i, "value out of range ["+String(EXPECTED_VALUE_RANGE[i][0])+","+String(EXPECTED_VALUE_RANGE[i][1])+"]: "+String(samplesTemp[i]));
                    }
                }else{
                    processErrorBuffer(i, "read error");
                    status = -1;
                }
            }
            return status;
        }

        /**
         * @brief enable sensors implmentation
         * return 1:all sensors anabled, -1: some or all sensors enable failed
         * 
         * @return int 
         */
        virtual int enableSensorsImpl(int *sensorstatus){ return 0;};

        /**
         * @brief enable sensor and check if alive
         * 
         * @param trials 
         * @return int 
         */
        int enableSensors(int trials=3)
        {
            int status_ = 0;
            for(int i=0; i<trials;i++)
            {
                status = enableSensorsImpl(sensorStatus);
                for(int i2=0; i2 < numberOfreadings;i2++){
                    if(sensorStatus[i2]==1){
                        status_ += 1;
                    }
                }
                if(status_ == numberOfreadings){
                    break;
                }
            }
            
            for(int i=0; i < numberOfreadings; i++){
                if(sensorStatus[i] == -1){
                    processErrorBuffer(i, "enable failed");
                    status = -1;
                }
            }

            if (status != 1){
                disableSensors();
            }
            return status;
        }
        
        virtual int disableSensorsImpl(int *sensorstatus){ return 0;};


        /**
         * @brief disable sensor and check if dead
         * 
         * @param trials 
         * @return int 
         */
        int disableSensors(int trials=3){
            for(int i=0; i<trials;i++)
            {
                status = disableSensorsImpl(sensorStatus);
                if(status == 1){
                    break;
                }
            }

            for(int i=0; i < numberOfreadings; i++){
                if(sensorStatus[i] == -1)
                {
                    processErrorBuffer(i, "disable failed");
                    status = -1;
                }
            }
            return status;
        }

        /**
         * @brief seensor calibration function implementation
         * 
         * @param statusLed ( 0 if no status led)
         * @return int 
         */
        virtual int calibrateSensorsImpl(int statusLed){ return 0;};

        /**
         * @brief begin sensor calibration process
         * 
         * @param statusLedPin (0 if no status led)
         */
        int calibrate(int statusLedPin=0){
            return calibrateSensorsImpl(statusLedPin);
        }
    };
#endif