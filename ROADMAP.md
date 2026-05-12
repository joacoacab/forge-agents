# Forge Agents Roadmap

## Phase 1 - Repository Bootstrap

- [x] Create base repository structure
- [x] Define MQTT topic convention
- [x] Define telemetry payload schema
- [x] Define status payload schema
- [x] Add ESP32 example project
- [x] Add configuration example

## Phase 2 - ESP32 Base Runtime

- [x] WiFi connection helper
- [x] MQTT connection helper
- [x] Telemetry publisher
- [x] Heartbeat publisher
- [x] Status publisher
- [x] Basic reconnection handling
- [ ] Last will and testament status
- [ ] NTP time synchronization

## Phase 3 - Sensor Examples

- [ ] DHT22 example
- [ ] BME280 example
- [ ] BH1750 light sensor example
- [ ] Soil moisture example

## Phase 4 - Camera Support

- [ ] ESP32-CAM example
- [ ] Timed image capture
- [ ] HTTP image upload to EdgeForge
- [ ] Camera status publishing

## Phase 5 - Control Nodes

- [ ] Relay example
- [ ] Water pump example
- [ ] Grow light example
- [ ] Fan control example
- [x] MQTT command subscription

## Phase 6 - Reliability

- [ ] Watchdog
- [ ] Offline buffering
- [ ] Config validation
- [x] Firmware version reporting
- [ ] OTA update research
