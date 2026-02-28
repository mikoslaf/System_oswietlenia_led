#ifndef CONFIG_H
#define CONFIG_H

// ============== PINY (nie edytowalne przez web) ==============
#define LED_PIN 4
#define LED_COUNT 8
#define PIR_PIN 2
#define LDR_PIN A0
#define BUTTON_PIN 3

// ============== USTAWIENIA DOMYŚLNE (edytowalne przez web) ==============

// Automatyka
#define LIGHT_THRESHOLD 700           // Próg jasności (0-1023)
#define MOTION_TIMEOUT 10000          // Timeout ruchu w ms
#define USER_ACTION_DISABLE_TIME 5000 // Blokada automatyki po akcji użytkownika

// LED - jasność domyślna (0-255)
#define DEFAULT_BRIGHTNESS 50

// Kolor domyślny (biały)
#define DEFAULT_COLOR_R 255
#define DEFAULT_COLOR_G 255
#define DEFAULT_COLOR_B 255

// Kolor przy wykryciu ruchu (ciepły)
#define MOTION_COLOR_R 255
#define MOTION_COLOR_G 180
#define MOTION_COLOR_B 100

// ============== SIEĆ ==============
#define WIFI_SSID ""
#define WIFI_PASS ""

// ============== DEBUG ==============
#define SERIAL_BAUD 9600

#endif