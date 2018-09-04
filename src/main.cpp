#include "dht11.h"

#define DHT_PIN 2
#define READING_INTERVAL 60000

static void printSystemInfo() {
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
    Serial.print("Chip ID: ");
    Serial.println(ESP.getChipId());
    Serial.print("Flash size: ");
    Serial.println(ESP.getFlashChipSize());
}

void setup() {
    Serial.begin(115200);
    delay(100);

    Serial.println();
    Serial.println("=== ESP01 DHT11 Monitor ===");
    printSystemInfo();
}

void loop() {
    Serial.println();
    Serial.println("--- Reading DHT11 sensor ---");

    dht11 dht_sensor;
    int result = dht_sensor.read(DHT_PIN);

    if (result != DHTLIB_OK) {
        Serial.print("DHT11 error: ");
        switch (result) {
            case DHTLIB_ERROR_CHECKSUM:
                Serial.println("Checksum mismatch");
                break;
            case DHTLIB_ERROR_TIMEOUT:
                Serial.println("Timeout waiting for sensor");
                break;
            default:
                Serial.println("Unknown error");
                break;
        }
        delay(5000);
        return;
    }

    int temperature = dht_sensor.temperature;
    int humidity = dht_sensor.humidity;

    if (temperature < 0 || temperature > 50 || humidity > 100) {
        Serial.println("Warning: Invalid sensor reading detected");
        Serial.print("Temp: ");
        Serial.print(temperature);
        Serial.print(", Humidity: ");
        Serial.println(humidity);
        delay(5000);
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    printSystemInfo();
    Serial.print("Next reading in ");
    Serial.print(READING_INTERVAL / 1000);
    Serial.println(" seconds...");
    Serial.println();

    delay(READING_INTERVAL);
}
