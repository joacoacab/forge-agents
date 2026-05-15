# Forge Agents

Forge Agents is a reusable firmware/runtime foundation for IoT and Edge devices that connect to EdgeForge through MQTT.

The first target is a simple ESP32 sensor node built with PlatformIO. It handles WiFi, MQTT, status heartbeats, DHT11 temperature/humidity telemetry, command subscription, and a small structure for future sensors and drivers.

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
edgeforge/germinator/esp32-001/telemetry
```

See [docs/mqtt-contract.md](docs/mqtt-contract.md) for payload examples.

## Repository Layout

```txt
include/
  ForgeConfig.h             Device configuration used by the MVP build
  ForgeConfig.example.h     Copyable configuration template
lib/
  ForgeAgent/               Reusable WiFi, MQTT, status, telemetry, and event helper
  ForgeSensors/             Small reusable sensor drivers
src/
  main.cpp                  Germinator ESP32-001 firmware entrypoint
docs/
  mqtt-contract.md          MQTT topic and payload contract
platformio.ini              PlatformIO project configuration
```

## Configure

Create a local config from the example, then edit it before flashing:

```sh
cp include/ForgeConfig.example.h include/ForgeConfig.h
```

`include/ForgeConfig.h` is ignored by git so WiFi credentials stay local.

```cpp
#define FORGE_PROJECT "germinator"
#define FORGE_NODE_ID "esp32-001"

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

#define MQTT_HOST "192.168.0.32"
#define MQTT_PORT 1883

#define DHT_PIN 23
#define DHT_SENSOR_TYPE 11
```

Every node must have a unique `FORGE_NODE_ID` within its project.

## Wiring

Current MVP wiring:

```txt
DHT11 +    -> ESP32 3V3
DHT11 out  -> ESP32 GPIO 23
DHT11 -    -> ESP32 GND
```

If your DHT11 module does not include a pull-up resistor, add a 4.7k to 10k
resistor between `3V3` and `out`.

## Build

Install PlatformIO, then compile:

```sh
pio run
```

## Upload

With the ESP32 connected over USB:

```sh
pio run --target upload
```

On WSL, pass the USB device from Windows first with `usbipd`, then use the
detected port explicitly:

```sh
pio device list
pio run --target upload --upload-port /dev/ttyUSB0
```

## Serial Monitor

Open the monitor at 115200 baud:

```sh
pio device monitor --port /dev/ttyUSB0 --baud 115200
```

Expected lines after WiFi and MQTT connect:

```txt
DHT sensor initialized on GPIO 23 as DHT11
Published telemetry temperature=17.60 celsius humidity=63.00 percent
```

The node publishes:

- status heartbeats to `edgeforge/{project}/{node}/status`
- temperature telemetry to `edgeforge/{project}/{node}/telemetry`
- humidity telemetry to `edgeforge/{project}/{node}/telemetry`
- command receipt events to `edgeforge/{project}/{node}/events`

It also subscribes to:

```txt
edgeforge/{project}/{node}/commands
```

## ESP32 to EdgeForge Flow

```txt
ESP32 DHT11 sensor
  -> ForgeAgent publishes MQTT
  -> Mosquitto receives edgeforge/germinator/esp32-001/telemetry
  -> Node-RED parses the JSON payload
  -> InfluxDB stores metric/value/unit/status
  -> Grafana visualizes the time series
```

The telemetry payload stays compatible with the current backend:

```json
{
  "project": "germinator",
  "node": "esp32-001",
  "metric": "temperature",
  "value": 24.6,
  "unit": "celsius",
  "timestamp": "uptime:120",
  "status": "ok"
}
```

Humidity uses the same payload shape with `metric` set to `humidity` and `unit`
set to `percent`.

## Validate MQTT

From the Raspberry Pi or any machine that can reach the broker:

```sh
mosquitto_sub -h 192.168.0.32 -p 1883 -t 'edgeforge/germinator/esp32-001/telemetry' -v
```

Expected messages:

```txt
edgeforge/germinator/esp32-001/telemetry {"project":"germinator","node":"esp32-001","metric":"temperature","value":17.6,"unit":"celsius","timestamp":"uptime:120","status":"ok"}
edgeforge/germinator/esp32-001/telemetry {"project":"germinator","node":"esp32-001","metric":"humidity","value":63,"unit":"percent","timestamp":"uptime:120","status":"ok"}
```

## Troubleshooting

If readings fail or appear as `NaN`, confirm the sensor type and pin in
`include/ForgeConfig.h`:

```cpp
#define DHT_PIN 23
#define DHT_SENSOR_TYPE 11
```

Use `11` for DHT11 and `22` for DHT22/AM2302. Also check the DHT data wire and
pull-up resistor.

If MQTT does not connect, confirm the Raspberry Pi broker address:

```cpp
#define MQTT_HOST "192.168.0.32"
#define MQTT_PORT 1883
```

Also verify the broker from your machine:

```sh
mosquitto_sub -h 192.168.0.32 -p 1883 -t 'edgeforge/#' -v
```

## PlatformIO Libraries

The MVP uses these PlatformIO libraries:

- `knolleary/PubSubClient`: small MQTT client, widely used with Arduino ESP32.
- `bblanchon/ArduinoJson`: predictable JSON serialization for MQTT payloads.
- `adafruit/DHT sensor library`: DHT11/DHT22/AM2302 sensor support.
- `adafruit/Adafruit Unified Sensor`: dependency used by the Adafruit DHT stack.

For a future BME280 sensor, add:

```ini
adafruit/Adafruit BME280 Library
```

BME280 is usually wired over I2C:

```txt
BME280 VIN -> ESP32 3V3
BME280 GND -> ESP32 GND
BME280 SCL -> ESP32 GPIO 22
BME280 SDA -> ESP32 GPIO 21
```

## Extending the MVP

Add isolated drivers under `lib/ForgeSensors/` and keep project-specific logic in `src/main.cpp` or in a dedicated example folder. Future nodes should only need a new `ForgeConfig.h`, a unique `FORGE_NODE_ID`, and whichever sensor driver they initialize.
