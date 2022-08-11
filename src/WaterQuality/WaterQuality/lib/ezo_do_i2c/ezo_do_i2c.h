#ifndef EZO_RTD_I2C_H
#define EZO_RTD_I2C_H
#endif
#include <Ezo_i2c.h>

class ezo_rtd_i2c{
    public:
    /**
     * @brief number of individual sensor values to return
     * 
     */
    static const int numberOfreadings = 1;

    /**
     * @brief name of sensor array
     * 
     */
    String sensorName[numberOfreadings];

    /**
     * @brief samples of sensor readings
     * 
     */
    String samplesBuffer[numberOfreadings];

    /**
     * @brief units of each sensor
     * 
     */
    String units[numberOfreadings];

    /**
     * @brief delay to wait for sensor to stabilize
     * 
     */
    unsigned long sensorStabilizeDelay[numberOfreadings] = {30000};

    /**
     * @brief error roport from specific sensor reading
     * 
     */
    String errorBuffer[numberOfreadings];

    /**
     * @brief status SUCCESS = 1, ERROR = -1
     * 
     */
    int status;

    /**
     * @brief status of each sample/sensor
     * 
     */
    int sensorStatus[numberOfreadings];

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
    float EXPECTED_VALUE_RANGE[1][2] = {0, 100};

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

    int sensorReadingDecimals[numberOfreadings] = {3};

    float samplesTemp[numberOfreadings];

    Ezo_board *SENSOR_OBJECT;

    /**
     * @brief begin function, sensor arguments
     * 
     * @param enablePin 
     * @param oversamples 
     * @param sensorName 
     * @param unit 
     */
    void begin(int enablePin=13, uint8_t i2c_address=0x66, float oversamples=5, String sensorName="TEMPERATURE", String unit="°C"){
        ENABLEPIN = enablePin;
        averagingSamples = oversamples;
        units[0] = unit;
        this->sensorName[0] = sensorName;
        if(enablePin != 0){
            pinMode(enablePin, OUTPUT);
            digitalWrite(enablePin, !SENSOR_ENABLE_STATE);
        }
        Ezo_board sensor_object = Ezo_board(i2c_address, sensorName.c_str());
        SENSOR_OBJECT = &sensor_object;
    }

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
    int readSensorImpl(float *buffer, int *sensorstatus, long delay_){
        int status_ = 0;
        SENSOR_OBJECT->send_read_cmd();
        delay(delay_);
        SENSOR_OBJECT->receive_read_cmd(); 
        float val = SENSOR_OBJECT->get_last_received_reading();
        status_ = SENSOR_OBJECT->get_error() == SENSOR_OBJECT->SUCCESS? 1: -1;
        sensorstatus[0] = status_;
        buffer[0] += val;
        return status_;
    }

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
        for(int i = 0; i < averagingSamples; i++){readSensorImpl(samplesTemp, sensorStatus, sampleReadDelay);
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
    int enableSensorsImpl(int *sensorstatus){
        digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
        delay(sensorPwrDelay);
        int status_ = readSensorImpl(samplesTemp, sensorstatus, sampleReadDelay);
        return status_;
    }

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
     * @brief begin sensor calibration process
     * 
     * @param statusLedPin (0 if no status led)
     */
    void calibrate(int statusLedPin=0){
        status = 1;
        sensorStatus[0] = status;
    }
};