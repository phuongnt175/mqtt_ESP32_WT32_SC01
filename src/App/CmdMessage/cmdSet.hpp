#include <ArduinoJson.h>
#include <vector>

struct Device {
  uint32_t devid;
  String mac;
  uint8_t endpoint;
  String source;
  JsonVariant execution;
};

class JsonCmdSet {
public:
  using ListDevice_t = std::vector<Device>;

  void Refresh() {
    list_devices.clear();
    execution_.clear();
  }

  ListDevice_t List() const {
    return list_devices;
  }

  void ParseJsonCommand(const String& jsonCommand) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonCommand);

    if (error) {
      Serial.print("Parsing JSON failed: ");
      Serial.println(error.c_str());
      return;
    }

    list_devices.clear();
    execution_.clear();

    JsonArray devices = doc["devices"];

    for (const auto& device : devices) {
      Device newDevice;
      newDevice.devid = device["devid"];
      newDevice.mac = device["mac"].as<String>();
      newDevice.endpoint = device["endpoint"];
      newDevice.source = device["source"].as<String>();
      newDevice.execution = device["execution"].as<JsonVariant>();

      list_devices.push_back(newDevice);
    }

    execution_ = doc["execution"];
  }

  void CreateJsonCommand(const Device& device, JsonObject& command) {
    command["devid"] = device.devid;
    command["mac"] = device.mac;
    command["endpoint"] = device.endpoint;
    command["source"] = device.source;
    command["execution"] = device.execution;
  }

  void CreateJsonCommand(const ListDevice_t& devices, JsonObject& command) {
    JsonArray deviceArray = command.createNestedArray("devices");

    for (const auto& device : devices) {
      JsonObject deviceObj = deviceArray.createNestedObject();
      CreateJsonCommand(device, deviceObj);
    }

    command["execution"] = execution_;
  }

private:
  ListDevice_t list_devices;
  JsonVariant execution_;
};