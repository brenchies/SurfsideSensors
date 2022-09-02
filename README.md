# Project scope
The idea behind this project was to create a sensor module that is easy to replicate and upgraded.
The easiest way to achieve this was to have an object oriented appreach, in which everything from the sesnors to the communiction devices are defined as an object.
This meant that we had deal with generic types and in case of some cases even variadic generic types, which a combination that you don't generally come accross.
To solve this, templates and ellipses were used, during the creation of the surfsidescience class which is the class that handles the generic types. \

We wanted this project to not be bount to a specific microcontroller so we prevented making MCU specific code part of the surfsidescience class. But instead when needed MCU specif code was written in the main file that is easilly accessible.
MCU specifc classes that are used are the sleep function and the watchdog timer that are specific to the ESP32. This project was based on the LilyGO-T-SIM7000G, for more information about this board please consult their github repo: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G.

## The surfsidescience class has three main objects

template<typename... sensorType> \
void processSensors(sensorType&... sensors) \
This method handles the sensors in the followwing way-> \
- turn sesnors on
- read the sensors
- turn off the sensors
- collect errors from the sensors

template <typename modemType> \
int postData(modemType Modem, bool reportRSSI=true) \ 
This method handles the modem in the following way: \
- turn on modem
- establish internet connection
- get network time
- post data
- disable modem
- collect errors from modem

template <typename loggerType> \
int log(loggerType logger) \
This method handles the logger in the following way: \
- write data to temporary file if data was not uploaded
- write data to data file
- write error to error file

# Writing a sensor class
Writing a class can be done by using the inheriting the sensorBase class and overwriting the virtual functions:
- int enableSensorsImpl(int *sensorstatus)
- int disableSensorsImpl(int *sensorstatus)
- int calibrateSensorsImpl(int statusLed, int *sensorstatus)
- int readSensorImpl(float *buffer, int *sensorstatus, long delay_)

Furthermore the follwing variables have to be modified accroding to the users needs
- int ENABLEPIN;
- int averagingSamples;
- bool checkValueInRange;
- unsinged long sensorPwrDelay;
- unsinged long sampleReadDelay;
- bool SENSOR_ENABLE_STATE;
- String sensorName[i];
- String units[i];
- int numberOfreadings;
- unsinged long sensorStabilizeDelay[i];
- int sensorReadingDecimals[i];
- float EXPECTED_VALUE_MIN[i]=low;
- float EXPECTED_VALUE_MAX[i]=max;


#TODO
- standerdize the modem class to a more generic type, since this is currently not structured in a user friendly way
- make a serverHandler method for the surfsidescience class, which will allow more flexibility for server specific accions
- Server class needs to be introduced with at least endpoint for error reporting, datareport, and updates checking endpoint.
- Standerdize logger class 
- have more controll over sleep times, since sleep is just set to certain amount of seconds which means that the sample upload time will drift over time.
- find a way to introduce a more reliable time source since the time is network dependent so when the network is unreachable, there will be no timestamp with the stored temporary data
- the TinyGSM library is being, during the posting process if the server cannot be reached, the loop will hang there forever, currently this bug is pached with the use of a watchdog timer that has a timout of 10 minutes.
- Simplify the amount of variables of the sesnorBase class. Currently a lot is done for the user in exchange at the cost of them requirening so many input variables to the sesnorBase class.
