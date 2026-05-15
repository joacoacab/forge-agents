# ESP32 Sensor Node Example

The repository root is a working PlatformIO project for the first ESP32 sensor node MVP.

To adapt it:

1. Edit `include/ForgeConfig.h`.
2. Wire a DHT sensor to the configured `DHT_PIN`.
3. Build and upload with PlatformIO.
4. Add future reusable sensor drivers under `lib/ForgeSensors/`.

Published topics follow:

```txt
edgeforge/{project}/{node}/telemetry
edgeforge/{project}/{node}/status
edgeforge/{project}/{node}/events
```

The command subscription topic is:

```txt
edgeforge/{project}/{node}/commands
```

The MVP publishes two telemetry metrics:

- `temperature` with unit `celsius`
- `humidity` with unit `percent`

For the first real node, the sensor is a DHT11 wired to GPIO 23.
