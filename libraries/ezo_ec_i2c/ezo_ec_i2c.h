#ifndef EZO_EC_I2C_H
    #define EZO_EC_I2C_H
    #include <Ezo_i2c.h>
    #include <sensorbase.h>

    class ezo_ec_i2c: public sensorBase, public Ezo_board{
        public:
        /**
         * @brief begin function, sensor arguments
         * 
         * @param enablePin 
         * @param oversamples 
         * @param sensorName 
         * @param unit 
         */
        ezo_ec_i2c(int enablePin=13, uint8_t address=0x64, float oversamples=5, String sensorname="CONDUCTIVITY", String unit="μS/cm") : Ezo_board(address, sensorname.c_str()){
            ENABLEPIN = enablePin;
            averagingSamples = oversamples;
            checkValueInRange = true;
            sensorPwrDelay = 2000;
            sampleReadDelay = 1000;
            SENSOR_ENABLE_STATE = HIGH;
            sensorName[0] = sensorname;
            units[0] = unit;
            numberOfreadings = 1;
            sensorStabilizeDelay[0] = 2200;
            sensorReadingDecimals[0] = 3;
            EXPECTED_VALUE_MIN[0] = 0.07;
            EXPECTED_VALUE_MAX[0] = 500000;
            sensorName[0] = sensorname;
            i2c_address = address;

            if(enablePin != 0){
                pinMode(enablePin, OUTPUT);
                digitalWrite(enablePin, !SENSOR_ENABLE_STATE);
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
        
        int readSensorImpl(float *buffer, int *sensorstatus, long delay_){
            int status_ = 0;
            send_read_cmd();
            delay(delay_);
            receive_read_cmd(); 
            float val = get_last_received_reading();
            status_ = get_error() == SUCCESS? SENSOR_BASE_SUCCESS: SENSOR_BASE_FAIL;
            sensorstatus[0] = status_;
            buffer[0] = val;
            return status_;
        }

        /**
         * @brief enable sensors implmentation
         * return 1:all sensors anabled, -1: some or all sensors enable failed
         * 
         * @return int 
         */
        int enableSensorsImpl(int *sensorstatus){
            digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
            delay(sensorPwrDelay);
            readSensorImpl(samplesTemp, sensorstatus, sampleReadDelay);
            int status_ = sensorstatus[0];
            return status_;
        }

        /**
         * @brief disable sensor implementation
         * 
         * @param sensorstatus 
         * @return int 
         */
        int disableSensorsImpl(int *sensorstatus){
            digitalWrite(ENABLEPIN, !SENSOR_ENABLE_STATE);
            delay(sensorPwrDelay);
            readSensorImpl(samplesTemp, sensorstatus, sampleReadDelay);
            int status_ = sensorstatus[0] == SENSOR_BASE_FAIL? SENSOR_BASE_SUCCESS: SENSOR_BASE_FAIL;
            sensorstatus[0] = status_;
            return sensorstatus[0];
        }

        /**
         * @brief calibrate sensor implementation
         * 
         * @param sensorstatus 
         * @return int 
         */
        int calibrateSensorsImpl(int statusLed,int *sensorstatus){
            sensorstatus[0] = SENSOR_BASE_SUCCESS;
            return SENSOR_BASE_SUCCESS;
        }
    };
#endif