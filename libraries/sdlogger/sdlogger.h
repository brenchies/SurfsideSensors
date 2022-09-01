#ifndef SDLOGGER_H
#define SDLOGGER_H
#endif
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


class sdlogger{
    public:
    //SPI pins
    int SD_MISO = 2;
    int SD_MOSI = 15;
    int SD_SCLK = 14;
    int SD_CS = 13;
    int LED_PIN = 12;
    bool isVolumeMounted = false;

    /**
     * @brief SD file
     * 
     */
    File myFile;

    /**
     * @brief File size in MB;
     * 
     */
    long maxFileSize = 1024;

    String DATAFILENAME = "";
    String ERRORLOGFILENAME = "";
    String TEMPDATAFILENAME = "";

    /**
     * @brief sd status
     * 
     */
    int status = 1;

    /**
     * @brief error
     * 
     */
    String errorBuffer = "";
    /**
     * @brief begin sdcard
     * 
     * @param miso 
     * @param mosi 
     * @param sclk 
     * @param cs 
     * @param filePrefix 
     * @param dataFileName 
     * @param errorFileName 
     * @param tempFileName 
     */
    void begin(int miso=2, int mosi=15, int sclk=14, int cs=13, String filePrefix="WATERQ01", String dataFileName="samples.txt", String errorFileName="errorLog.txt", String tempFileName="temp.txt"){
        SD_MISO = miso;
        SD_MOSI = mosi;
        SD_SCLK = sclk;
        SD_CS = cs;

        DATAFILENAME = filePrefix +"_"+ dataFileName;
        ERRORLOGFILENAME = filePrefix +"_"+ errorFileName;
        TEMPDATAFILENAME = filePrefix +"_"+ tempFileName;

        SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        if (!SD.begin(SD_CS)) {
            Serial.println("SDCard MOUNT FAIL");
            status = -1;
        } else {
            uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            String str = "SDCard Size: " + String(cardSize) + "MB";
            Serial.println(str);
            isVolumeMounted = true;
            status = 1;
        }
    }

    int writeToSD(String data, String fileName){
    if (isVolumeMounted){
      Serial.println("writing to sd card");
      myFile = SD.open("/"+fileName, FILE_APPEND);
      myFile.println(data);
      myFile.close();
      Serial.println(data);
    }else{
      return -1;
    }
    return 1;
  }

  /**
   * @brief Write to sd temp file
   * 
   * @param data 
   * @return int 
   */
    int writeTemp(String data){
        return writeToSD(data, TEMPDATAFILENAME);
    }
    int readTemp(String data, bool delLine){
      // delete line and return temp data line if available;
    return 1;
    }

  /**
   * @brief write to sd error log
   * 
   * @param data 
   * @return int 
   */
    int writeLog(String data){
      // delete line and return temp data line if available;
    return writeToSD(data, ERRORLOGFILENAME);
    }

    /**
     * @brief write to sample data file
     * 
     * @param data 
     * @return int 
     */
    int writeData(String data){
      // delete line and return temp data line if available;
    return writeToSD(data, DATAFILENAME);
    }
};