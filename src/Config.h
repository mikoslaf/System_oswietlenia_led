#ifndef CONFIG_H
#define CONFIG_H

// ============== PINY ==============
#define LED_PIN 4
#define LED_COUNT 8
#define PIR_PIN 2
#define LDR_PIN A0
#define BUTTON_PIN 3

// ============== USTAWIENIA ==============

// Automatyka
#define LIGHT_THRESHOLD 700           
#define MOTION_TIMEOUT 10000          
#define USER_ACTION_DISABLE_TIME 5000

// LED - jasność (0-255)
#define DEFAULT_BRIGHTNESS 50

// ============== SIEĆ ==============
#define WIFI_SSID ""
#define WIFI_PASS ""

// ============== DEBUG ==============
#define SERIAL_BAUD 9600

#endif