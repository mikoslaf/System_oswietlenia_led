#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include <FastLED.h>
#include "Config.h"
#include "RuntimeSettings.h"

class LightController {
private:
    CRGB leds[LED_COUNT];
    bool isOn;
    uint8_t brightness;
    RuntimeSettings* settings;

public:
    // W FastLED nie inicjalizujemy paska w konstruktorze, tylko w begin()
    LightController() : settings(nullptr), isOn(false), brightness(DEFAULT_BRIGHTNESS) {}

    void begin(RuntimeSettings* s = nullptr) {
        settings = s;

        // Inicjalizacja FastLED dla chipsetu WS2812B
        // LED_PIN musi być zdefiniowany w Config.h jako stała (np. #define LED_PIN 1)
        FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
        
        uint8_t defaultBright = settings ? settings->defaultBrightness : DEFAULT_BRIGHTNESS;
        FastLED.setBrightness(defaultBright);
        
        // Wyczyszczenie paska na starcie
        turnOff();
        
        isOn = false;
        brightness = defaultBright;
    }

    void turnOn(uint8_t r, uint8_t g, uint8_t b) {
        // fill_solid to bardzo szybka metoda FastLED na ustawienie całego paska
        fill_solid(leds, LED_COUNT, CRGB(r, g, b));
        FastLED.show();
        isOn = true;
    }
    
    void turnOnDefault() {
        uint8_t r = settings ? settings->defaultColorR : DEFAULT_COLOR_R;
        uint8_t g = settings ? settings->defaultColorG : DEFAULT_COLOR_G;
        uint8_t b = settings ? settings->defaultColorB : DEFAULT_COLOR_B;
        turnOn(r, g, b);
    }
    
    void turnOnMotion() {
        uint8_t r = settings ? settings->motionColorR : MOTION_COLOR_R;
        uint8_t g = settings ? settings->motionColorG : MOTION_COLOR_G;
        uint8_t b = settings ? settings->motionColorB : MOTION_COLOR_B;
        turnOn(r, g, b);
    }

    void turnOff() {
        // CRGB::Black to odpowiednik RGB(0,0,0)
        fill_solid(leds, LED_COUNT, CRGB::Black);
        FastLED.show();
        isOn = false;
    }

    void setBrightness(uint8_t value) {
        brightness = value;
        FastLED.setBrightness(brightness);
        // FastLED automatycznie przelicza jasność przy wywołaniu show()
        FastLED.show();
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