#include <Arduino.h>
#include <cstdarg>
#include <stdio.h>
#ifndef SURFSIDESCIENCE_H
#define SURFSIDESCIENCE_H
#endif

int SUCCESS = 1;
int ERROR = -1;


class surfSideScience{
    public:
    /**
     * @brief 
     * 
     */
    String deviceName="";

    /**
     * @brief error strings {err, }
     * 
     */

    /**
     * @brief {error},...,{errorN}
     * 
     */
    String errorBuffer="";

    /**
     * @brief payload = {
     *                  'deviceName':'id',
     *                  'timestamp':'ISO 8601',
     *                  'sensors': {sensor data}
     *                   }     * 
     */
    String payload="{'deviceName':'WATER_QUALITY_01','timestamp':'2022-08-09 18:11:04.000-4:00','sensors':[{'sensorName':'DISSOLVED_OXYGEN','value':1.97,'unit':'mg/L'},{'sensorName':'CONDUCTIVITY','value':0.00,'unit':'μS/cm'},{'sensorName':'TEMPERATURE','value':0.00,'unit':'°C'},{'sensorName':'PH','value':0.00,'unit':'NAN'},{'sensorName':'RSSI','value':31,'unit':'NAN'},{'sensorName':'SOLAR_VIN','value':3179.96,'unit':'mV'},{'sensorName':'BATTERY_VIN','value':0.00,'unit':'mV'}]}";

    /**
     * @brief flag for posted data
     * 
     */
    bool payloadPosted = false;
    /**
     * @brief sensor data in json "{'sensorName':'','value': val,'unit': unt},...,{dataN}"
     * 
     */
    String sensorsData="";
    
    /**
     * @brief Construct a new surf Side Science object
     * 
     * @param deviceId String
     */
    surfSideScience(String deviceName){
        deviceName = deviceName;
    }

    /**
     * @brief sensor stabilize delay before reading in ms
     * 
     */
    long sensorStabilizeDelay=0;
    /**
     * @brief 
     * 
     * @tparam T object type
     * @param sensors objects
     */
    template<typename... sensorType>
    void processSensors(sensorType... sensors...){
        (enableSensor(sensors), ...);
        delay(sensorStabilizeDelay);
        (sampleSensor(sensors), ...);
        (stopSensor(sensors), ...);
    }

    /**
     * @brief enable sensor
     * 
     * @tparam T object type
     * @param sensor 
     */
    template <typename sensorType>
    void enableSensor(sensorType sensor){
        sensor.enableSensor();
        if (sensor.sensorStabilizeDelay > sensorStabilizeDelay)
            {
            sensorStabilizeDelay = sensor.sensorStabilizeDelay;
            }

    }

    /**
     * @brief disable sensor
     * 
     * @tparam T 
     * @param sensor 
     */
    template <typename sensorType>
    void stopSensor(sensorType sensor){
        sensor.disableSensor();
    }

    /**
     * @brief read sensordata, log error if any
     * 
     * @tparam T 
     * @param sensor 
     */
    template <typename sensorType>
    void sampleSensor(sensorType sensor){
        float *data;
        sensor.getSamples();
        int numberOfreadings = sensor.numberOfreadings;
        for (int i = 0; i < numberOfreadings; i++){
            if(sensor.sampleStatus[i]== ERROR){
                if(errorBuffer.length() > 0){errorBuffer += ",";}
                errorBuffer += "{sensorName: "+sensor.sensorName[i]+","+sensor.errorBuffer[i]+"}";
            }else{
                if(sensorsData.length() > 0){errorBuffer += ",";}
                errorBuffer += "{sensorName:"+sensor.sensorName[i]+",'value':"+sensor.samplesBuffer[i]+",'unit':"+sensor.unit[i]+"}";
            }
        }
    }

    template <typename modemType>
    int postData(modemType Modem){
        Serial.println("postData");
        Modem.enableModem();
        Modem.establishConnection();
        Modem.getTime();
        generatePayload(Modem.dateTime);

        if (Modem.status == ERROR){
            errorBuffer += "{deviceName: "+Modem.deviceName+","+Modem.errorBuffer+"},";
            payloadPosted = false;
        }else{
            Modem.postData(payload);
            if(Modem.status == ERROR){
                payloadPosted = true;
                errorBuffer += "{: "+Modem.deviceName+","+Modem.errorBuffer+"},";
            }
        }
        Modem.disableModem();
        return 0;
    }

    /**
     * @brief generate payload
     * 
     * @param dateTime String ISO 8601 time
     */

    void generatePayload(String dateTime){
        payload = "{";
        payload += "'deviceName':'"+String(deviceName)+"',";
        payload += "'timestamp':'"+dateTime+"',";
        payload += "'sensors':";
        payload += sensorsData;
        payload += "}";
    }

    template <typename loggerType>
    int log(loggerType logger){
        if(logger.status == -1){return -1;}
        if(!payloadPosted){logger.writeTemp(payload);}
        logger.writeData(payload);
        if(errorBuffer.length() > 0){logger.writeLog(errorBuffer);}
        return 1;
    }
};

