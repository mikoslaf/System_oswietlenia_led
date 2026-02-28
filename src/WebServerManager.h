#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

#include <WiFiS3.h>
#include "Config.h"
#include "RuntimeSettings.h"

class WebServerManager {
private:
    WiFiServer server;
    RuntimeSettings* settings;
    
    int getParamValue(String& request, const char* param) {
        String paramStr = String(param) + "=";
        int startIdx = request.indexOf(paramStr);
        if (startIdx == -1) return -1;
        
        startIdx += paramStr.length();
        int endIdx = request.indexOf('&', startIdx);
        if (endIdx == -1) endIdx = request.indexOf(' ', startIdx);
        if (endIdx == -1) endIdx = request.length();
        
        String value = request.substring(startIdx, endIdx);
        return value.toInt();
    }
    
    void sendPageHeader(WiFiClient& client) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html; charset=utf-8");
        client.println("Connection: close");
        client.println();
    }
    
    void sendStyles(WiFiClient& client) {
        client.println("<!DOCTYPE html><html><head>");
        client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
        client.println("<style>");
        client.println("body{font-family:Arial;margin:0;padding:20px;background:#1a1a2e;}");
        client.println(".container{max-width:500px;margin:0 auto;background:#16213e;padding:20px;border-radius:15px;box-shadow:0 4px 15px rgba(0,0,0,0.3);}");
        client.println("h1{color:#e94560;text-align:center;margin-bottom:20px;}");
        client.println("h2{color:#0f3460;background:#e94560;padding:10px;border-radius:8px;margin:20px 0 15px 0;}");
        client.println(".form-group{margin:15px 0;background:#0f3460;padding:15px;border-radius:10px;}");
        client.println("label{display:block;margin-bottom:8px;font-weight:bold;color:#eee;}");
        client.println("input[type=number]{width:100%;padding:12px;border:2px solid #e94560;border-radius:8px;box-sizing:border-box;background:#1a1a2e;color:#fff;font-size:16px;}");
        client.println("input[type=range]{width:100%;margin:10px 0;}");
        client.println("output{color:#e94560;font-weight:bold;font-size:18px;}");
        client.println("input[type=submit],input[type=button]{width:100%;padding:15px;border:none;border-radius:10px;cursor:pointer;font-size:16px;font-weight:bold;margin-top:10px;transition:transform 0.2s;}");
        client.println("input[type=submit]:hover,input[type=button]:hover{transform:scale(1.02);}");
        client.println(".btn-save{background:linear-gradient(45deg,#28a745,#20c997);color:white;}");
        client.println(".btn-reset{background:linear-gradient(45deg,#dc3545,#e94560);color:white;}");
        client.println(".btn-back{background:linear-gradient(45deg,#6c757d,#495057);color:white;}");
        client.println(".color-inputs{display:flex;gap:10px;}");
        client.println(".color-inputs input{width:33%;text-align:center;}");
        client.println(".info{font-size:12px;color:#888;margin-top:8px;font-style:italic;}");
        client.println(".color-preview{height:30px;border-radius:5px;margin-top:10px;border:2px solid #333;}");
        client.println(".alert{padding:15px;border-radius:10px;margin-bottom:20px;text-align:center;font-weight:bold;}");
        client.println(".alert-success{background:linear-gradient(45deg,#28a745,#20c997);color:white;}");
        client.println(".alert-info{background:linear-gradient(45deg,#17a2b8,#138496);color:white;}");
        client.println("</style></head><body><div class='container'>");
    }
    
    void sendSettingsPage(WiFiClient& client, const char* alertMsg = nullptr, const char* alertClass = nullptr) {
        sendPageHeader(client);
        sendStyles(client);
        
        client.println("<h1>⚙️ Ustawienia</h1>");
        
        // Komunikat po akcji
        if (alertMsg != nullptr) {
            client.print("<div class='alert ");
            client.print(alertClass);
            client.print("'>");
            client.print(alertMsg);
            client.println("</div>");
        }
        
        client.println("<a href='/'><input type='button' value='← Powrót' class='btn-back'></a>");
        
        client.println("<form action='/save' method='GET'>");
        
        // Sekcja automatyki
        client.println("<h2>🤖 Automatyka</h2>");
        
        client.println("<div class='form-group'>");
        client.println("<label>📊 Próg jasności (0-1023):</label>");
        client.print("<input type='number' name='lt' min='0' max='1023' value='");
        client.print(settings->lightThreshold);
        client.println("'>");
        client.println("<div class='info'>Wartość poniżej której jest ciemno (wyższa = czulsze)</div>");
        client.println("</div>");
        
        client.println("<div class='form-group'>");
        client.println("<label>⏱️ Timeout ruchu (sekundy):</label>");
        client.print("<input type='number' name='mt' min='1' max='3600' value='");
        client.print(settings->motionTimeout / 1000);
        client.println("'>");
        client.println("<div class='info'>Po ilu sekundach bez ruchu wyłączyć światło</div>");
        client.println("</div>");
        
        client.println("<div class='form-group'>");
        client.println("<label>🚫 Blokada automatyki (sekundy):</label>");
        client.print("<input type='number' name='ud' min='0' max='300' value='");
        client.print(settings->userActionDisableTime / 1000);
        client.println("'>");
        client.println("<div class='info'>Ile sekund ignorować automatykę po ręcznej zmianie</div>");
        client.println("</div>");
        
        // Sekcja LED
        client.println("<h2>💡 Oświetlenie LED</h2>");
        
        client.println("<div class='form-group'>");
        client.println("<label>🔆 Jasność domyślna:</label>");
        client.print("<input type='range' name='br' min='1' max='255' value='");
        client.print(settings->defaultBrightness);
        client.print("' oninput='this.nextElementSibling.value=this.value'><output>");
        client.print(settings->defaultBrightness);
        client.println("</output>");
        client.println("</div>");
        
        // Kolor domyślny
        client.println("<div class='form-group'>");
        client.println("<label>🎨 Kolor domyślny (R, G, B):</label>");
        client.println("<div class='color-inputs'>");
        client.print("<input type='number' name='dr' min='0' max='255' value='");
        client.print(settings->defaultColorR);
        client.println("' placeholder='R'>");
        client.print("<input type='number' name='dg' min='0' max='255' value='");
        client.print(settings->defaultColorG);
        client.println("' placeholder='G'>");
        client.print("<input type='number' name='db' min='0' max='255' value='");
        client.print(settings->defaultColorB);
        client.println("' placeholder='B'>");
        client.println("</div>");
        client.print("<div class='color-preview' style='background:rgb(");
        client.print(settings->defaultColorR);
        client.print(",");
        client.print(settings->defaultColorG);
        client.print(",");
        client.print(settings->defaultColorB);
        client.println(")'></div>");
        client.println("</div>");
        
        // Kolor przy ruchu
        client.println("<div class='form-group'>");
        client.println("<label>🚶 Kolor przy ruchu (R, G, B):</label>");
        client.println("<div class='color-inputs'>");
        client.print("<input type='number' name='mr' min='0' max='255' value='");
        client.print(settings->motionColorR);
        client.println("' placeholder='R'>");
        client.print("<input type='number' name='mg' min='0' max='255' value='");
        client.print(settings->motionColorG);
        client.println("' placeholder='G'>");
        client.print("<input type='number' name='mb' min='0' max='255' value='");
        client.print(settings->motionColorB);
        client.println("' placeholder='B'>");
        client.println("</div>");
        client.print("<div class='color-preview' style='background:rgb(");
        client.print(settings->motionColorR);
        client.print(",");
        client.print(settings->motionColorG);
        client.print(",");
        client.print(settings->motionColorB);
        client.println(")'></div>");
        client.println("</div>");
        
        client.println("<input type='submit' value='💾 Zapisz ustawienia' class='btn-save'>");
        client.println("</form>");
        
        client.println("<form action='/reset' method='GET'>");
        client.println("<input type='submit' value='🔄 Przywróć domyślne' class='btn-reset'>");
        client.println("</form>");
        
        client.println("</div></body></html>");
    }
    
    void sendMainPage(WiFiClient& client) {
        sendPageHeader(client);
        sendStyles(client);
        
        client.println("<h1>💡 Smart Light</h1>");
        client.println("<a href='/ON'><input type='button' value='🔆 Włącz światło' class='btn-save'></a>");
        client.println("<a href='/OFF'><input type='button' value='🌑 Wyłącz światło' class='btn-reset'></a>");
        
        client.println("<style>");
        client.println(".btn-auto{background:linear-gradient(45deg,#007bff,#6f42c1);color:white;}");
        client.println(".btn-manual{background:linear-gradient(45deg,#6c757d,#495057);color:white;}");
        client.println(".btn-settings{background:linear-gradient(45deg,#17a2b8,#138496);color:white;}");
        client.println("</style>");
        
        client.println("<a href='/AUTO'><input type='button' value='🤖 Tryb automatyczny' class='btn-auto'></a>");
        client.println("<a href='/MANUAL'><input type='button' value='✋ Tryb manualny' class='btn-manual'></a>");
        client.println("<a href='/settings'><input type='button' value='⚙️ Ustawienia' class='btn-settings'></a>");
        
        client.print("<div class='form-group'><label>📊 Próg: ");
        client.print(settings->lightThreshold);
        client.print(" | ⏱️ Timeout: ");
        client.print(settings->motionTimeout / 1000);
        client.print("s | 🔆 Jasność: ");
        client.print(settings->defaultBrightness);
        client.println("</label></div>");
        
        client.println("</div></body></html>");
    }

public:
    WebServerManager() : server(80), settings(nullptr) {}

    void begin(RuntimeSettings* s) {
        settings = s;
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        server.begin();
        Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
    }

    String handleClient() {
        WiFiClient client = server.available();
        if (!client) return "";

        String request = client.readStringUntil('\r');
        client.flush();

        // Zapis ustawień - wyświetl stronę od razu z komunikatem sukcesu
        if (request.indexOf("GET /save?") != -1) {
            int val;
            
            val = getParamValue(request, "lt");
            if (val >= 0) settings->lightThreshold = val;
            
            val = getParamValue(request, "mt");
            if (val > 0) settings->motionTimeout = (unsigned long)val * 1000;
            
            val = getParamValue(request, "ud");
            if (val >= 0) settings->userActionDisableTime = (unsigned long)val * 1000;
            
            val = getParamValue(request, "br");
            if (val >= 1 && val <= 255) settings->defaultBrightness = (uint8_t)val;
            
            val = getParamValue(request, "dr");
            if (val >= 0 && val <= 255) settings->defaultColorR = (uint8_t)val;
            
            val = getParamValue(request, "dg");
            if (val >= 0 && val <= 255) settings->defaultColorG = (uint8_t)val;
            
            val = getParamValue(request, "db");
            if (val >= 0 && val <= 255) settings->defaultColorB = (uint8_t)val;
            
            val = getParamValue(request, "mr");
            if (val >= 0 && val <= 255) settings->motionColorR = (uint8_t)val;
            
            val = getParamValue(request, "mg");
            if (val >= 0 && val <= 255) settings->motionColorG = (uint8_t)val;
            
            val = getParamValue(request, "mb");
            if (val >= 0 && val <= 255) settings->motionColorB = (uint8_t)val;
            
            Serial.println("Settings saved via web panel!");
            settings->printSettings();
            
            // Wyświetl stronę ustawień z komunikatem - bez redirecta
            sendSettingsPage(client, "✅ Ustawienia zapisane pomyślnie!", "alert-success");
            client.stop();
            return "SETTINGS_SAVED";
        }
        
        // Reset ustawień - wyświetl stronę od razu z komunikatem
        if (request.indexOf("GET /reset") != -1) {
            settings->reset();
            Serial.println("Settings reset to defaults!");
            
            // Wyświetl stronę ustawień z komunikatem - bez redirecta
            sendSettingsPage(client, "🔄 Ustawienia przywrócone do domyślnych!", "alert-info");
            client.stop();
            return "SETTINGS_RESET";
        }
        
        // Strona ustawień
        if (request.indexOf("GET /settings") != -1) {
            sendSettingsPage(client);
            client.stop();
            return "";
        }

        // Komendy sterowania - strona główna od razu po akcji
        if (request.indexOf("GET /ON") != -1) {
            client.stop();
            return "ON";
        }
        if (request.indexOf("GET /OFF") != -1) {
            client.stop();
            return "OFF";
        }
        if (request.indexOf("GET /AUTO") != -1) {
            client.stop();
            return "AUTO";
        }
        if (request.indexOf("GET /MANUAL") != -1) {
            client.stop();
            return "MANUAL";
        }

        // Strona główna
        sendMainPage(client);
        client.stop();

        return "";
    }
    
    RuntimeSettings* getSettings() {
        return settings;
    }
};

#endif