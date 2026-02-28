#include <Arduino.h>

#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

class LightSensor {
private:
    int pin;

public:
    LightSensor(int p) : pin(p) {
        pinMode(p, OUTPUT);
    }

    int readValue() {
        return analogRead(pin);
    }

    bool isDark(int threshold) {
        return readValue() < threshold;
    }

    bool isBright(int threshold) {
        return readValue() > threshold;
    }
};

#endif