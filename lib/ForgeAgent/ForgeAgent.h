#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

class ForgeAgent {
 public:
  ForgeAgent();

  void begin();
  void loop();
  void publishStatus(const char* status);
  void publishTelemetry(const char* metric, float value, const char* unit, const char* status = "ok");
  void publishEvent(const char* event, const char* severity, const char* message);

 private:
  WiFiClient wifiClient_;
  PubSubClient mqttClient_;
  unsigned long lastHeartbeatMs_;

  void connectWiFi();
  void connectMqtt();
  void subscribeCommands();
  void handleCommand(char* topic, byte* payload, unsigned int length);
  void publishHeartbeat();
  String topic(const char* channel) const;
  String timestamp() const;
};
