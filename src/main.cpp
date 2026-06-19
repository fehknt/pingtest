#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <ESP32Ping.h>
#include <Adafruit_NeoPixel.h>

#include "secrets.h"

#define RGB_BUILTIN 48
Adafruit_NeoPixel pixels(1, RGB_BUILTIN, NEO_GRB + NEO_KHZ800);
#include "display_utils.h"

// Define network monitoring states
enum NetworkState {
    STATE_OK = 0,
    STATE_WIFI_FAIL,
    STATE_ROUTER_FAIL,
    STATE_MODEM_FAIL,
    STATE_ISP_FAIL,
    STATE_INTERNET_FAIL
};

NetworkState currentState = STATE_OK;
unsigned long outageStartTime = 0;

// Helper to get formatted local time HH:MM
String getLocalTimeStr() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 10)) {
        return "--:--";
    }
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    return String(buf);
}

unsigned long lastPingTime = 0;
const unsigned long pingInterval = 60000; // 1 minute

unsigned long lastDisplayTime = 0;
const unsigned long displayInterval = 500; // 500ms for 1Hz blink (on 0.5s, off 0.5s)

// Helper function to ping a target with up to 3 rapid retries on failure
bool pingWithRetry(const char* host) {
    for (int attempt = 1; attempt <= 3; attempt++) {
        Serial.printf("[Ping] Target %s - Attempt %d/3...", host, attempt);
        bool success = Ping.ping(host, 1);
        if (success) {
            Serial.println(" Success!");
            return true;
        }
        Serial.println(" Failed.");
    }
    return false;
}

// Sequence to determine the first point of failure
NetworkState checkNetwork() {
    if (WiFi.status() != WL_CONNECTED) {
        // If WiFi is not connected, report WIFI outage.
        return STATE_WIFI_FAIL;
    }

    if (!pingWithRetry(IP_ROUTER)) {
        return STATE_ROUTER_FAIL;
    }
    if (!pingWithRetry(IP_MODEM)) {
        return STATE_MODEM_FAIL;
    }
    if (!pingWithRetry(IP_ISP)) {
        return STATE_ISP_FAIL;
    }
    if (!pingWithRetry(IP_INTERNET)) {
        return STATE_INTERNET_FAIL;
    }
    return STATE_OK;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- ESP32 Network Monitor Starting ---");

    pixels.begin();
    pixels.clear();
    pixels.show();

    // Initialize Displays
    if (!initDisplays()) {
        Serial.println("[ERROR] Failed to initialize one or both OLED displays!");
    } else {
        Serial.println("[OK] OLED displays initialized successfully.");
    }

    // Connect to WiFi
    Serial.printf("[WiFi] Connecting to SSID: %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    // Non-blocking WiFi connection feedback on displays
    updateWideDisplay("CONNECTING", false, false);
    updateNarrowDisplay("", 0);

    int connectionRetries = 0;
    while (WiFi.status() != WL_CONNECTED && connectionRetries < 20) {
        delay(500);
        Serial.print(".");
        connectionRetries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\n[WiFi] Connected. IP: %s\n", WiFi.localIP().toString().c_str());
        configTzTime(TZ_RULE, "pool.ntp.org");
    } else {
        Serial.println("\n[WiFi] Connection failed. Starting monitoring anyway.");
    }

    // Run initial network check
    currentState = checkNetwork();
    if (currentState != STATE_OK) {
        outageStartTime = millis();
    }
}

void loop() {
    unsigned long currentMillis = millis();

    // Pulse RGB LED when there is an outage
    static unsigned long lastPixelUpdate = 0;
    if (currentMillis - lastPixelUpdate >= 20) {
        lastPixelUpdate = currentMillis;
        if (currentState != STATE_OK) {
            float phase = (currentMillis % 1000) / 1000.0 * 2.0 * PI;
            int brightness = (int)(((sin(phase - PI/2) + 1.0) / 2.0) * 255.0);
            pixels.setPixelColor(0, pixels.Color(brightness, 0, 0));
        } else {
            // Dim green for OK status (green value: 15)
            pixels.setPixelColor(0, pixels.Color(0, 15, 0));
        }
        pixels.show();
    }

    // 1-minute network check timer
    if (currentMillis - lastPingTime >= pingInterval || lastPingTime == 0) {
        lastPingTime = currentMillis;
        Serial.println("[Monitor] Running scheduled ping tests...");
        
        NetworkState nextState = checkNetwork();
        
        if (nextState != STATE_OK) {
            if (currentState == STATE_OK) {
                // Outage started
                outageStartTime = currentMillis;
                Serial.println("[ALERT] Network outage detected!");
            }
        } else {
            if (currentState != STATE_OK) {
                // Network recovered
                Serial.println("[INFO] Network restored.");
            }
        }
        currentState = nextState;
    }

    // 1-second display update timer
    if (currentMillis - lastDisplayTime >= displayInterval) {
        lastDisplayTime = currentMillis;

        // Get time string from local clock
        String timeStr = getLocalTimeStr();

        bool isOutage = (currentState != STATE_OK);
        bool showSquare = isOutage && ((currentMillis / 500) % 2 == 0);
        updateWideDisplay(timeStr, isOutage, showSquare);

        if (isOutage) {
            String failureLabel = "";
            switch (currentState) {
                case STATE_WIFI_FAIL:     failureLabel = "WIFI"; break;
                case STATE_ROUTER_FAIL:   failureLabel = "ROUTER"; break;
                case STATE_MODEM_FAIL:    failureLabel = "MODEM"; break;
                case STATE_ISP_FAIL:      failureLabel = "ISP"; break;
                case STATE_INTERNET_FAIL: failureLabel = "INTERNET"; break;
                default:                  failureLabel = "UNKNOWN"; break;
            }
            unsigned long outageDurationSecs = (currentMillis - outageStartTime) / 1000;
            updateNarrowDisplay(failureLabel, outageDurationSecs);
        } else {
            updateNarrowDisplay("", 0);
        }
    }
}
