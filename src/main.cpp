#include <Arduino.h>

#include "ForgeAgent.h"
#include "ForgeConfig.h"
#include "DhtSensor.h"

ForgeAgent agent;
DhtSensor environmentSensor(DHT_PIN, DHT_SENSOR_TYPE);

unsigned long lastTelemetryMs = 0;

void setup() {
  agent.begin();
  environmentSensor.begin();
  Serial.print("DHT sensor initialized on GPIO ");
  Serial.print(DHT_PIN);
  Serial.print(" as DHT");
  Serial.println(DHT_SENSOR_TYPE);
}

void loop() {
  agent.loop();

  const unsigned long now = millis();
  if (now - lastTelemetryMs >= TELEMETRY_INTERVAL_MS) {
    EnvironmentReading reading;
    if (environmentSensor.read(reading)) {
      agent.publishTelemetry("temperature", reading.temperatureC, "celsius");
      agent.publishTelemetry("humidity", reading.humidityPercent, "percent");
      Serial.print("Published telemetry temperature=");
      Serial.print(reading.temperatureC);
      Serial.print(" celsius humidity=");
      Serial.print(reading.humidityPercent);
      Serial.println(" percent");
    } else {
      agent.publishEvent("sensor_read_error", "warning", environmentSensor.lastError());
      Serial.print("DHT read failed: ");
      Serial.println(environmentSensor.lastError());
    }

    lastTelemetryMs = now;
  }
}
