#ifndef EZO_RTD_I2C_H
    #define EZO_RTD_I2C_H
    #include <Ezo_i2c.h>
    #include <sensorbase.h>

    class ezo_rtd_i2c: public sensorBase{
        public:
        Ezo_board *SENSOR_OBJECT;

        /**
         * @brief begin function, sensor arguments
         * 
         * @param enablePin 
         * @param oversamples 
         * @param sensorName 
         * @param unit 
         */
        void begin(int enablePin=13, uint8_t i2c_address=0x66, float oversamples=5, String sensorname="TEMPERATURE", String unit="°C"){
            ENABLEPIN = enablePin;
            averagingSamples = oversamples;
            checkValueInRange = true;
            sensorPwrDelay = 2000;
            sampleReadDelay = 1000;
            SENSOR_ENABLE_STATE = HIGH;
            sensorName[0] = sensorname;
            units[0] = unit;
            numberOfreadings = 1;
            sensorStabilizeDelay[0] = 3000;
            sensorReadingDecimals[0] = 3;
            EXPECTED_VALUE_RANGE[0][0] = 0;
            EXPECTED_VALUE_RANGE[0][1] = 100;

            // this->sensorName[0] = sensorname;
            if(enablePin != 0){
                pinMode(enablePin, OUTPUT);
                digitalWrite(enablePin, !SENSOR_ENABLE_STATE);
            }
            Ezo_board sensor_object = Ezo_board(i2c_address, sensorname.c_str());
            SENSOR_OBJECT = &sensor_object;
                        
        }

        /**
         * @brief read sensor implementation
         * sum the current value to the buffer
         * report sensor status
         * 
         * @param delay_ 
         * @return int
         */
        
        int readSensorImpl(float *buffer, int *sensorstatus, long delay_){
            int status_ = 0;
            SENSOR_OBJECT->send_read_cmd();
            Serial.println("sensor object 3");
            delay(delay_);
            SENSOR_OBJECT->receive_read_cmd(); 
            float val = SENSOR_OBJECT->get_last_received_reading();
            status_ = SENSOR_OBJECT->get_error() == SENSOR_OBJECT->SUCCESS? 1: -1;
            sensorstatus[0] = status_;
            buffer[0] += val;
            Serial.println(val);
            return status_;
        }

        /**
         * @brief enable sensors implmentation
         * return 1:all sensors anabled, -1: some or all sensors enable failed
         * 
         * @return int 
         */
        int enableSensorsImpl(int *sensorstatus){
            Serial.println("class 2");
            digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
            Serial.println("class 2");
            delay(sensorPwrDelay);
            Serial.println("class 3");
            int status_ = readSensorImpl(samplesTemp, sensorstatus, sampleReadDelay);
            Serial.println("class 4");
            return 1;
        }

        int disableSensorsImpl(int *sensorstatus){
            digitalWrite(ENABLEPIN, !SENSOR_ENABLE_STATE);
            delay(sensorPwrDelay);
            int status_ = 0;
            readSensorImpl(samplesTemp, sensorstatus, sampleReadDelay);
            for(int i=0; i < numberOfreadings; i++){
                status_ += sensorStatus[i];
                if(sensorstatus[i] == -1){
                    sensorstatus[i] = 1;
                }
            }
            status_ = status_ == -numberOfreadings? 1: -1;
            return status_;
        }

        int calibrateSensorsImpl(int statusLed){
            return 0;
        }
    };
#endif