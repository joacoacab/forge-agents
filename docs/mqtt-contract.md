# MQTT Contract

Forge Agents nodes publish MQTT messages using a shared topic and payload model.

## Topic Format

```txt
edgeforge/{project}/{node}/{channel}
```

## Fields

`project` is the project or use case, for example `germinator`, `irrigation`, or `galpon`.

`node` is the unique device identifier, for example `esp32-01`, `esp32-cam-01`, `patio-01`, or `zone-a`.

`channel` is the message category. Supported channels are `telemetry`, `status`, `events`, `commands`, and `images`.

## Examples

```txt
edgeforge/germinator/esp32-01/telemetry
edgeforge/germinator/esp32-01/status
edgeforge/germinator/esp32-001/telemetry
edgeforge/germinator/esp32-cam-01/images
edgeforge/irrigation/patio-01/commands
edgeforge/galpon/zone-a/events
```

## Telemetry Payload

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

Humidity uses the same payload shape:

```json
{
  "project": "germinator",
  "node": "esp32-001",
  "metric": "humidity",
  "value": 61.2,
  "unit": "percent",
  "timestamp": "uptime:120",
  "status": "ok"
}
```

## Status Payload

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

## Event Payload

```json
{
  "project": "germinator",
  "node": "esp32-01",
  "event": "sensor_read_error",
  "severity": "warning",
  "message": "Failed to read humidity sensor",
  "timestamp": "2026-05-12T18:00:00Z"
}
```
