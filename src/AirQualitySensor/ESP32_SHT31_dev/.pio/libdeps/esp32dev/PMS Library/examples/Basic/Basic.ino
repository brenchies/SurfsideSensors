#include "PMS.h"
#include "SoftwareSerial.h"
#define SAMPLES 5
// PMS pms(Serial);
// PMS::DATA data;

#define PMS1
#define PMS2
#ifdef PMS1
//Set ESP32 pins to receive/transfer data
#define TX1 32 //GPIO32
#define RX1 33 //GPIO33
//Creation of espSoftwareSerial objects
SoftwareSerial Serial_PM1;
//PM Sensor names
String name_pms1 = "PMS 1";
//Creation of PM sensor objects
PMS pms1(Serial_PM1, &name_pms1);
#endif

#ifdef PMS2 //Uncomment PMS2 if you are using an second PMS sensor
//Set ESP32 pins to receive/transfer data
#define TX2 34 //GPIO34
#define RX2 35 //GPIO35
//Creation of espSoftwareSerial objects
SoftwareSerial Serial_PM2;
//PM Sensor names
String name_pms2 = "PMS 2";
//Creation of PM sensor objects
PMS pms2(Serial_PM2, &name_pms2);
#endif


void readData(PMS*, SoftwareSerial*);
void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  #ifdef PMS1
  Serial_PM1.begin(9600, SWSERIAL_8N1, RX1, TX1, false, 192);
  #endif

  #ifdef PMS2
  Serial_PM2.begin(9600, SWSERIAL_8N1, RX2, TX2, false, 192);
  #endif
}

void loop()
{
  delay(30000);
  readData(&pms1, &Serial_PM1);
  readData(&pms2, &Serial_PM2);
  // Do other stuff...
}

void readData(PMS *pms, SoftwareSerial *Serial_PM)
{
  PMS::DATA data;
  //Clear buffer (does a burst read to remove potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial_PM->available()) { Serial_PM->read(); }
  Serial.println("Send read request...");
  pms->requestRead();

  Serial.println("Reading data...");
  if (pms->readUntil(data, SAMPLES))
  {
    Serial.println();
    Serial.println(pms->getName());
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);
    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);
    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  }
  else
  {
    Serial.println("No data.");
  }
}