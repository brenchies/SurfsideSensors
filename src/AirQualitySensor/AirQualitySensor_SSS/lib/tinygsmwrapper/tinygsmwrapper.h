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
#include <StreamDebugger.h>

#define SerialMon Serial
#define SerialAT Serial1
#define TinyGSMDEBUG
#ifndef TinyGSMDEBUG
    TinyGsm modem(SerialAT);
    TinyGsmClient client(modem);
#else
    StreamDebugger debugger(SerialAT, SerialMon);
    TinyGsm modem(debugger);
    TinyGsmClient client(modem);
#endif
#define TINY_GSM_RX_BUFFER 650

class TinyGSMWrapper{
  public:
  String errorBuffer = "";
  String deviceName = "";
  int status = 1;
  String dateTime = "";

  //GPIO & communication
  long UART_BAUD = 115200;
  long PIN_DTR  = 25;
  long PIN_TX   = 27;
  long PIN_RX   = 26;
  long PIN_PWR  = 4;
  long modemPwrdelay = 5000;

  // Your GPRS credentials, if any
  const char *APN   = "web.digicelaruba.com";
  const char *GPRSUSER = "";
  const char *GPRSPASS = "";
  const char *GSMPIN = "";
  bool gprsReady = false;

  //server
  const char *SERVER = "surfside-db.brenchies.com";
  const char *POSTPATH = "/observations";
  const char *CONTENTTYPE = "application/json";
  long PORT = 80;
  long SUCCESSCODE = 201;

  /**
   * @brief 
   * 
   * @param apn 
   * @param gprsuser 
   * @param gprspass 
   * @param server 
   * @param postPath 
   * @param successCode 
   * @param contentType 
   * @param uart_baud 
   * @param pin_dtr 
   * @param pin_tx 
   * @param pin_rx 
   * @param pin_pwr 
   */
  void begin(const char* apn="web.digicelaruba.com", const char* gprsuser="", const char* gprspass="",\
    const char* server="surfside-db.brenchies.com", const char* postPath="/observations", long successCode=201,\
    const char* contentType= "application/json", long uart_baud=115200, long pin_dtr=25, long pin_tx = 27,\
    long pin_rx=26, long pin_pwr=4, String devicename="SIMCom SIM7000")
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
      deviceName = devicename;
      APN = apn;
      GPRSPASS = gprspass;
      GPRSUSER = gprsuser;
      POSTPATH = postPath;
      CONTENTTYPE = contentType;
      SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  }

  /**
     * @brief add report to a specific error buffer
     * 
     * @param bufferNr 
     * @param cause 
     */
    void processErrorBuffer(String cause){
        if(errorBuffer.length() > 0){errorBuffer += ",";}
        errorBuffer += "{'error':'"+deviceName+" "+cause+"'}";
    }

  /**
   * @brief check for enable/disable response by trying multiple times
   * 
   * @param response 
   * @param trials 
   * @return int 
   */
  int isModemAlive(bool response=1, int trials=5){
    int success_count = 0;
    for (int i = 0; i < trials; i++){
      modem.sendAT("+CPIN?");
      String receive = SerialAT.readString();
      if(response && receive.length() > 0 ||  !response && receive.length() == 0){ 
        success_count++;
      }else{
        success_count = 0;
      }
    }
    if(success_count != 0){
      return 1;
    }
    return -1;
  }

  void sendPwrPulse(int delay_=1000, bool enable=true){
    if(enable){
      digitalWrite(PIN_PWR, LOW);
      delay(delay_);
      digitalWrite(PIN_PWR, HIGH);
      delay(delay_);    //Datasheet Ton mintues = 1.2S
    }
    
    digitalWrite(PIN_PWR, LOW);
    delay(delay_);    //Datasheet Ton mintues = 1.2S
    digitalWrite(PIN_PWR, HIGH);
    if(!enable){
      delay(delay_);    //Datasheet Ton mintues = 1.2S
      digitalWrite(PIN_PWR, LOW);
    }
    delay(modemPwrdelay);
  }

  /**
   * @brief enable modem check if alive by
   * 
   * @param trials 
   */
  int enableModem(int trials=5){
    for(int i = 0; i < trials; i++){
      Serial.println("enabling modem");
      sendPwrPulse(200, true);
      status = isModemAlive(1);
      if(status == 1){
        Serial.println("enabling success");
        break;
      }
      Serial.println("Enable MODEM failed trials: "+String(i)+"/"+String(trials));
    }
    if (status == -1){
      processErrorBuffer("enable failed trials: "+String(trials));
    }
    return status;
  }

  /**
   * @brief disable modem
   * 
   * @param trials 
   */
  int disableModem(int trials=20){
    for(int i = 0; i < trials; i++){
      Serial.println("disabling modem");
      sendPwrPulse(200, false);

      status = isModemAlive(0);
      Serial.println("disabling success");
      if(status == 1){
        break;
      }
      Serial.println("disable MODEM failed trials: "+String(i)+"/"+String(trials));
    }
    if (status == -1){
      processErrorBuffer("disable failed trials: "+String(trials));
    }
    return status;
  }


  /**
   * @brief Get the Time object
   * 
   * @param trials 
   */
  void getTime(int trials=3){
    for(int i = 0; i < trials; i++){
            //ISO 8601
        int year, month, day, hour, minute, second;
        float timezone;
        TinyGSMDateTimeFormat format = DATE_TIME;
        // modem.sendAT("+CNTP=pool.ntp.org,2");
        Serial.println(SerialAT.readString());
        Serial.println(modem.getGSMDateTime(format));
        modem.getNetworkTime(&year, &month, &day, &hour, &minute, &second, &timezone);
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
        dateTime = datetime;
        break;
    }
  }

  /**
   * @brief Get the Signal Quality object
   * 
   * @return int 
   */
  int getSignalQuality()
  {
    return modem.getSignalQuality();
  }

  /**
   * @brief establish connnection
   * 
   * @param trials 
   * @return int 
   */
  int establishConnection(int trials=3){
      if (gprsReady){return 1;}

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

  /**
   * @brief post data to server
   * 
   * @param payload 
   * @param trials 
   * @return int 
   */
  int postData(String payload, int trials=3){
    for(int i = 0; i < trials; i++)
    {
      if(!gprsReady){establishConnection();}
      HttpClient http(client, SERVER, PORT);
      SerialMon.print(F("Performing HTTP POST request... "));
      int err = http.post(POSTPATH, CONTENTTYPE, payload);
      if (err != 0) {
          SerialMon.println(F("failed to connect"));
          SerialMon.println(err);
          return -1;
      }
      int status_ = http.responseStatusCode();
      SerialMon.print(F("Response status code: "));
      SerialMon.println(status_);
      if (!status_) {
        errorBuffer = "|received error code: "+String(status)+"|";
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
      status = status_ != SUCCESSCODE ? -1: 1;
      if(status != 1){
        errorBuffer = "|incorrect status code: "+String(status)+", expected: "+String(SUCCESSCODE)+"|";
      }
      else 
      {
        Serial.println("Post success trials: "+String(i));
        return status;
      }
    }
    return status;
  }
};