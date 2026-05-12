#pragma once

// Example configuration for a Forge Agents ESP32 sensor node.
// Copy to include/ForgeConfig.h and adjust for your device.

#define FORGE_PROJECT "germinator"
#define FORGE_NODE_ID "esp32-01"
#define FORGE_FIRMWARE_VERSION "0.1.0"

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

#define MQTT_HOST "192.168.1.10"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define HEARTBEAT_INTERVAL_MS 30000UL
#define TELEMETRY_INTERVAL_MS 10000UL
