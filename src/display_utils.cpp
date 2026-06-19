#include "display_utils.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "formatters.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

bool initDisplays() {
    // Initialize I2C0 on Pins 8 (SDA) and 9 (SCL)
    Wire.begin(8, 9, 400000);
    
    // Initialize I2C1 on Pins 10 (SDA) and 11 (SCL)
    Wire1.begin(10, 11, 400000);
    
    // Initialize OLED displays
    bool init1 = display1.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    bool init2 = display2.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    
    if (init1) {
        display1.clearDisplay();
        display1.setTextWrap(false);
        display1.display();
    }
    if (init2) {
        display2.clearDisplay();
        display2.setTextWrap(false);
        display2.display();
    }
    
    return init1 && init2;
}

void updateWideDisplay(const String& timeStr, bool isOutage) {
    display1.clearDisplay();
    display1.setTextColor(SSD1306_WHITE);
    
    // Show Time (Large Text)
    display1.setTextSize(2);
    display1.setCursor(0, 0);
    display1.print(timeStr);
    
    // Show Status
    display1.setTextSize(1);
    display1.setCursor(0, 20);
    if (isOutage) {
        display1.print("STATUS: OUTAGE");
    } else {
        display1.print("STATUS: OK");
    }
    
    display1.display();
}

void updateNarrowDisplay(const String& failPoint, unsigned long outageDurationSecs) {
    display2.clearDisplay();
    
    if (failPoint.length() == 0) {
        display2.display();
        return;
    }
    
    display2.setTextColor(SSD1306_WHITE);
    
    // Show Point of Failure (Large Text)
    display2.setTextSize(2);
    display2.setCursor(0, 0);
    display2.print(failPoint);
    
    // Show Time Since Outage Detected
    display2.setTextSize(1);
    display2.setCursor(0, 20);
    std::string dur = formatDuration(outageDurationSecs);
    display2.print("DOWN: " + String(dur.c_str()));
    
    display2.display();
}
