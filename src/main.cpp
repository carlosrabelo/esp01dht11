#include <ESP8266WiFi.h>
#include "dht11.h"
#include "secret.h"

dht11 dht_sensor;

const char *host = "api.thingspeak.com";

const int dht_pin = 2;

void setup()
{
    Serial.begin(115200);
    delay(10);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.print("WiFi connesso a ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (dht_sensor.read(dht_pin) != DHTLIB_OK)
    {
        Serial.println("Sensor error");
        delay(500);
    }
    else
    {
        String url = "http://api.thingspeak.com/update";

        url += "?api_key=";
        url += privateKey;
        url += "&field1=";
        url += dht_sensor.temperature;
        url += "&field2=";
        url += dht_sensor.humidity;

        Serial.println("send get request ");
        Serial.println(url);

        WiFiClient client;

        if (!client.connect(host, 80))
        {
            Serial.println("connection failed");
            return;
        }

        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");

        unsigned long timeout = millis();

        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                Serial.println(">>> Timeout!");
                client.stop();
                return;
            }
        }

        delay(60000);
    }
}
