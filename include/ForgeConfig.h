#pragma once

// Copy this file into your project-specific firmware and change these values.
// Keep every node ID unique inside a project.

#define FORGE_PROJECT "germinator"
#define FORGE_NODE_ID "esp32-01"
#define FORGE_FIRMWARE_VERSION "0.1.0"

#define WIFI_SSID "CHANGE_ME"
#define WIFI_PASSWORD "CHANGE_ME"

#define MQTT_HOST "192.168.1.10"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define HEARTBEAT_INTERVAL_MS 30000UL
#define TELEMETRY_INTERVAL_MS 10000UL
