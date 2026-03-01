#ifndef RUNTIME_SETTINGS_H
#define RUNTIME_SETTINGS_H

#include "Config.h"
#include <Arduino.h>

struct RuntimeSettings {
    // Ustawienia automatyki
    int lightThreshold;
    unsigned long motionTimeout;
    unsigned long userActionDisableTime;
    
    // Ustawienia LED
    uint8_t defaultBrightness;
    
    RuntimeSettings() {
        reset();
    }
    
    void reset() {
        lightThreshold = LIGHT_THRESHOLD;
        motionTimeout = MOTION_TIMEOUT;
        userActionDisableTime = USER_ACTION_DISABLE_TIME;
        defaultBrightness = DEFAULT_BRIGHTNESS;
    }
    
    void printSettings() {
        Serial.println("=== Current Settings ===");
        Serial.println("Light threshold: " + String(lightThreshold));
        Serial.println("Motion timeout: " + String(motionTimeout/1000) + "s");
        Serial.println("User disable time: " + String(userActionDisableTime/1000) + "s");
        Serial.println("Brightness: " + String(defaultBrightness));
        Serial.println("========================");
    }
};

#endif