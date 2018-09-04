#include "dht11.h"

#define DHT11_START_TRANSMISSION_DELAY 18000
#define DHT11_PULL_UP_DELAY 40
#define DHT11_BIT_READ_TIMEOUT 10000

int dht11::read(int pin) {
    uint8_t bits[5] = {0, 0, 0, 0, 0};
    uint8_t cnt = 7;
    uint8_t idx = 0;

    pinMode(pin, OUTPUT);

    digitalWrite(pin, LOW);
    delay(DHT11_START_TRANSMISSION_DELAY);

    digitalWrite(pin, HIGH);
    delayMicroseconds(DHT11_PULL_UP_DELAY);

    pinMode(pin, INPUT);

    unsigned int loopCnt = DHT11_BIT_READ_TIMEOUT;
    while (digitalRead(pin) == LOW) {
        if (loopCnt-- == 0) {
            return DHTLIB_ERROR_TIMEOUT;
        }
    }

    loopCnt = DHT11_BIT_READ_TIMEOUT;
    while (digitalRead(pin) == HIGH) {
        if (loopCnt-- == 0) {
            return DHTLIB_ERROR_TIMEOUT;
        }
    }

    for (int i = 0; i < 40; i++) {
        loopCnt = DHT11_BIT_READ_TIMEOUT;

        while (digitalRead(pin) == LOW) {
            if (loopCnt-- == 0) {
                return DHTLIB_ERROR_TIMEOUT;
            }
        }

        unsigned long t = micros();

        loopCnt = DHT11_BIT_READ_TIMEOUT;

        while (digitalRead(pin) == HIGH) {
            if (loopCnt-- == 0) {
                return DHTLIB_ERROR_TIMEOUT;
            }
        }

        if ((micros() - t) > 40) {
            bits[idx] |= (1 << cnt);
        }

        if (cnt == 0) {
            cnt = 7;
            idx++;
        } else {
            cnt--;
        }
    }

    humidity = bits[0];
    temperature = bits[2];

    uint8_t sum = bits[0] + bits[2];

    if (bits[4] != sum) {
        return DHTLIB_ERROR_CHECKSUM;
    }

    return DHTLIB_OK;
}