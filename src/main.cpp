#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(100);

    Serial.println();
    Serial.println("ESP-01 ready");
}

void loop() {
}
