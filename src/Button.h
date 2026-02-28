#include <Arduino.h>

#ifndef BUTTON_H
#define BUTTON_H

class Button {
private:
    int pin;
    bool lastState;

public:
    Button(int p) : pin(p), lastState(false) {}

    void begin() {
        pinMode(pin, INPUT_PULLUP);
    }

    bool pressed() {
        bool state = !digitalRead(pin);
        if (state && !lastState) {
            lastState = true;
            return true;
        }
        if (!state) lastState = false;
        return false;
    }
};

#endif