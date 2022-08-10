#ifndef EZO_RTD_I2C_H
#define EZO_RTD_I2C_H
#endif
#include <Ezo_i2c.h>

#ifndef EZO_RTD_I2C_ADDRESS
    #define EZO_RTD_I2C_ADDRESS 0x66
#endif


Ezo_board SENSOR_OBJECT = Ezo_board(EZO_RTD_I2C_ADDRESS, "RTD");

class ezo_rtd_i2c{
    public:
    /**
     * @brief number of indicidual sensor values
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
    unsigned long sensorStabilizeDelay[numberOfreadings] = {5000};

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
    float EXPECTED_VALUE_RANGE[2] = {0, 100};

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
     * @brief begin function, sensor arguments
     * 
     * @param enablePin 
     * @param oversamples 
     * @param sensorName 
     * @param unit 
     */
    void begin(int enablePin=13, float oversamples=5, String sensorName="TEMPERATURE", String unit="°C"){
        ENABLEPIN = enablePin;
        averagingSamples = oversamples;
        units[0] = unit;
        this->sensorName[0] = sensorName;
        if(enablePin != 0){
            pinMode(enablePin, OUTPUT);
            digitalWrite(enablePin, !SENSOR_ENABLE_STATE);
        }
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
     * @brief read sensor
     * 
     * @param delay_ 
     * @return float 
     */
    float readSensor(long delay_){
        SENSOR_OBJECT.send_read_cmd();
        delay(delay_);
        SENSOR_OBJECT.receive_read_cmd(); 
        float val = SENSOR_OBJECT.get_last_received_reading();
        status = SENSOR_OBJECT.get_error() == SENSOR_OBJECT.SUCCESS? 1: -1;
        return val;
    }

    /**
     * @brief add report to a specific error buffer
     * 
     * @param bufferNr 
     * @param cause 
     */
    void processErrorBuffer(int bufferNr, String cause){
        if(errorBuffer[0].length() > 0){errorBuffer[0] += ",";}
        errorBuffer[0] += "{'error': 'sensor "+sensorName[0]+" "+cause+"'}";
    }

    /**
     * @brief get the samples and do average
     * 
     * @return int [1, -1]
     */
    int getSamples(){
        float value = 0;
        for(int i = 0; i < averagingSamples; i++){
            value += readSensor(sampleReadDelay);
            if (status != 1){
                processErrorBuffer(0, "read error");
                break;
            }
        }
        value /= averagingSamples;
        if (status == 1 && checkValueInRange){
            if(!valueInRange(value, EXPECTED_VALUE_RANGE[0], EXPECTED_VALUE_RANGE[1]))
            {
                processErrorBuffer(0, "value out of range ["+String(EXPECTED_VALUE_RANGE[0])+","+String(EXPECTED_VALUE_RANGE[1])+"]: "+String(value));
                status = -1;
            }
        }
        samplesBuffer[0] = String(value, 3);
        sensorStatus[0] = status;
        return status;
    }

    /**
     * @brief enable sensor and check if alive
     * 
     * @param trials 
     * @return int 
     */
    int enableSensors(int trials=3)
    {
        for(int i=0; i<trials;i++)
        {
            digitalWrite(ENABLEPIN, SENSOR_ENABLE_STATE);
            delay(sensorPwrDelay);
            readSensor(sampleReadDelay);
            if(status == 1){
                break;
            }
        }

        if (status != 1){
            processErrorBuffer(0, "enable failed");
            disableSensors();
        }
        sensorStatus[0] = status;
        return status;
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
            digitalWrite(ENABLEPIN, !SENSOR_ENABLE_STATE);
            delay(sensorPwrDelay);
            readSensor(sampleReadDelay);
            if(status == -1){
                break;
            }
        }

        if (status != -1){
            processErrorBuffer(0, "disable failed");
        }else{
            status = 1;
        }
        
        sensorStatus[0] = status;
        return status;
    }

    /**
     * @brief begin sensor calibration process
     * 
     * @param LedPin (0 if no status led)
     */
    void calibrate(int LedPin=0){

        sensorStatus[0] = status;
    }
};