#include <Arduino.h>
#include <cstdarg>
#include <stdio.h>
#ifndef SURFSIDESCIENCE_H
    #define SURFSIDESCIENCE_H


    int SUCCESS = 1;
    int ERROR = -1;

    class surfSideScience{
        public:
        /**
         * @brief 
         * 
         */
        String deviceName="NO_ID";

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
        String payload="";
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
        surfSideScience(String devicename){
            deviceName = devicename;
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
        void processSensors(sensorType&... sensors)
        {
            Serial.println("enabling sensors");
            (enableSensor(sensors),...);
            Serial.println("delaying: "+String(sensorStabilizeDelay));
            delay(sensorStabilizeDelay);
            Serial.println("sampling sensors");
            (sampleSensor(sensors), ...);
            Serial.println("disabling sensors");
            (stopSensor(sensors), ...);
            Serial.println("errors: "+String(errorBuffer));
            Serial.println("samples: "+String(sensorsData));
        }

        /**
         * @brief enable sensor
         * 
         * @tparam T object type
         * @param sensor 
         */
        template <typename sensorType>
        void enableSensor(sensorType sensor){
            sensor.enableSensors();
            int numberOfreadings = sensor.numberOfreadings;
            for(int i=0; i < numberOfreadings; i++)
            {
                if (sensor.status != SUCCESS)
                {
                    processErrorBuffer("{'sensorName':'"+sensor.sensorName[i]+"','"+sensor.errorBuffer[i]+"'}");
                    sensor.errorBuffer[i] = "";
                }

                if (sensor.sensorStabilizeDelay[i] > sensorStabilizeDelay)
                {
                sensorStabilizeDelay = sensor.sensorStabilizeDelay[i];
                }
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
            sensor.disableSensors();
            int numberOfreadings = sensor.numberOfreadings;
            for(int i=0; i < numberOfreadings; i++)
            {
                if (sensor.sensorStatus[i] != SUCCESS)
                {
                    processErrorBuffer("{'sensorName':'"+sensor.sensorName[i]+"','"+sensor.errorBuffer[i]+"'}");
                    sensor.errorBuffer[i] = "";
                }
            }
        }

        /**
         * @brief append error to buffer
         * 
         * @param cause 
         */
        void processErrorBuffer(String cause){
            if(errorBuffer.length() > 0){errorBuffer += ",";}
            errorBuffer += cause;
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
                if(sensor.sensorStatus[i] != SUCCESS){
                    processErrorBuffer("{'sensorName': '"+sensor.sensorName[i]+"', 'error': '"+sensor.errorBuffer[i]+"'}");
                    sensor.errorBuffer[i] = "";
                }else{
                    if(sensorsData.length() > 0){sensorsData += ",";}
                    sensorsData += "{'sensorName':'"+sensor.sensorName[i]+"','value':"+sensor.samplesBuffer[i]+",'unit':'"+sensor.units[i]+"'}";
                }
            }
        }

        template <typename modemType>
        int postData(modemType Modem, bool reportRSSI=true){
            Serial.println("postData");
            Modem.enableModem();
            Modem.establishConnection();
            Modem.getTime();
            generatePayload(Modem.dateTime);
            Serial.println(payload);

            if (Modem.status == ERROR){
                processErrorBuffer("{'deviceName': '"+Modem.deviceName+"', 'error':"+Modem.errorBuffer+"}");
                payloadPosted = false;
            }else{
                if(reportRSSI)
                {
                    int rssi = Modem.getSignalQuality();
                    if(sensorsData.length() > 0){sensorsData += ",";}
                    sensorsData += "{'sensorName':'RSSI','value':"+String(rssi)+",'unit':'NAN'}";
                }
                
                Modem.postData(payload);
                if(Modem.status == ERROR){
                    payloadPosted = false;
                    processErrorBuffer("{'deviceName': '"+Modem.deviceName+"', 'error':"+Modem.errorBuffer+"}");
                }else{
                    payloadPosted = true;
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
            payload += "'sensors':[";
            payload += sensorsData;
            payload += "]}";
            payload.replace("'", String('"'));
        }

        template <typename loggerType>
        int log(loggerType logger){
            Serial.println("logger status: "+String(logger.status));
            if(logger.status == -1){return -1;}
            if(!payloadPosted){logger.writeTemp(payload);}
            logger.writeData(payload);
            if(errorBuffer.length() > 0){logger.writeLog(errorBuffer);}
            return 1;
        }
    };

#endif