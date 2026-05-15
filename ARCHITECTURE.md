# Forge Agents Architecture

Forge Agents is the device-side layer of the EdgeForge ecosystem. It provides reusable firmware patterns for nodes that publish telemetry, status, and events through MQTT.

## Layers

```txt
Application logic
  -> Sensor drivers
  -> Forge Agent core
  -> MQTT
  -> EdgeForge
```

## Core Responsibilities

WiFi connects devices to the local network and handles reconnection.

MQTT publishes telemetry, status, and events to EdgeForge. Nodes that support actions subscribe to command topics.

Telemetry uses a common JSON payload so different devices can be consumed by the same EdgeForge pipeline.

Heartbeat/status messages periodically report that a node is alive and include uptime, WiFi RSSI, and firmware version.

Sensor drivers should be reusable and isolated from project-specific logic.

Project-specific firmware should stay small: configure the node, initialize the
agent and sensors, then publish readings through the shared MQTT contract.

## Device Types

Sensor nodes publish environmental data such as temperature, humidity, light, and soil moisture.

Camera nodes capture images and publish image events or send image payloads through the `images` channel.

Control nodes receive commands and control actuators such as relays, water pumps, fans, and grow lights.

## Current MVP

The initial ESP32 node includes:

- WiFi connection handling
- MQTT connection handling
- retained online status publishing
- periodic heartbeat/status publishing
- DHT22 temperature and humidity telemetry
- command topic subscription
- command receipt event publishing

## Sensor Drivers

Drivers live under `lib/ForgeSensors/` and expose small reading structs. The
first supported driver is `DhtSensor`, which publishes:

- `temperature` in `celsius`
- `humidity` in `percent`

Future sensors should follow the same pattern without changing `ForgeAgent`.

## Planned Drivers

- BME280
- BH1750
- soil moisture sensor
- relay module
- ESP32-CAM

## Topic Model

```txt
edgeforge/{project}/{node}/{channel}
```

Example:

```txt
edgeforge/germinator/esp32-01/telemetry
edgeforge/germinator/esp32-01/status
edgeforge/germinator/esp32-01/commands
```

See [docs/mqtt-contract.md](docs/mqtt-contract.md) for the full payload contract.
