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
        return request.substring(startIdx, endIdx).toInt();
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
        client.println("button, input[type=submit], input[type=button]{width:100%;padding:15px;border:none;border-radius:10px;cursor:pointer;font-size:16px;font-weight:bold;margin-top:10px;transition:transform 0.2s;}");
        client.println("button:active, input:active{transform:scale(0.98);}");
        client.println(".btn-save{background:linear-gradient(45deg,#28a745,#20c997);color:white;}");
        client.println(".btn-reset{background:linear-gradient(45deg,#dc3545,#e94560);color:white;}");
        client.println(".btn-back{background:linear-gradient(45deg,#6c757d,#495057);color:white;}");
        client.println(".btn-auto{background:linear-gradient(45deg,#007bff,#6f42c1);color:white;}");
        client.println(".btn-manual{background:linear-gradient(45deg,#6c757d,#495057);color:white;}");
        client.println(".btn-settings{background:linear-gradient(45deg,#17a2b8,#138496);color:white;}");
        client.println(".color-inputs{display:flex;gap:10px;}");
        client.println(".color-inputs input{width:33%;text-align:center;}");
        client.println(".alert{padding:15px;border-radius:10px;margin-bottom:20px;text-align:center;font-weight:bold;}");
        client.println(".alert-success{background:rgba(40, 167, 69, 0.2);color:#28a745;border:1px solid #28a745;}");
        client.println(".alert-info{background:rgba(23, 162, 184, 0.2);color:#17a2b8;border:1px solid #17a2b8;}");
        client.println("</style>");
        
        // JS: Funkcja wysyłająca komendę w tle (fetch)
        client.println("<script>");
        client.println("function sendCmd(cmd) { fetch(cmd).then(r => console.log('OK')); }");
        client.println("</script>");
        client.println("</head><body><div class='container'>");
    }

    void sendSettingsPage(WiFiClient& client, const char* alertMsg = nullptr, const char* alertClass = nullptr) {
        sendPageHeader(client);
        sendStyles(client);
        
        client.println("<h1>⚙️ Ustawienia</h1>");
        if (alertMsg != nullptr) {
            client.print("<div class='alert "); client.print(alertClass); client.print("'>");
            client.print(alertMsg); client.println("</div>");
        }
        
        // Klasyczny link do powrotu
        client.println("<a href='/' style='text-decoration:none;'><button type='button' class='btn-back'>← Powrót</button></a>");
        
        client.println("<form action='/save' method='GET'>");
        client.println("<h2>🤖 Automatyka</h2>");
        client.println("<div class='form-group'><label>📊 Próg jasności (0-1023):</label>");
        client.print("<input type='number' name='lt' min='0' max='1023' value='"); client.print(settings->lightThreshold); client.println("'></div>");
        
        client.println("<div class='form-group'><label>⏱️ Timeout ruchu (s):</label>");
        client.print("<input type='number' name='mt' min='1' max='3600' value='"); client.print(settings->motionTimeout / 1000); client.println("'></div>");
        
        client.println("<h2>💡 LED</h2>");
        client.println("<div class='form-group'><label>🔆 Jasność:</label>");
        client.print("<input type='range' name='br' min='1' max='255' value='"); client.print(settings->defaultBrightness); client.println("' oninput='this.nextElementSibling.value=this.value'><output>"); client.print(settings->defaultBrightness); client.println("</output></div>");
        
        client.println("<input type='submit' value='💾 Zapisz ustawienia' class='btn-save'></form>");
        client.println("<form action='/reset' method='GET'><input type='submit' value='🔄 Resetuj' class='btn-reset'></form>");
        client.println("</div></body></html>");
    }
    
    void sendMainPage(WiFiClient& client) {
        sendPageHeader(client);
        sendStyles(client);
        
        client.println("<h1>💡 Smart Light</h1>");
        
        // Przyciski Akcji: używają sendCmd (fetch) -> BRAK przeładowania
        client.println("<button onclick=\"sendCmd('/ON')\" class='btn-save'>🔆 Włącz światło</button>");
        client.println("<button onclick=\"sendCmd('/OFF')\" class='btn-reset'>🌑 Wyłącz światło</button>");
        client.println("<button onclick=\"sendCmd('/AUTO')\" class='btn-auto'>🤖 Tryb automatyczny</button>");
        client.println("<button onclick=\"sendCmd('/MANUAL')\" class='btn-manual'>✋ Tryb manualny</button>");
        
        // Przycisk Nawigacji: zwykły link <a> -> PRZEŁADOWANIE do nowej strony
        client.println("<a href='/settings' style='text-decoration:none;'><button class='btn-settings'>⚙️ Ustawienia</button></a>");
        
        client.print("<div style='color:#888; text-align:center; margin-top:15px; font-size:12px;'>");
        client.print("Próg: "); client.print(settings->lightThreshold);
        client.print(" | Jasność: "); client.print(settings->defaultBrightness);
        client.println("</div></div></body></html>");
    }

public:
    WebServerManager() : server(80), settings(nullptr) {}

    void begin(RuntimeSettings* s) {
        settings = s;
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) delay(500);
        server.begin();
    }

    String handleClient() {
        WiFiClient client = server.available();
        if (!client) return "";

        String request = client.readStringUntil('\r');
        client.flush();

        // Funkcja wysyłająca poprawny nagłówek dla żądań w tle (fetch)
        auto sendBackgroundResponse = [&](WiFiClient& c) {
            c.println("HTTP/1.1 200 OK");
            c.println("Content-Type: text/plain");
            c.println("Content-Length: 2");
            c.println("Connection: close");
            c.println();
            c.println("OK");
        };

        // 1. Zapis / Reset (Formularze)
        if (request.indexOf("GET /save?") != -1) {
            int val;
            if ((val = getParamValue(request, "lt")) >= 0) settings->lightThreshold = val;
            if ((val = getParamValue(request, "mt")) > 0) settings->motionTimeout = (unsigned long)val * 1000;
            if ((val = getParamValue(request, "br")) >= 1 && val <= 255) settings->defaultBrightness = (uint8_t)val;
            
            sendSettingsPage(client, "✅ Zapisano pomyślnie!", "alert-success");
            delay(1); client.stop();
            return "SETTINGS_SAVED";
        }
        
        if (request.indexOf("GET /reset") != -1) {
            settings->reset();
            sendSettingsPage(client, "🔄 Przywrócono domyślne!", "alert-info");
            delay(1); client.stop();
            return "SETTINGS_RESET";
        }
        
        // 2. Nawigacja (Podstrony)
        if (request.indexOf("GET /settings") != -1) {
            sendSettingsPage(client);
            delay(1); client.stop();
            return "";
        }

        // 3. Akcje w tle (Komendy fetch)
        if (request.indexOf("GET /ON") != -1) { sendBackgroundResponse(client); client.stop(); return "ON"; }
        if (request.indexOf("GET /OFF") != -1) { sendBackgroundResponse(client); client.stop(); return "OFF"; }
        if (request.indexOf("GET /AUTO") != -1) { sendBackgroundResponse(client); client.stop(); return "AUTO"; }
        if (request.indexOf("GET /MANUAL") != -1) { sendBackgroundResponse(client); client.stop(); return "MANUAL"; }

        // 4. Strona główna (Domyślnie)
        sendMainPage(client);
        delay(1); client.stop();
        return "";
    }
};
#endif