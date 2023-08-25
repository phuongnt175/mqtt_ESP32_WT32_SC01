#include <Mid/jsonMessage/Mid_jsonMessage.h>

void generateJsonCommandPost(const String& bridgeKey, const String& reqId, char jsonString[4096], char *macDevice) 
{
  StaticJsonDocument<4096> doc;
  
  doc["cmd"] = "post";
  JsonArray objects = doc.createNestedArray("objects");
  JsonObject object = objects.createNestedObject();
  object["type"] = "devices_local";
  object["bridge_key"] = bridgeKey;

  JsonArray data = object.createNestedArray("data");
  // Generating data items
  for (int i = 1; i <= 7; i =i+2) {
    JsonObject dataItem = data.createNestedObject();
    dataItem["type"] = "SWITCH";
    dataItem["brigde_key"] = bridgeKey;
    dataItem["hash"] = bridgeKey + String("-") + String(macDevice) + String("-") + String(i); //get mac

    JsonObject attr = dataItem.createNestedObject("attr");
    attr["McuInfo"]["GlassType"] = 2;
    attr["McuInfo"]["RelayType"] = 0;
    attr["deviceInfo"]["Manufacturer"] = "Lumi R&D";
    attr["deviceInfo"]["ModelId"] = "LM-SZDM4";
    attr["sceneConfig"]["lock_touch"] = false;
    attr["sceneConfig"]["output"]["delay"] = 0;
    attr["sceneConfig"]["output"]["mode"] = 0;
    attr["sceneConfig"]["touch_mode"] = 0;
    // Create sceneSetting array and add objects
    JsonArray sceneSetting = attr.createNestedArray("sceneSetting");
    JsonObject sceneSettingItem1 = sceneSetting.createNestedObject();
    sceneSettingItem1["name"] = "lock_touch";
    JsonObject sceneSettingItem2 = sceneSetting.createNestedObject();
    sceneSettingItem2["name"] = "touch_mode";
    JsonArray traits = dataItem.createNestedArray("traits");
    JsonObject trait = traits.createNestedObject();
    trait["is_main"] = true;
    trait["name"] = "OnOff";

    doc["reqid"] = reqId;
    doc["source"] = bridgeKey;
  }
  serializeJson(doc, jsonString, 4096);
}

void generateJsonCmdStatus(const String& bridgeKey, const String& reqId, char jsonString[1024], char *macDevice, const char *ep, boolean flag)
{
  StaticJsonDocument<1024> doc;

  // Add command and request ID
  doc["cmd"] = "status";
  // Create an array of objects for devices
  JsonArray devices = doc.createNestedArray("objects");

  // Create a device object
  JsonObject device = devices.createNestedObject();
  device["bridge_key"] = bridgeKey;
  device["type"] = "devices";

  // Create an array of data for the device
  JsonArray data = device.createNestedArray("data");

  // Create a data object for the device
  JsonObject dataObject = data.createNestedObject();

 // dataObject["hash"] = bridgeKey + String(macDevice) + String(ep);
  dataObject["hash"] =  bridgeKey + String("-") + macDevice + String(ep);
  dataObject["type"] = "SWITCH";

  // Create a states object for the data
  JsonObject states = dataObject.createNestedObject("states");
  states["OnOff"]["on"] = flag;
  doc["reqid"] = reqId;
  doc["source"] = bridgeKey;
  serializeJson(doc, jsonString, 1024);
}

void responseGetStatus(const String& bridgeKey, const String& reqId, char jsonString[], char* macDevice)
{

  StaticJsonDocument<4096> doc;
  doc.clear();

  boolean flag[8];
  if(lv_obj_get_state(ui_button1) == 6 || lv_obj_get_state(ui_button1) == 0)
  {
    flag[1] = false;
  }
  else if(lv_obj_get_state(ui_button1) == 7 || lv_obj_get_state(ui_button1) == 1)
  {
    flag[1] = true;
  }
  if(lv_obj_get_state(ui_button2) == 6 || lv_obj_get_state(ui_button2) == 0)
  {
    flag[3] = false;
  }
  else if(lv_obj_get_state(ui_button2) == 7 || lv_obj_get_state(ui_button2) == 1)
  {
    flag[3] = true;
  }

  if(lv_obj_get_state(ui_button3) == 6 || lv_obj_get_state(ui_button3) == 0)
  {
    flag[5] = false;
  }
  else if(lv_obj_get_state(ui_button3) == 7 || lv_obj_get_state(ui_button3) == 1)
  {
    flag[5] = true;
  }

  if(lv_obj_get_state(ui_button4) == 6 || lv_obj_get_state(ui_button4) == 0)
  {
    flag[7] = false;
  }
  else if(lv_obj_get_state(ui_button4) == 7 || lv_obj_get_state(ui_button4) == 1)
  {
    flag[7] = true;
  }
  // Add command and request ID
  doc["cmd"] = "status";
  // Create an array of objects for devices
  JsonArray objects = doc.createNestedArray("objects");

  // Create a device object
  JsonObject device = objects.createNestedObject();
  device["bridge_key"] = bridgeKey;
  device["type"] = "devices";
  
  // Create an array of data for the device
  JsonArray data = device.createNestedArray("data");
  for (int i = 1; i <= 7; i =i+2){
    // Create a data object for the device
    JsonObject dataObject = data.createNestedObject();
    dataObject["hash"] = String(bridgeKey) + String("-") + String(macDevice) + String("-") + String(i);
    dataObject["type"] = "SWITCH";

    // Create a states object for the data
    JsonObject states = dataObject.createNestedObject("states");
    states["OnOff"]["on"] = flag[i];
    if(i == 7)
    {
      break;
    }
  }
  doc["reqid"] = reqId;
  doc["source"] = bridgeKey;
  // Serialize the JSON document to a string
  serializeJson(doc, jsonString, 4096);
}