#ifndef EZO_RTD_I2C_H
#define EZO_RTD_I2C_H
#endif
#include <Ezo_i2c.h>

#define EZO_RTD_I2C_ADDRESS 0x66
Ezo_board EZO_RTD = Ezo_board(EZO_RTD_I2C_ADDRESS, "RTD");

class ezo_rtd_i2c{
    public:
    int numberOfreadings = 1;
    String sensorName[1];
    String samplesBuffer[1];
    String unit[1] = {"C"};
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

    void begin(int enablePin=13, uint8_t address=0x66, float oversamples=10, String sensorName="TEMPERATURE"){
        ENABLEPIN = enablePin;
        averagingSamples = oversamples;
        this->sensorName[0] = sensorName;
        if(enablePin != 0){
            pinMode(enablePin, OUTPUT);
            digitalWrite(enablePin, HIGH);
        }

        Serial.print(EZO_RTD.get_name()); Serial.print(": "+String(enablePin)); // print the name of the circuit getting the reading
        pinMode(13, OUTPUT);
        digitalWrite(13, HIGH);
        // while(1){
        //     EZO_RTD.send_read_cmd();
        //     delay(sampleReadDelay);
        //     EZO_RTD.receive_read_cmd(); 
        //     Serial.println(String(EZO_RTD.get_last_received_reading(), 2).c_str());
        // }
        
    }

    float readSensor(){
        

        EZO_RTD.send_read_cmd();
        delay(sampleReadDelay);
        EZO_RTD.receive_read_cmd(); 
        float val = EZO_RTD.get_last_received_reading();
        status = EZO_RTD.get_error() == 0? 1: -1;
        Serial.println("reading sensor: "+String(val, 3)+ " error: "+String(EZO_RTD.get_error()));
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