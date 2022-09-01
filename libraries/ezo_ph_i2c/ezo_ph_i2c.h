#ifndef EZO_PH_I2C_H
    #define EZO_PH_I2C_H
    #include <Ezo_i2c.h>
    #include <sensorbase.h>
    #include <ezo_rtd_i2c.h>

    class ezo_ph_i2c: public sensorBase, public Ezo_board{
        public:
        /**
         * @brief begin function, sensor arguments
         * 
         * @param enablePin 
         * @param oversamples 
         * @param sensorName 
         * @param unit 
         */
        bool ph_temperature_compensation = true;
        uint8_t ezo_rtd_i2c_address = 0x66;
        ezo_rtd_i2c RTD_TEMP_COMPENSATION;
        ezo_ph_i2c(int enablePin=13, uint8_t address=0x63, float oversamples=5, String sensorname="PH", String unit="NAN") : Ezo_board(address, sensorname.c_str()){
            ENABLEPIN = enablePin;
            averagingSamples = oversamples;
            checkValueInRange = true;
            sensorPwrDelay = 2000;
            sampleReadDelay = 1000;
            SENSOR_ENABLE_STATE = HIGH;
            sensorName[0] = sensorname;
            units[0] = unit;
            numberOfreadings = 1;
            sensorStabilizeDelay[0] = 2100;
            sensorReadingDecimals[0] = 3;
            EXPECTED_VALUE_MIN[0] = 0.001;
            EXPECTED_VALUE_MAX[0] = 14.000;
            sensorName[0] = sensorname;
            i2c_address = address;

            if(enablePin != 0){
                pinMode(enablePin, OUTPUT);
                digitalWrite(enablePin, !SENSOR_ENABLE_STATE);
            }
                        
        }

        /**
         * @brief read sensor implementation
         * current value to the buffer
         * report sensor status
         * 
         * @param delay_ 
         * @return int
         */
        
        int readSensorImpl(float *buffer, int *sensorstatus, long delay_){

            if(ph_temperature_compensation){
                RTD_TEMP_COMPENSATION.getSamples();
                float temperature = RTD_TEMP_COMPENSATION.samplesTemp[0];
                String T_compensate = "T,"+String(temperature, 2);
                send_cmd(T_compensate.c_str());
                delay(1000);
                send_cmd("T,?");
                char buf[32];
                delay(1000);
                receive_cmd(buf, 32);
                bool compensation_confirmed = String(buf).indexOf(String(temperature, 2)) > 0;
                if (!compensation_confirmed){
                    processErrorBuffer(0, "temperature compensation Fail T: "+String(temperature, 2)+" calibration response: "+String(buf));
                    sensorstatus[0] = SENSOR_BASE_FAIL;
                    buffer[0] = SENSOR_BASE_FAIL;
                    return SENSOR_BASE_FAIL;
                }
            }
            
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