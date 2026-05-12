# Forge Agents Instructions

This repository contains reusable firmware/runtime patterns for IoT edge devices.

## Role of This Repo

Forge Agents must provide reusable device-side code, examples, and documentation.

## Responsibilities

- WiFi connection handling
- MQTT client setup
- telemetry publishing
- device status reporting
- heartbeat messages
- command subscription
- sensor abstraction
- reusable examples for ESP32 and ESP32-CAM

## Do Not Include Here

- EdgeForge Docker Compose files
- Grafana dashboards
- InfluxDB configuration
- Node-RED flows
- Raspberry-specific infrastructure code

Those belong in the `edgeforge` repository.

## Design Principles

- Keep the device code simple and reliable.
- Prefer clear examples over complex abstractions.
- Make every device publish telemetry using the same contract.
- Every node must have a unique node ID.
- Every node should report status and heartbeat.
- Every example should be easy to adapt to a real project.

## MQTT Topic Convention

```txt
edgeforge/{project}/{node}/{channel}
```

Examples:

```txt
edgeforge/germinator/esp32-01/telemetry
edgeforge/germinator/esp32-01/status
edgeforge/germinator/esp32-cam-01/images
edgeforge/irrigation/patio-01/events
edgeforge/galpon/zone-a/telemetry
```

Channels:

- `telemetry`
- `status`
- `events`
- `commands`
- `images`

## Payload Examples

Telemetry:

```json
{
  "project": "germinator",
  "node": "esp32-01",
  "metric": "temperature",
  "value": 24.6,
  "unit": "celsius",
  "timestamp": "2026-05-12T18:00:00Z",
  "status": "ok"
}
```

Status:

```json
{
  "project": "germinator",
  "node": "esp32-01",
  "status": "online",
  "uptime_seconds": 3600,
  "wifi_rssi": -58,
  "firmware_version": "0.1.0"
}
```
