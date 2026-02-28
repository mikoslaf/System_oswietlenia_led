#ifndef AUTOMATION_MANAGER_H
#define AUTOMATION_MANAGER_H

#include "Config.h"
#include "RuntimeSettings.h"
#include "Arduino.h"

class AutomationManager {
private:
    unsigned long lastMotionTime;
    bool automationEnabled;
    unsigned long userDisabledUntil;
    RuntimeSettings* settings;

public:
    AutomationManager() : lastMotionTime(0), automationEnabled(true), 
                          userDisabledUntil(0), settings(nullptr) {}

    void begin(RuntimeSettings* s) {
        settings = s;
    }

    void enable(bool state) {
        automationEnabled = state;
        if (!state) {
            userDisabledUntil = 0; 
        }
    }

    void userAction() {
        unsigned long disableTime = settings ? settings->userActionDisableTime 
                                             : USER_ACTION_DISABLE_TIME;
        userDisabledUntil = millis() + disableTime;
    }

    bool isUserDisabled() {
        if (userDisabledUntil == 0) return false;
        
        if (millis() > userDisabledUntil) {
            userDisabledUntil = 0;
            return false;
        }
        return true;
    }

    void motionDetected() {
        lastMotionTime = millis();
    }

    bool shouldTurnOff() {
        unsigned long timeout = settings ? settings->motionTimeout : MOTION_TIMEOUT;
        return automationEnabled && (millis() - lastMotionTime > timeout);
    }

    bool isEnabled() { 
        return automationEnabled && !isUserDisabled(); 
    }
    
    unsigned long getRemainingDisableTime() {
        if (userDisabledUntil == 0) return 0;
        unsigned long remaining = (userDisabledUntil - millis()) / 1000;
        return (userDisabledUntil > millis()) ? remaining : 0;
    }
    
    unsigned long getMotionTimeout() {
        return settings ? settings->motionTimeout : MOTION_TIMEOUT;
    }
    
    unsigned long getUserActionDisableTime() {
        return settings ? settings->userActionDisableTime : USER_ACTION_DISABLE_TIME;
    }
};

#endif