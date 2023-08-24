#include <ArduinoJson.h>
#include <iostream>

class JsonCmdPost {
public:
    struct DeviceInfo {
        String manufacturer;
        String modelId;
        String applicationVer;
        String zigbeeStackVer;
        String zigbeeProtocolVer;
        String hardwareVer;
        String dataCode;
        String iconkey;
        DeviceInfo() : manufacturer(""), modelId(""), applicationVer(""), zigbeeStackVer(""), zigbeeProtocolVer(""), hardwareVer(""), dataCode(""), iconkey("") {}
    };

    struct Device {
        int devid;
        String mac;
        uint8_t endpoint;
        String source;
        DeviceInfo deviceInfo;
        JsonArray traits;
        String type;
        JsonArray attr;
        JsonObject config;
        String iconkey;

        Device() : devid(0), mac(""), endpoint(0), source(""), type(""), iconkey("") {}
        Device(int devid_, String mac_, uint8_t endpoint_, String source_, JsonArray& traits_, String type_, JsonArray& attr_, JsonObject& config_, String iconkey_)
            : devid(devid_), mac(mac_), endpoint(endpoint_), source(source_), traits(traits_), type(type_), attr(attr_), config(config_), iconkey(iconkey_) {}
    };

    typedef std::vector<Device> ListDevice_t;

    void Refresh() {
        list_devices.clear();
    }

    ListDevice_t List() const {
        return list_devices;
    }

    static String CreateJsonCommand(Device dev, const std::string& bridge_key);
    static String CreateJsonCommand(ListDevice_t lstAdd, const std::string& bridge_key);

public:
    ListDevice_t list_devices;
};

String JsonCmdPost::CreateJsonCommand(Device dev, const std::string& bridge_key) {
    DynamicJsonDocument doc(4096);
    JsonObject dat = doc.to<JsonObject>();
    dat["brigde_key"] = bridge_key;
    JsonObject devInfo = dat.createNestedObject("deviceInfo");
    devInfo["Manufacturer"] = dev.deviceInfo.manufacturer;
    devInfo["ModelId"] = dev.deviceInfo.modelId;
    devInfo["ApplicationVer"] = dev.deviceInfo.applicationVer;
    devInfo["ZigbeeStackVer"] = dev.deviceInfo.zigbeeStackVer;
    devInfo["ZigbeeProtocolVer"] = dev.deviceInfo.zigbeeProtocolVer;
    devInfo["HardwareVer"] = dev.deviceInfo.hardwareVer;
    devInfo["DataCode"] = dev.deviceInfo.dataCode;
    devInfo["Iconkey"] = dev.deviceInfo.iconkey;
    dat["devid"] = dev.devid;
    dat["mac"] = dev.mac;
    dat["endpoint"] = dev.endpoint;
    dat["source"] = dev.source;
    dat["deviceInfo"] = devInfo;
    dat["traits"] = dev.traits;
    dat["type"] = dev.type;
    dat["attr"] = dev.attr;
    dat["config"] = dev.config;
    dat["iconkey"] = dev.iconkey;
    // return dat;

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

String JsonCmdPost::CreateJsonCommand(ListDevice_t lstAdd, const std::string& bridge_key) {
    DynamicJsonDocument doc(4096);
    JsonObject dat = doc.to<JsonObject>();
    dat["brigde_key"] = bridge_key;
    JsonArray arrDevices = dat.createNestedArray("devices");

    for (const auto& dev : lstAdd) {
        JsonObject devObj = arrDevices.createNestedObject();
        JsonObject devInfo = devObj.createNestedObject("deviceInfo");
        devInfo["Manufacturer"] = dev.deviceInfo.manufacturer;
        devInfo["ModelId"] = dev.deviceInfo.modelId;
        devInfo["ApplicationVer"] = dev.deviceInfo.applicationVer;
        devInfo["ZigbeeStackVer"] = dev.deviceInfo.zigbeeStackVer;
        devInfo["ZigbeeProtocolVer"] = dev.deviceInfo.zigbeeProtocolVer;
        devInfo["HardwareVer"] = dev.deviceInfo.hardwareVer;
        devInfo["DataCode"] = dev.deviceInfo.dataCode;
        devInfo["Iconkey"] = dev.deviceInfo.iconkey;
        devObj["devid"] = dev.devid;
        devObj["mac"] = dev.mac;
        devObj["endpoint"] = dev.endpoint;
        devObj["source"] = dev.source;
        devObj["deviceInfo"] = devInfo;
        devObj["traits"] = dev.traits;
        devObj["type"] = dev.type;
        devObj["attr"] = dev.attr;
        devObj["config"] = dev.config;
        devObj["iconkey"] = dev.iconkey;
    }

    // return dat;

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}