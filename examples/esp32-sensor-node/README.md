# ESP32 Sensor Node Example

The repository root is a working PlatformIO project for the first ESP32 sensor node MVP.

To adapt it:

1. Edit `include/ForgeConfig.h`.
2. Replace `readExampleTemperature()` in `src/main.cpp` with a real sensor read.
3. Add reusable sensor drivers under `lib/`.
4. Build and upload with PlatformIO.

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
