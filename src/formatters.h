#ifndef FORMATTERS_H
#define FORMATTERS_H

#include <string>

// Formats a duration in seconds to a human-readable string (e.g., "15s", "2m", "3h", "5d")
std::string formatDuration(unsigned long seconds);

#endif // FORMATTERS_H
