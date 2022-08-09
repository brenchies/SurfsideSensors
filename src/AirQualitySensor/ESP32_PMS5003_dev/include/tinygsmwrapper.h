#ifndef TINYGSMWRAPPER_H
#define TINYGSMWRAPPER_H
#endif
#include <Arduino.h>
#include <HardwareSerial.h>
// src: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G
// https://github.com/vshymanskyy/TinyGSM/blob/master/examples/HttpClient/HttpClient.ino

#define TINY_GSM_MODEM_SIM7000
// tinygsm
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

#include <TinyGSM.h>
#include <ArduinoHttpClient.h>


#define SerialMon Serial
#define SerialAT Serial1
#define TinyGSMDEBUG
#ifndef TinyGSMDEBUG
    TinyGsm modem(SerialAT);
    TinyGsmClient client(modem);
#else
  #include <StreamDebugger.h>
    StreamDebugger debugger(SerialAT, SerialMon);
    TinyGsm modem(debugger);
    TinyGsmClient client(modem);
#endif
#define TINY_GSM_RX_BUFFER 650


class TinyGSMWrapper{
    public:
    long UART_BAUD = 115200;
    long PIN_DTR  = 25;
    long PIN_TX   = 27;
    long PIN_RX   = 26;
    long PIN_PWR  = 4;

    // Your GPRS credentials, if any
    const char *APN   = "web.digicelaruba.com";
    const char *GPRSUSER = "";
    const char *GPRSPASS = "";
    const char *SERVER = "";
    const char *GSMPIN = "";
    long PORT = 80;
    const char *RESOURCE = "";

    //gprs
    bool gprsReady = false;


    TinyGSMWrapper(const char* server="www.google.com", const char* resource="/", const char* apn="web.digicelaruba.com", const char* gprsuser="",\
     const char* gprspass="", long uart_baud=115200, long pin_dtr=25, long pin_tx = 27, long pin_rx=26, long pin_pwr=4, long port=80)
    {
        UART_BAUD = uart_baud;
        PIN_DTR = pin_dtr;
        pinMode(PIN_DTR, OUTPUT);
        digitalWrite(PIN_DTR, LOW);
        PIN_TX = pin_tx;
        PIN_RX = pin_rx;
        PIN_PWR = pin_pwr;
        pinMode(PIN_PWR, OUTPUT);
        digitalWrite(PIN_PWR, LOW);
        SERVER = server;
        APN = apn;
        GPRSPASS = gprspass;
        GPRSUSER = gprsuser;
        PORT = port;
        RESOURCE = resource;


        SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
    }
    
    int begingprs(int trials=2)
    {// setup internet connection
      SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
      Serial.begin(UART_BAUD);
      if (gprsReady){return 1;}
      //  modem.restart();
      // delay(5000);
      // Serial.println("modem ini starting");
      // debugger.println("AT+GMR");
      modemPowerOn();
      // modem.restart();
      delay(5000);
      // String modemInfo = modem.getModemInfo();
      // SerialMon.print("Modem Info: ");
      // SerialMon.println(modemInfo);
      // Serial.println("modem ini done");
      // debugger.println("AT+GMR=?");

      SerialMon.print(F("Connecting to "));
      SerialMon.print(APN);
      for(int i = 0; i < trials; i++){
          if (!modem.gprsConnect(APN, GPRSUSER, GPRSPASS)) {
          SerialMon.println("GPRS connection fail Trial: "+String(i)+"/"+String(trials));
          gprsReady = false;
        }else{
          break;
        }
      }

      if (modem.isGprsConnected()) {
          SerialMon.println("GPRS connected"); 
          gprsReady = true;
          return 1;
      }else{
        gprsReady = false;
        return -1;
      }

    }

    int getSignalQuality(){
      return modem.getSignalQuality();
    }

  int get(const char * server="surfside-db.brenchies.com", long port=80, const char *resource="/observations")
    {
      HttpClient http(client, server, port);
      SerialMon.print(F("Performing HTTP GET request... "));
      int err = http.get(resource);
      if (err != 0) {
          SerialMon.println(F("failed to connect"));
          return -1;
      }

      int status = http.responseStatusCode();
      SerialMon.print(F("Response status code: "));
      SerialMon.println(status);
      if (!status) {
        return -1;
      }

    SerialMon.println(F("Response Headers:"));
    while (http.headerAvailable()) {
      String headerName  = http.readHeaderName();
      String headerValue = http.readHeaderValue();
      SerialMon.println("    " + headerName + " : " + headerValue);
    }

    int length = http.contentLength();
    if (length >= 0) {
      SerialMon.print(F("Content length is: "));
      SerialMon.println(length);
    }
    if (http.isResponseChunked()) {
      SerialMon.println(F("The response is chunked"));
    }

    String body = http.responseBody();
    SerialMon.println(F("Response:"));
    SerialMon.println(body);

    SerialMon.print(F("Body length is: "));
    SerialMon.println(body.length());
      
  }

    int getTime(String* buffer, int hourOffset=8, int minuteOffset=0, int secondsOffset=0){
      //ISO 8601
        int year, month, day, hour, minute, second;
        float timezone;
        TinyGSMDateTimeFormat format = DATE_TIME;
        modem.sendAT("+CNTP=pool.ntp.org,2");
        Serial.println(SerialAT.readString());
        Serial.println(modem.getGSMDateTime(format));
        modem.getNetworkTime(&year, &month, &day, &hour, &minute, &second, &timezone);
        hour = hourOffset > hour? 24 - hourOffset + hour: hour - hourOffset;
        minute = minuteOffset > minute? 60 - minuteOffset + minute: minute - minuteOffset;
        second = secondsOffset > second? 60 - secondsOffset + second: second - secondsOffset;
        String timezoneStr = String(timezone);
        timezoneStr.replace(".", ":");
        String datetime = String(year)+"-"+\
            String(month < 10? "0"+String(month):String(month))+"-"+\
            String(day < 10? "0"+String(day):String(day))+" "+\
            String(hour < 10? "0"+String(hour):String(hour))+":"+\
            String(minute < 10? "0"+String(minute):String(minute))+":"+\
            String(second < 10? "0"+String(second):String(second))+".000"+\
            String(timezone > 0?"+"+timezoneStr:timezoneStr);
        Serial.println(datetime);
        *buffer = datetime;
        return 1;
    }

    long post(String server="surfside-db.brenchies.com", long port=80, String path="/observations", String payload="{'hello': 0}", String contentType="application/json", long successCode=201)
    {//todo handle error if modem not connected, if server not connected etc

      if (!gprsReady){
        begingprs();
        if(!gprsReady){
          return -1;
        }
      }
        HttpClient http(client, server, port);
        SerialMon.print(F("Performing HTTP POST request... "));
        int err = http.post(path, contentType, payload);
        if (err != 0) {
            SerialMon.println(F("failed to connect"));
            SerialMon.println(err);
            return -1;
        }
        int status = http.responseStatusCode();
    SerialMon.print(F("Response status code: "));
    SerialMon.println(status);
    if (!status) {
      return -1;
    }

    SerialMon.println(F("Response Headers:"));
    while (http.headerAvailable()) {
      String headerName  = http.readHeaderName();
      String headerValue = http.readHeaderValue();
      SerialMon.println("    " + headerName + " : " + headerValue);
    }

    int length = http.contentLength();
    if (length >= 0) {
      SerialMon.print(F("Content length is: "));
      SerialMon.println(length);
    }
    if (http.isResponseChunked()) {
      SerialMon.println(F("The response is chunked"));
    }

    String body = http.responseBody();
    SerialMon.println(F("Response:"));
    SerialMon.println(body);

    SerialMon.print(F("Body length is: "));
    SerialMon.println(body.length());
    return status != successCode ? -1: 1;
    } 

  void gprsDisconnect(){
    int status = modem.gprsDisconnect();
    SerialMon.println("GPRS disconnect: "+String(status));
  }   

    
   void modemPowerOn()
{
    pinMode(PIN_PWR, OUTPUT);
    digitalWrite(PIN_PWR, HIGH);
    delay(1500);    //Datasheet Ton mintues = 1.2S
    digitalWrite(PIN_PWR, LOW);
    delay(1500);    //Datasheet Ton mintues = 1.2S
    digitalWrite(PIN_PWR, HIGH);
    delay(5000);
}

void modemPowerOff()
{
    pinMode(PIN_PWR, OUTPUT);
    digitalWrite(PIN_PWR, LOW);
    delay(1500);    //Datasheet Ton mintues = 1.2S
    digitalWrite(PIN_PWR, HIGH);
    delay(1500);    //Datasheet Ton mintues = 1.2S
    digitalWrite(PIN_PWR, LOW);

}

    void modemRestart()
    {
        modemPowerOff();
        delay(1000);
        modemPowerOn();
    }
};