#include "Config.h"
#include "RuntimeSettings.h"
#include "Logger.h"
#include "LightController.h"
#include "MotionSensor.h"
#include "LightSensor.h"
#include "Button.h"
#include "WebServerManager.h"
#include "AutomationManager.h"

RuntimeSettings settings;
LightController light;
MotionSensor motion(PIR_PIN);
LightSensor ldr(LDR_PIN);
Button button(BUTTON_PIN);
WebServerManager web;
AutomationManager automation;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    light.begin(&settings);
    motion.begin();
    button.begin();
    web.begin(&settings);
    automation.begin(&settings);
    
    LOG_LIGHT_CHANGE(false, "System startup");
    Serial.println("System initialized. Automation enabled: " + String(automation.isEnabled()));
    settings.printSettings();
}

void loop() {
    // === Obsługa Web ===
    String command = web.handleClient();
    
    if (command == "ON") {
        light.turnOnDefault();
        LOG_LIGHT_CHANGE(true, "Web command ON");
        automation.userAction();
        Serial.println("Automation disabled for " + String(settings.userActionDisableTime/1000) + "s");
    }
    
    if (command == "OFF") {
        light.turnOff();
        LOG_LIGHT_CHANGE(false, "Web command OFF");
        automation.userAction();
    }
    
    if (command == "AUTO") {
        automation.enable(true);
        Serial.println("Automation enabled permanently");
    }
    
    if (command == "MANUAL") {
        automation.enable(false);
        LOG_LIGHT_CHANGE(light.getState(), "Manual mode - automation disabled");
        Serial.println("Automation disabled manually");
    }
    
    if (command == "SETTINGS_SAVED" || command == "SETTINGS_RESET") {
        light.applySettingsBrightness();
        Serial.println("Brightness updated to: " + String(settings.defaultBrightness));
    }

    // === Obsługa przycisku ===
    if (button.pressed()) {
        if (light.getState()) {
            light.turnOff();
            LOG_LIGHT_CHANGE(false, "Button pressed");
        } else {
            light.turnOnDefault();
            LOG_LIGHT_CHANGE(true, "Button pressed");
        }
        automation.userAction(); 
    }

    bool motionStarted = motion.motionStarted();

    // === Automatyka ===
    if (automation.isEnabled()) {
        // Włączenie przy ruchu i ciemności
        if (motionStarted && ldr.isDark(settings.lightThreshold)) {
            light.turnOnMotion();
            LOG_LIGHT_CHANGE(true, "Motion + Dark");
            automation.motionDetected();
        }

        // Wyłączenie po timeout
        if (automation.shouldTurnOff() && light.getState()) {
            light.turnOff();
            LOG_LIGHT_CHANGE(false, "Motion timeout");
        }
    } else {
        static unsigned long lastCheck = 0;
        if (millis() - lastCheck > 5000) {
            lastCheck = millis();
            unsigned long remaining = automation.getRemainingDisableTime();
            if (remaining > 0) {
                Serial.println("Automation paused. Remaining: " + String(remaining) + "s");
            }
        }
    }
}