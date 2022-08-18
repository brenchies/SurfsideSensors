#ifndef EZO_RTD_I2C_H
    #define EZO_RTD_I2C_H
    #include <Ezo_i2c.h>
    #include <sensorbase.h>

    class ezo_rtd_i2c: public sensorBase, public Ezo_board{
        public:

        /**
         * @brief signle point calibration temperatur in C
         * 
         */
        float calibration_temperature = 30;
        /**
         * @brief begin function, sensor arguments
         * 
         * @param enablePin 
         * @param oversamples 
         * @param sensorName 
         * @param unit 
         */
        ezo_rtd_i2c(int enablePin=13, uint8_t address=0x66, float oversamples=5, String sensorname="TEMPERATURE", String unit="°C") : Ezo_board(address, sensorname.c_str()){
            ENABLEPIN = enablePin;
            averagingSamples = oversamples;
            checkValueInRange = true;
            sensorPwrDelay = 2000;
            sampleReadDelay = 1000;
            SENSOR_ENABLE_STATE = HIGH;
            sensorName[0] = sensorname;
            units[0] = unit;
            numberOfreadings = 1;
            sensorStabilizeDelay[0] = 28600;
            sensorReadingDecimals[0] = 3;
            EXPECTED_VALUE_MIN[0] = -126.000;
            EXPECTED_VALUE_MAX[0] = 1254;
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

        int disableSensorsImpl(int *sensorstatus){
            digitalWrite(ENABLEPIN, !SENSOR_ENABLE_STATE);
            delay(sensorPwrDelay);
            readSensorImpl(samplesTemp, sensorstatus, sampleReadDelay);
            int status_ = sensorstatus[0] == SENSOR_BASE_FAIL? SENSOR_BASE_SUCCESS: SENSOR_BASE_FAIL;
            sensorstatus[0] = status_;
            return sensorstatus[0];
        }

        int calibrateSensorsImpl(int statusLed,int *sensorstatus, int buttonPin){
            String T_calibrate = "Cal,"+String(calibration_temperature, 2);
            Serial.println(T_calibrate);
            send_cmd("Cal,clear");
            delay(1000);
            send_cmd(T_calibrate.c_str());
            delay(1000);
            send_cmd("cal,?");
            delay(1000);
            char buf[32];
            receive_cmd(buf, 32);
            Serial.println(String(buf));
            bool isCalibrated = String(buf).indexOf("CAL,1") > 0;
            if(!isCalibrated){
                processErrorBuffer(0, "calibration Fail T: "+String(calibration_temperature, 2)+" calibration response: "+String(buf));
                sensorstatus[0] = SENSOR_BASE_FAIL;
                return SENSOR_BASE_FAIL;
            }
            Serial.println(sensorName[0]+" calibrated T: "+String(calibration_temperature));
            sensorstatus[0] = SENSOR_BASE_SUCCESS;
            return SENSOR_BASE_SUCCESS;
        }
    };
#endif