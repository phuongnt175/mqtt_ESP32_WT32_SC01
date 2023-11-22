 /* File name: 
 *
 * Description:
 *
 *
 * Last Changed By:  $Author: PhuongNT$
 * Revision:         $Revision: $
 * Last Changed:     $Date: $August 28, 2023
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <Mid/jsonMessage/Mid_jsonMessage.h>

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

String getReqId(char* message) {
  StaticJsonDocument<1024> doc;

  // Deserialize the JSON command from the message variable
  DeserializationError error = deserializeJson(doc, message);

  // Check for parsing errors
  if (error) {
    ESP_LOGE("main", "Parsing failed: %s", error.c_str());
    return String(); // Return an empty string if parsing failed
  }
  const char* reqid = doc["reqid"];
  return String(reqid);
}

void generateJsonCommandPost(const String& bridgeKey, String& reqId, char jsonString[], char *macDevice) 
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
    //dataItem["mac"] = String(macDevice);
    dataItem["macdev"] = String(macDevice);

    JsonObject attr = dataItem.createNestedObject("attr");
    attr["deviceInfo"]["ApplicationVer"] = "1";
    attr["deviceInfo"]["Manufacturer"] = "Lumi R&D";
    attr["deviceInfo"]["ModelId"] = "...";

    if(i == 1)
    {
      JsonObject sceneConfig = attr.createNestedObject("sceneConfig");
      JsonArray ruleConfig = sceneConfig.createNestedArray("ruleConfig");
      JsonObject maxRule = ruleConfig.createNestedObject();
      maxRule["MaxRule"] = "10";   //fix this to the max rule can stored in device
      // Create sceneSetting array and add objects
      JsonArray sceneSetting = attr.createNestedArray("sceneSetting");
      JsonObject sceneSettingItem = sceneSetting.createNestedObject();
      sceneSettingItem["name"] = "ruleConfig";
    }

    JsonArray traits = dataItem.createNestedArray("traits");
    JsonObject trait = traits.createNestedObject();
    trait["is_main"] = true;
    trait["name"] = "OnOff";

    JsonObject config = dataItem.createNestedObject("config");

    JsonObject deviceInfo = dataItem.createNestedObject("deviceInfo");
    deviceInfo["ApplicationVer"] = "1";
    deviceInfo["DataCode"] = " ";
    deviceInfo["HardwareVer"] = " ";
    deviceInfo["Manufacturer"] = " ";
    deviceInfo["ModelId"] = " ";
    deviceInfo["DeviceIPVersion"] = "3.0";
  }
    doc["reqid"] = reqId;
    doc["source"] = bridgeKey;
  serializeJson(doc, jsonString, 4096);
}

//==============================================================================================================================================

void generateJsonCmdStatus(const String& bridgeKey, String& reqId, char jsonString[], char *macDevice, const char *ep, boolean flag)
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

//==============================================================================================================================================

void responseGetStatus(const String& bridgeKey, String& reqId, char jsonString[], char* macDevice)
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
  for (int i = 1; i <= 7; i = i + 2){
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

void advanceStatusCmd(const String& bridgeKey, String& reqId, String ruleConfig, char jsonString[], char *macDevice, char *macHC) 
{
  StaticJsonDocument<4096> doc;

  doc["cmd"] = "status";

  JsonArray objects = doc.createNestedArray("objects");

  JsonObject advancedObject = objects.createNestedObject();
  advancedObject["type"] = "advanced";

  JsonArray data = advancedObject.createNestedArray("data");

  JsonObject dataObject = data.createNestedObject();
  dataObject["brigde_key"] = bridgeKey;
  dataObject["command"] = "status_scene";
  dataObject["hash"] = bridgeKey + String("-") + macDevice + String("-1");
  dataObject["machc"] = macHC;

  JsonArray params = dataObject.createNestedArray("params");
  JsonObject ruleObject = params.createNestedObject();
  ruleObject["ruleConfig"] = serialized(ruleConfig);

  

  doc["reqid"] = reqId;
  doc["source"] = bridgeKey;

  // Serialize JSON to string
  serializeJson(doc, jsonString, 4096);
}

void activeRuleCmd(String& reqId ,char jsonString[], const char ruleId[]) {

  StaticJsonDocument<1024> doc;
  doc.clear();

  doc["cmd"] = "set";
  doc["reqid"] = reqId;
  doc["source"] = "deviceIP";

  JsonArray objects = doc.createNestedArray("objects");
  JsonObject object = objects.createNestedObject();

  object["type"] = "rules";

  JsonArray data = object.createNestedArray("data");
  data.add(ruleId);

  serializeJson(doc, jsonString, 1024);
}

void writeJsonToFile(const char *path, String output) {
  
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    ESP_LOGE("main", "Failed to open file for writing");
    return;
  }

  // Write the JSON string to the file
  if (file.print(output)) {
    ESP_LOGE("main", "File was written");
  } else {
    ESP_LOGE("main", "File write failed");
  }
  file.close();
}

void appendJsonToFile(const char *path, String output)
{
  File file = LittleFS.open(path, FILE_APPEND);
  if(file) {
    ESP_LOGE("main", "Failed to open file for appending");
    return;
  }

  if(file.println(output)) {
    ESP_LOGE("main", "File was appended");
  } else {
    ESP_LOGE("main", "File append failed");
  }
  file.close();
}

String readJsonFromFile(const char *path) {
  // Open the file for reading
  File file = LittleFS.open(path, FILE_READ);
  if (!file) {
    ESP_LOGE("main", "Failed to open file for reading");
    return "";
  }

  // Read the file content into a String
  String fileContent = file.readString();
  file.close();

  return fileContent;
}

const char *readRuleIDValue(const char *path, int num)
{
  File file = LittleFS.open(path, "r");
  if (!file) {
    ESP_LOGE("main", "Failed to open file");
  }

  String ruleid;
  int count = 0;
  while (count < num && file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("ruleid: ")) {
      ruleid = line.substring(8);
      ruleid.trim();
      count++;
    }
  }
  file.close();
  return ruleid.c_str();
}

const char *readIconKeyValue(const char *path, int num)
{
  File file = LittleFS.open(path, "r");
  if (!file) {
    ESP_LOGE("main", "Failed to open file");
  }

  String tmp;
  int count = 0;
  while (count < num && file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("iconkey: ")) {
      tmp = line.substring(9);
      tmp.trim();
      count++;
    }
  }
  const char *value = strdup(tmp.c_str());
  file.close();
  return value;
}

const char *readNameValue(const char *path, int num)
{
  File file = LittleFS.open(path, "r");
  if (!file) {
    ESP_LOGE("main", "Failed to open file");
  }

  String name;
  int count = 0;
  while (count < num && file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("name: ")) {
      name = line.substring(6);
      name.trim();
      count++;
    }
  }
  const char *value = strdup(name.c_str());
  file.close();
  return value;
}

int readEnableValue(const char *path, int num)
{
  int enable;
  File file = LittleFS.open(path, "r");
  if (!file) {
    ESP_LOGE("main", "Failed to open file");
  }

  String tmp;
  int count = 0;
  while (count < num && file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("enable: ")) {
      tmp = line.substring(8);
      count++;
    }
  }
  enable = tmp.toInt();
  file.close();
  return enable;
}