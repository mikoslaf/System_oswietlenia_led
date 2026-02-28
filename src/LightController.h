#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "RuntimeSettings.h"

class LightController {
private:
    Adafruit_NeoPixel strip;
    bool isOn;
    uint8_t brightness;
    RuntimeSettings* settings;

public:
    LightController() : strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800), 
                        settings(nullptr), isOn(false), brightness(DEFAULT_BRIGHTNESS) {}

    void begin(RuntimeSettings* s = nullptr) {
        settings = s;
        strip.begin();
        strip.show();
        uint8_t defaultBright = settings ? settings->defaultBrightness : DEFAULT_BRIGHTNESS;
        strip.setBrightness(defaultBright);
        isOn = false;
        brightness = defaultBright;
    }

    void turnOn(uint8_t r, uint8_t g, uint8_t b) {
        for (int i = 0; i < LED_COUNT; i++)
            strip.setPixelColor(i, strip.Color(r, g, b));
        strip.show();
        isOn = true;
    }
    
    // Włącz z domyślnym kolorem z ustawień
    void turnOnDefault() {
        uint8_t r = settings ? settings->defaultColorR : DEFAULT_COLOR_R;
        uint8_t g = settings ? settings->defaultColorG : DEFAULT_COLOR_G;
        uint8_t b = settings ? settings->defaultColorB : DEFAULT_COLOR_B;
        turnOn(r, g, b);
    }
    
    // Włącz z kolorem dla wykrycia ruchu
    void turnOnMotion() {
        uint8_t r = settings ? settings->motionColorR : MOTION_COLOR_R;
        uint8_t g = settings ? settings->motionColorG : MOTION_COLOR_G;
        uint8_t b = settings ? settings->motionColorB : MOTION_COLOR_B;
        turnOn(r, g, b);
    }

    void turnOff() {
        strip.clear();
        strip.show();
        isOn = false;
    }

    void setBrightness(uint8_t value) {
        brightness = value;
        strip.setBrightness(brightness);
        if (isOn) strip.show();
    }
    
    void applySettingsBrightness() {
        if (settings) {
            setBrightness(settings->defaultBrightness);
        }
    }

    bool getState() { return isOn; }
    uint8_t getBrightness() { return brightness; }
};

#endif