#include <Arduino.h>
#include "PMS.h"
#include "SoftwareSerial.h"

#define SAMPLES 5
#define mS_TO_S_FACTOR 1000  /* Conversion factor for micro seconds to seconds */
#define ESP_SLEEP_INTERVAL_IN_MIN(x) (x * 60 * mS_TO_S_FACTOR)   
#define PMS_STARTUP_DELAY_IN_SEC(x) delay((x * mS_TO_S_FACTOR))  

#define TERMINAL_OUT Serial //For debugging on serial monitor
const int RESET = 13;

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
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);

  TERMINAL_OUT.begin(9600);
  TERMINAL_OUT.println("plug in sensors");
  delay(5000);
  TERMINAL_OUT.println("Initializing...");

  #ifdef PMS1
  Serial_PM1.begin(9600, SWSERIAL_8N1, RX1, TX1, false, 192);
  #endif

  #ifdef PMS2
  Serial_PM2.begin(9600, SWSERIAL_8N1, RX2, TX2, false, 192);
  #endif
  
  #ifdef PMS1
  //Switch to passive mode.
  pms1.passiveMode();
  pms1.wakeUp();
  PMS_STARTUP_DELAY_IN_SEC(30);
  readData(&pms1, &Serial_PM1);
  pms1.sleep();
  #endif
 
  #ifdef PMS2
  pms2.passiveMode();
  pms2.wakeUp();
  readData(&pms2, &Serial_PM2);
  pms2.sleep();
  #endif

//Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
//Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
 
TERMINAL_OUT.println("Going to sleep now in 5s...");
digitalWrite(RESET, LOW);
delay(5000);
ESP.deepSleep(10000);
}

void loop()
{
}

/**
 * @brief While UART connection between ESP32 and PMS sensor is stable.
 * A " read request " is sent to the PMS sensor. The PMS sensor then sends
 * data containing particle measurements, and this gets printed on to the serial terminal monitor.
 * 
 * @param pms selected PMS sensor object
 * @param Serial_PM selected SoftwareSerial port object
 */
void readData(PMS *pms, SoftwareSerial *Serial_PM)
{
  PMS::DATA data;
  //Clear buffer (does a burst read to remove potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial_PM->available()) { Serial_PM->read(); }
  TERMINAL_OUT.println("Send read request...");
  pms->requestRead();

  TERMINAL_OUT.println("Reading data...");
  if (pms->readUntil(data, SAMPLES))
  {
    TERMINAL_OUT.println();
    TERMINAL_OUT.println(pms->getName());
    TERMINAL_OUT.print("PM 1.0 (ug/m3): ");
    TERMINAL_OUT.println(data.PM_AE_UG_1_0);
    TERMINAL_OUT.print("PM 2.5 (ug/m3): ");
    TERMINAL_OUT.println(data.PM_AE_UG_2_5);
    TERMINAL_OUT.print("PM 10.0 (ug/m3): ");
    TERMINAL_OUT.println(data.PM_AE_UG_10_0);

    TERMINAL_OUT.println();
  }
  else
  {
    TERMINAL_OUT.println("No data.");
  }
}