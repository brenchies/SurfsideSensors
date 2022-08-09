#include <Arduino.h>
#include <cstdarg>
#include <stdio.h>
#ifndef SURFSIDESCIENCE_H
#define SURFSIDESCIENCE_H
#endif


// #include <surfscienceenums.h>

#define SUCCESS 1
#define ERROR -1



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
     * @brief payload
     * 
     */
    String SDtempData="";
    /**
     * @brief payload = {
     *                  'deviceName':'id',
     *                  'timestamp':'ISO 8601',
     *                  'sensors': {sensor data}
     *                   }     * 
     */
    String payload="";
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
    template<typename... T>
    void processSensors(T... sensors...){
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
    template <typename T>
    void enableSensor(T sensor){
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
    template <typename T>
    void stopSensor(T sensor){
        sensor.disableSensor();
    }

    /**
     * @brief read sensor copy error data to buffer
     * 
     * @tparam T 
     * @param sensor 
     */
    template <typename T>
    void sampleSensor(T sensor){
        float *data;
        sensor.getSample(&data);
        if (sensor.status == ERROR){
            errorBuffer += "{sensorName: "+sensor.sensorName+","+sensor.errorBuffer+"},";
        }

    }

    template <typename T>
    int uploadData(T modem){
        modem.enableModem();
        modem.begin();
        modem.establishConnection();
        modem.getTime();
        generatePayload(modem.dateTime);

        if (modem.status == ERROR){
            errorBuffer += "{deviceName: "+modem.deviceName+","+modem.errorBuffer+"},";
        }else{
            modem.uploadData(payload);
            if(modem.status == ERROR){
                errorBuffer += "{deviceName: "+modem.deviceName+","+modem.errorBuffer+"},";
            }
        }
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
};

