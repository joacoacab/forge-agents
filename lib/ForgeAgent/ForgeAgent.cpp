#include "ForgeAgent.h"

#include <ArduinoJson.h>
#include <cstring>

#include "ForgeConfig.h"

ForgeAgent::ForgeAgent()
    : mqttClient_(wifiClient_), lastHeartbeatMs_(0) {}

void ForgeAgent::begin() {
  Serial.begin(115200);
  delay(200);

  connectWiFi();
  mqttClient_.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient_.setCallback([this](char* topic, byte* payload, unsigned int length) {
    handleCommand(topic, payload, length);
  });
  connectMqtt();
  publishStatus("online");
}

void ForgeAgent::loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient_.connected()) {
    connectMqtt();
  }

  mqttClient_.loop();

  const unsigned long now = millis();
  if (now - lastHeartbeatMs_ >= HEARTBEAT_INTERVAL_MS) {
    publishHeartbeat();
    lastHeartbeatMs_ = now;
  }
}

void ForgeAgent::publishStatus(const char* status) {
  JsonDocument doc;
  doc["project"] = FORGE_PROJECT;
  doc["node"] = FORGE_NODE_ID;
  doc["status"] = status;
  doc["uptime_seconds"] = millis() / 1000;
  doc["wifi_rssi"] = WiFi.RSSI();
  doc["firmware_version"] = FORGE_FIRMWARE_VERSION;

  String payload;
  serializeJson(doc, payload);
  mqttClient_.publish(topic("status").c_str(), payload.c_str(), true);
}

void ForgeAgent::publishTelemetry(const char* metric, float value, const char* unit, const char* status) {
  JsonDocument doc;
  doc["project"] = FORGE_PROJECT;
  doc["node"] = FORGE_NODE_ID;
  doc["metric"] = metric;
  doc["value"] = value;
  doc["unit"] = unit;
  doc["timestamp"] = timestamp();
  doc["status"] = status;

  String payload;
  serializeJson(doc, payload);
  mqttClient_.publish(topic("telemetry").c_str(), payload.c_str());
}

void ForgeAgent::publishEvent(const char* event, const char* severity, const char* message) {
  JsonDocument doc;
  doc["project"] = FORGE_PROJECT;
  doc["node"] = FORGE_NODE_ID;
  doc["event"] = event;
  doc["severity"] = severity;
  doc["message"] = message;
  doc["timestamp"] = timestamp();

  String payload;
  serializeJson(doc, payload);
  mqttClient_.publish(topic("events").c_str(), payload.c_str());
}

void ForgeAgent::connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

void ForgeAgent::connectMqtt() {
  while (!mqttClient_.connected()) {
    Serial.print("Connecting to MQTT...");

    bool connected = false;
    if (strlen(MQTT_USER) > 0) {
      connected = mqttClient_.connect(FORGE_NODE_ID, MQTT_USER, MQTT_PASSWORD);
    } else {
      connected = mqttClient_.connect(FORGE_NODE_ID);
    }

    if (connected) {
      Serial.println("connected");
      subscribeCommands();
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient_.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void ForgeAgent::subscribeCommands() {
  mqttClient_.subscribe(topic("commands").c_str());
}

void ForgeAgent::handleCommand(char* topicName, byte* payload, unsigned int length) {
  String command;
  command.reserve(length + 1);

  for (unsigned int i = 0; i < length; i++) {
    command += static_cast<char>(payload[i]);
  }

  Serial.print("Command received on ");
  Serial.print(topicName);
  Serial.print(": ");
  Serial.println(command);

  publishEvent("command_received", "info", command.c_str());
}

void ForgeAgent::publishHeartbeat() {
  publishStatus("online");
}

String ForgeAgent::topic(const char* channel) const {
  String result = "edgeforge/";
  result += FORGE_PROJECT;
  result += "/";
  result += FORGE_NODE_ID;
  result += "/";
  result += channel;
  return result;
}

String ForgeAgent::timestamp() const {
  // ESP32 nodes without time sync still report uptime as a stable device-local timestamp.
  return String("uptime:") + String(millis() / 1000);
}
