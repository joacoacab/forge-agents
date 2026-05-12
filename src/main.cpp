#include <Arduino.h>

#include "ForgeAgent.h"
#include "ForgeConfig.h"

ForgeAgent agent;

unsigned long lastTelemetryMs = 0;

float readExampleTemperature() {
  const float wave = sin(millis() / 15000.0f) * 1.5f;
  return 24.0f + wave;
}

void setup() {
  agent.begin();
}

void loop() {
  agent.loop();

  const unsigned long now = millis();
  if (now - lastTelemetryMs >= TELEMETRY_INTERVAL_MS) {
    agent.publishTelemetry("temperature", readExampleTemperature(), "celsius");
    lastTelemetryMs = now;
  }
}
