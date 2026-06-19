#include "formatters.h"

std::string formatDuration(unsigned long seconds) {
    if (seconds < 60) {
        return std::to_string(seconds) + "s";
    } else if (seconds < 3600) {
        return std::to_string(seconds / 60) + "m";
    } else if (seconds < 86400) {
        return std::to_string(seconds / 3600) + "h";
    } else {
        return std::to_string(seconds / 86400) + "d";
    }
}
