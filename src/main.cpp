#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

#include "dht11.h"
#include "secret.h"

#define HOST "api.thingspeak.com"
#define DHT_PIN 2
#define READING_INTERVAL 60000
#define WIFI_TIMEOUT_MS 30000
#define HTTP_TIMEOUT_MS 10000
#define MAX_RETRIES 3
#define RETRY_DELAY_MS 5000

static ESP8266WiFiMulti wifiMulti;
static bool wifiConnected = false;

static void connectToWiFi() {
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(ssid);

    wifiMulti.addAP(ssid, password);

    unsigned long startAttempt = millis();
    while (wifiMulti.run(WL_CONNECTED) != WL_CONNECTED) {
        if (millis() - startAttempt > WIFI_TIMEOUT_MS) {
            Serial.println("WiFi connection timeout!");
            wifiConnected = false;
            return;
        }
        Serial.print(".");
        delay(500);
    }

    wifiConnected = true;
    Serial.println();
    Serial.print("WiFi connected! IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}

static bool reconnectWiFi() {
    Serial.println("Attempting WiFi reconnection...");
    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        return true;
    }
    connectToWiFi();
    return wifiConnected;
}

static bool sendDataToThingSpeak(int temperature, int humidity, int retries) {
    if (!wifiConnected || WiFi.status() != WL_CONNECTED) {
        if (!reconnectWiFi()) {
            Serial.println("Failed to reconnect to WiFi");
            return false;
        }
    }

    WiFiClient client;
    HTTPClient http;

    String url = "http://" HOST "/update?api_key=";
    url += privateKey;
    url += "&field1=";
    url += temperature;
    url += "&field2=";
    url += humidity;

    Serial.print("Sending to ThingSpeak - Temp: ");
    Serial.print(temperature);
    Serial.print("C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    http.setTimeout(HTTP_TIMEOUT_MS);
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode > 0) {
        String payload = http.getString();
        Serial.print("HTTP ");
        Serial.println(httpCode);
        Serial.print("Response: ");
        Serial.println(payload);
        http.end();

        if (payload.length() > 0 && payload.toInt() == 0) {
            Serial.println("ThingSpeak returned 0 - possible API error");
        }
        return true;
    }

    Serial.print("HTTP Error: ");
    Serial.println(httpCode);
    http.end();

    if (retries > 0) {
        Serial.print("Retrying in ");
        Serial.print(RETRY_DELAY_MS / 1000);
        Serial.println(" seconds...");
        delay(RETRY_DELAY_MS);
        return sendDataToThingSpeak(temperature, humidity, retries - 1);
    }

    return false;
}

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
    Serial.println("=== ESP01 DHT11 ThingSpeak Monitor ===");
    printSystemInfo();

    connectToWiFi();
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

    bool success = sendDataToThingSpeak(temperature, humidity, MAX_RETRIES);

    if (success) {
        Serial.println("Data sent successfully!");
    } else {
        Serial.println("Failed to send data after retries");
    }

    printSystemInfo();
    Serial.print("Next reading in ");
    Serial.print(READING_INTERVAL / 1000);
    Serial.println(" seconds...");
    Serial.println();

    delay(READING_INTERVAL);
}