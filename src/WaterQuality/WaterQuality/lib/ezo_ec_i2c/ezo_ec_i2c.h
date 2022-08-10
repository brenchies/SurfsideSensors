#ifndef EZO_EC_I2C_H
#define EZO_EC_I2C_H
#endif
#include <Ezo_i2c.h>

#define EZO_EC_I2C_ADDRESS 0x64
Ezo_board EZO_EC = Ezo_board(EZO_EC_I2C_ADDRESS, "EC");

class ezo_ec_i2c{
    public:
    int numberOfreadings = 1;
    String sensorName[1];
    String samplesBuffer[1];
    String unit[1] = {"μS/cm"};
    unsigned long sensorStabilizeDelay = 3000;
    String errorBuffer[1];
    int status;
    int sampleStatus[1];
    long sampleReadDelay = 1000;

    /**
     * @brief enable pin zero for non
     * 
     */
    int ENABLEPIN = 0;
    float averagingSamples = 1;

    void begin(int enablePin=13, uint8_t address=0x66, float oversamples=10, String sensorName="CONDUCTIVITY"){
        ENABLEPIN = enablePin;
        averagingSamples = oversamples;
        this->sensorName[0] = sensorName;
        if(enablePin != 0){
            pinMode(enablePin, OUTPUT);
            digitalWrite(enablePin, HIGH);
        }

        Serial.print(EZO_EC.get_name()); Serial.print(": "+String(enablePin)); // print the name of the circuit getting the reading
        pinMode(13, OUTPUT);
        digitalWrite(13, HIGH);
        // while(1){
        //     EZO_EC.send_read_cmd();
        //     delay(sampleReadDelay);
        //     EZO_EC.receive_read_cmd(); 
        //     Serial.println(String(EZO_EC.get_last_received_reading(), 2).c_str());
        // }
        
    }

    float readSensor(){
        

        EZO_EC.send_read_cmd();
        delay(sampleReadDelay);
        EZO_EC.receive_read_cmd(); 
        float val = EZO_EC.get_last_received_reading();
        status = EZO_EC.get_error() == 0? 1: -1;
        Serial.println("reading sensor: "+String(val, 3)+ " error: "+String(EZO_EC.get_error()));
        return val;
    }

    void processErrorBuffer(String cause){
        if(errorBuffer[0].length() > 0){errorBuffer[0] += ",";}
        errorBuffer[0] += cause;
        Serial.println(errorBuffer[0]);
    }

    int getSamples(){
        float value = 0;
        for(int i = 0; i < averagingSamples; i++){
            value += readSensor();
            if (status != 1){
                processErrorBuffer(sensorName[0]+" read error");
                break;
            }
        }
        value /= averagingSamples;
        samplesBuffer[0] = String(value, 3);
        Serial.println("temperature: "+String(value));
        return status;
    }

    void enableSensor(){
        digitalWrite(ENABLEPIN, HIGH);
    }
    void disableSensor(){
        digitalWrite(ENABLEPIN, LOW);
    }

    void calibrate(){

    }
};