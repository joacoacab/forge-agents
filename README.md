# Forge Agents

Forge Agents is a reusable firmware/runtime foundation for IoT and Edge devices that connect to EdgeForge through MQTT.

The first target is a simple ESP32 sensor node built with PlatformIO. It handles WiFi, MQTT, status heartbeats, example telemetry, command subscription, and a small structure for future sensors and drivers.

## What Belongs Here

- Device firmware patterns
- ESP32 and ESP32-CAM examples
- WiFi and MQTT helpers
- Telemetry, status, event, and command handling
- Sensor and driver abstractions

Infrastructure such as Docker Compose, Grafana, InfluxDB, Node-RED, and Raspberry-specific services belongs in the `edgeforge` repository.

## MQTT Convention

```txt
edgeforge/{project}/{node}/{channel}
```

Supported channels:

- `telemetry`
- `status`
- `events`
- `commands`
- `images`

Example:

```txt
edgeforge/germinator/esp32-01/telemetry
```

See [docs/mqtt-contract.md](docs/mqtt-contract.md) for payload examples.

## Repository Layout

```txt
include/
  ForgeConfig.h             Device configuration used by the MVP build
  ForgeConfig.example.h     Copyable configuration template
lib/
  ForgeAgent/               Reusable WiFi, MQTT, status, telemetry, and event helper
src/
  main.cpp                  First ESP32 sensor node example
docs/
  mqtt-contract.md          MQTT topic and payload contract
platformio.ini              PlatformIO project configuration
```

## Configure

Edit `include/ForgeConfig.h` before flashing:

```cpp
#define FORGE_PROJECT "germinator"
#define FORGE_NODE_ID "esp32-01"

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

#define MQTT_HOST "192.168.1.10"
#define MQTT_PORT 1883
```

Every node must have a unique `FORGE_NODE_ID` within its project.

## Build and Upload

Install PlatformIO, then run:

```sh
pio run
pio run --target upload
pio device monitor
```

The node publishes:

- status heartbeats to `edgeforge/{project}/{node}/status`
- example temperature telemetry to `edgeforge/{project}/{node}/telemetry`
- command receipt events to `edgeforge/{project}/{node}/events`

It also subscribes to:

```txt
edgeforge/{project}/{node}/commands
```

## Extending the MVP

Replace `readExampleTemperature()` in `src/main.cpp` with a real sensor read. For reusable sensors, add isolated drivers under `lib/` and keep project-specific logic in `src/main.cpp` or in a dedicated example folder.
