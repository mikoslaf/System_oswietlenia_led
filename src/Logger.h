#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

// Pomocnicza funkcja wyciągająca samą nazwę pliku ze ścieżki
inline const char* getFilename(const char* path) {
    const char* filename = strrchr(path, '\\');  // Windows
    if (filename) return filename + 1;
    
    filename = strrchr(path, '/');               // Unix/Linux
    if (filename) return filename + 1;
    
    return path;
}

// Główna funkcja logująca zmiany światła
inline void logLightChange(bool turnedOn, const char* reason, const char* file, int line) {
    Serial.print("[");
    Serial.print(getFilename(file));
    Serial.print(":");
    Serial.print(line);
    Serial.print("] ");
    Serial.print(turnedOn ? "LIGHT ON " : "LIGHT OFF");
    Serial.print(" | Reason: ");
    Serial.println(reason);
}

// Makro dla łatwego użycia - automatycznie wstawia nazwę pliku i linię
#define LOG_LIGHT_CHANGE(state, reason) logLightChange(state, reason, __FILE__, __LINE__)

#endif