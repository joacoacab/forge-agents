#pragma once

#include <Arduino.h>
#include <DHT.h>

#include "EnvironmentReading.h"

class DhtSensor {
 public:
  DhtSensor(uint8_t pin, uint8_t type);

  void begin();
  bool read(EnvironmentReading& reading);
  const char* lastError() const;

 private:
  DHT dht_;
  const char* lastError_;
};
