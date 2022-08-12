#ifndef SHT31_SSS_H
#define SHT31_SSS_H
#include "Arduino.h"
#include "Wire.h"
#include "SHT31.h"

#define SHT31_ADDRESS   0x44

enum StatusCodes
{
    SUCCESS = 1,
    ERROR_CONNECTION = -1,
    ERROR_MEASUREMENT = -2
};

#endif

