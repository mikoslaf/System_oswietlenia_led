#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <Arduino.h>

class MotionSensor {
private:
    int pin;
    int pirState;

public:
    MotionSensor(int p) : pin(p), pirState(LOW) {}

    void begin() {
        pinMode(pin, INPUT);
    }

    bool motionStarted() {
        int motion = digitalRead(pin);
        bool started = false;
        if (motion == HIGH) {
            if (pirState == LOW) {
                pirState = HIGH;
                started = true;
            }
        } else {
            if (pirState == HIGH) {
                pirState = LOW;
            }
        }
        return started;
    }

    bool motionEnded() {
        int motion = digitalRead(pin);
        bool ended = false;
        if (motion == LOW) {
            if (pirState == HIGH) {
                pirState = LOW;
                ended = true;
            }
        } else {
            if (pirState == LOW) {
                pirState = HIGH;
            }
        }
        return ended;
    }

    bool isActive() {
        return digitalRead(pin) == HIGH;
    }
    
    bool detected() {
        return motionStarted();
    }
};

#endif