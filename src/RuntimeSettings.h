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
    uint8_t defaultColorR;
    uint8_t defaultColorG;
    uint8_t defaultColorB;
    
    // Kolor przy wykryciu ruchu
    uint8_t motionColorR;
    uint8_t motionColorG;
    uint8_t motionColorB;
    
    RuntimeSettings() {
        reset();
    }
    
    void reset() {
        lightThreshold = LIGHT_THRESHOLD;
        motionTimeout = MOTION_TIMEOUT;
        userActionDisableTime = USER_ACTION_DISABLE_TIME;
        defaultBrightness = DEFAULT_BRIGHTNESS;
        defaultColorR = DEFAULT_COLOR_R;
        defaultColorG = DEFAULT_COLOR_G;
        defaultColorB = DEFAULT_COLOR_B;
        motionColorR = MOTION_COLOR_R;
        motionColorG = MOTION_COLOR_G;
        motionColorB = MOTION_COLOR_B;
    }
    
    void printSettings() {
        Serial.println("=== Current Settings ===");
        Serial.println("Light threshold: " + String(lightThreshold));
        Serial.println("Motion timeout: " + String(motionTimeout/1000) + "s");
        Serial.println("User disable time: " + String(userActionDisableTime/1000) + "s");
        Serial.println("Brightness: " + String(defaultBrightness));
        Serial.println("Default color: RGB(" + String(defaultColorR) + "," + 
                       String(defaultColorG) + "," + String(defaultColorB) + ")");
        Serial.println("Motion color: RGB(" + String(motionColorR) + "," + 
                       String(motionColorG) + "," + String(motionColorB) + ")");
        Serial.println("========================");
    }
};

#endif