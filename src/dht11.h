#ifndef DHT11_H
#define DHT11_H

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#endif

#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK 0
#define DHTLIB_ERROR_CHECKSUM -1
#define DHTLIB_ERROR_TIMEOUT -2

class dht11 {
public:
    int read(int pin);
    int humidity;
    int temperature;
};

#endif