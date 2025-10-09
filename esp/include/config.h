#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "TechniSchools"
#define WIFI_PASSWORD "12345678"
#define WIFI_CONNECTION_TIMEOUT 30 // seconds

// Server Configuration
#define WEBSOCKET_PORT 3000
#define WEBSOCKET_PATH "/"
#define UDP_BROADCAST_PORT 8888
#define UDP_LISTEN_PORT 8889

// Hardware Configuration
#define LED_PIN 8 // Built-in LED pin

// Timing Configuration
#define RETRY_INTERVAL 10000              // 10 seconds
#define UDP_TIMEOUT 5000                  // 5 seconds
#define WEBSOCKET_TIMEOUT 10000           // 10 seconds
#define WEBSOCKET_RECONNECT_INTERVAL 5000 // 5 seconds

// JSON Buffer Sizes
#define JSON_BUFFER_SIZE_SMALL 150
#define JSON_BUFFER_SIZE_MEDIUM 200
#define JSON_BUFFER_SIZE_LARGE 300

// UDP Configuration
#define UDP_PACKET_BUFFER_SIZE 512

#endif // CONFIG_H