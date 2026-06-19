#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include <Arduino.h>

// Initializes both SSD1306 OLED displays on separate I2C buses.
// Returns true if both initialized successfully.
bool initDisplays();

// Updates the Wide display (Display 1) with time and device status.
void updateWideDisplay(const String& timeStr, bool isOutage, bool showSquare);

// Updates the Narrow display (Display 2) with the failure point and outage duration.
// If not in an outage, display is cleared (blank).
void updateNarrowDisplay(const String& failPoint, unsigned long outageDurationSecs);

#endif // DISPLAY_UTILS_H
