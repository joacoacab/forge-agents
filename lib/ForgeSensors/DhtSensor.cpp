#include "DhtSensor.h"

#include <cmath>

DhtSensor::DhtSensor(uint8_t pin, uint8_t type)
    : dht_(pin, type), lastError_("not_initialized") {}

void DhtSensor::begin() {
  dht_.begin();
  lastError_ = "ok";
}

bool DhtSensor::read(EnvironmentReading& reading) {
  const float humidity = dht_.readHumidity();
  const float temperature = dht_.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    lastError_ = "failed_to_read_dht";
    return false;
  }

  reading.temperatureC = temperature;
  reading.humidityPercent = humidity;
  lastError_ = "ok";
  return true;
}

const char* DhtSensor::lastError() const {
  return lastError_;
}
