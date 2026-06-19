#ifndef SECRETS_H
#define SECRETS_H

// WiFi Configuration
#define WIFI_SSID "your_wifi_ssid_here"
#define WIFI_PASSWORD "your_wifi_password_here"

// Target IP Addresses to Ping
#define IP_ROUTER "192.168.1.1" // Local Router IP
#define IP_MODEM "192.168.100.1" // Modem IP (or first hop outside router)
#define IP_ISP "8.8.4.4" // ISP Gateway or DNS
#define IP_INTERNET "8.8.8.8" // Public Internet Test IP

// Timezone specification string (handles automatic DST)
// See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for timezone rules
#define TZ_RULE "PST8PDT,M3.2.0,M11.1.0"

#endif // SECRETS_H
