// #include "tinygsmwrapper.h"



// void TinyGSMWrapper::setup()
//     {
//         HttpClient http(client, SERVER, PORT);
//         SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
//         Serial.begin(UART_BAUD);
//         // this->modemPowerOn();
//   //       delay(3000);
//   //       Serial.println("modem ini starting");
//   //       debugger.println("AT+GMR");
//   //       // TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
//   //       modem.restart();
//   //       String modemInfo = modem.getModemInfo();
//   //       SerialMon.print("Modem Info: ");
//   //       SerialMon.println(modemInfo);
//   //       Serial.println("modem ini done");
//   //       debugger.println("AT+GMR=?");
        

//   //       SerialMon.print(F("Connecting to "));
//   //       SerialMon.print(APN);
//   //       if (!modem.gprsConnect(APN, GPRSUSER, GPRSPASS)) {
//   //           SerialMon.println(" fail");
//   //           delay(10000);
//   //           return;
//   //       }
//   //       SerialMon.println(" success");

//   //       if (modem.isGprsConnected()) { SerialMon.println("GPRS connected"); }

//   //       SerialMon.print(F("Performing HTTP GET request... "));
//   //       int err = http.get(RESOURCE);
//   //       if (err != 0) {
//   //           SerialMon.println(F("failed to connect"));
//   //           delay(10000);
//   //           return;
//   //       }

//   //       int status = http.responseStatusCode();
//   // SerialMon.print(F("Response status code: "));
//   // SerialMon.println(status);
//   // if (!status) {
//   //   delay(10000);
//   //   return;
//   // }

//   // SerialMon.println(F("Response Headers:"));
//   // while (http.headerAvailable()) {
//   //   String headerName  = http.readHeaderName();
//   //   String headerValue = http.readHeaderValue();
//   //   SerialMon.println("    " + headerName + " : " + headerValue);
//   // }

//   // int length = http.contentLength();
//   // if (length >= 0) {
//   //   SerialMon.print(F("Content length is: "));
//   //   SerialMon.println(length);
//   // }
//   // if (http.isResponseChunked()) {
//   //   SerialMon.println(F("The response is chunked"));
//   // }

//   // String body = http.responseBody();
//   // SerialMon.println(F("Response:"));
//   // SerialMon.println(body);

//   // SerialMon.print(F("Body length is: "));
//   // SerialMon.println(body.length());


    
//     }

//  void TinyGSMWrapper::modemPowerOn()
//     {
        
//         Serial.println("pwron");
//         pinMode(PIN_PWR, OUTPUT);
//         digitalWrite(PIN_PWR, LOW);
//         delay(1000);    //Datasheet Ton mintues = 1S
//         digitalWrite(PIN_PWR, HIGH);
//     }

//     void TinyGSMWrapper::modemPowerOff()
//     {
//         pinMode(PIN_PWR, OUTPUT);
//         digitalWrite(PIN_PWR, LOW);
//         delay(1500);    //Datasheet Ton mintues = 1.2S
//         digitalWrite(PIN_PWR, HIGH);
//     }


//     void TinyGSMWrapper::modemRestart()
//     {
//         modemPowerOff();
//         delay(1000);
//         modemPowerOn();
//     }
//     void TinyGSMWrapper::awake()
//     {
//         modemPowerOn();
//     }

//     void TinyGSMWrapper::sleep()
//     {
//         modemPowerOff();
//     }

//     void TinyGSMWrapper::post(){

//     }

//     void TinyGSMWrapper::request(){

//     }