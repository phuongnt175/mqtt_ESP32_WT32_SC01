#include <ArduinoJson.h>
#include <vector>

class JsonCmdStatus {
public:
    struct Device {
        int devid;
        String mac;
        uint8_t endpoint;
        String source;
        JsonArray states; // Changed to JsonArray
        String type;

        Device() : devid(0), endpoint(0) {}
    };

    typedef std::vector<Device> ListDevice_t;

    ListDevice_t List() const {
        return list_devices_;
    }

    bool ParseJsonCommand(const char* jsonString);
    static String CreateJsonCommand(const Device& device);
    static String CreateJsonCommand(const ListDevice_t& devices);

private:
    ListDevice_t list_devices_;
};

bool JsonCmdStatus::ParseJsonCommand(const char* jsonString) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
        Serial.println("Failed to parse JSON");
        return false;
    }

    list_devices_.clear();

    for (JsonVariant device : doc.as<JsonArray>()) {
        if (device.containsKey("hash") && device.containsKey("states")) {
            Device dev;
            String temp = device["hash"].as<String>();
            int pos1stCol = temp.indexOf("-");
            if (pos1stCol == -1) {
                Serial.println("Invalid command!");
                continue;
            }
            dev.source = temp.substring(0, pos1stCol);

            temp = temp.substring(pos1stCol + 1);
            int pos2ndCol = temp.indexOf("-");
            if (pos2ndCol == -1) {
                Serial.println("Invalid command!");
                continue;
            }
            dev.mac = temp.substring(0, pos2ndCol);
            dev.mac.toUpperCase();

            temp = temp.substring(pos2ndCol + 1);
            if (temp.isEmpty()) {
                Serial.println("Invalid command!");
                continue;
            }
            dev.endpoint = temp.toInt();

            dev.states = device["states"].as<JsonArray>(); // Changed to JsonArray

            list_devices_.push_back(dev);
        } else {
            Serial.println("Device status format not recognized");
        }
    }

    return true;
}

String JsonCmdStatus::CreateJsonCommand(const Device& device) {
    StaticJsonDocument<256> doc;
    JsonObject data = doc.createNestedArray().createNestedObject(); // Changed to createNestedArray().createNestedObject()
    String source = device.source;
    if (source.isEmpty()) {
        source = "zigbee";
    }
    data["hash"] = source + "-" + device.mac + "-" + String(device.endpoint);
    data["states"] = device.states;
    data["type"] = device.type;

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

String JsonCmdStatus::CreateJsonCommand(const ListDevice_t& devices) {
    StaticJsonDocument<1024> doc;
    JsonArray data = doc.to<JsonArray>();

    for (const Device& device : devices) {
        JsonObject device_status = data.createNestedObject();
        String source = device.source;
        if (source.isEmpty()) {
            source = "zigbee";
        }
        device_status["hash"] = source + "-" + device.mac + "-" + String(device.endpoint);
        device_status["states"] = device.states;
        device_status["type"] = device.type;
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}