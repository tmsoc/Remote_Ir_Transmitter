#include <Arduino.h>

bool ledOn = true;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, ledOn);
    ledOn = !ledOn;
    delay(1000);
}