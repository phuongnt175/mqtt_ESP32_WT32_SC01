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

void generateJsonCommandPost(const String& bridgeKey, const String& reqId, JsonArray ruleConfig, char jsonString[4096], char *macDevice) 
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
    attr["deviceInfo"]["ModelId"] = "LM-SZDM4";

    JsonObject sceneConfig = attr.createNestedObject("sceneConfig");
    sceneConfig["ruleConfig"] = ruleConfig;

    // Create sceneSetting array and add objects
    JsonArray sceneSetting = attr.createNestedArray("sceneSetting");
    JsonObject sceneSettingItem = sceneSetting.createNestedObject();
    sceneSettingItem["name"] = "ruleConfig";
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

void setRuleCmd(const String& reqId ,char jsonString[], char ruleId[], char* macDevice) {

  StaticJsonDocument<1024> doc;
  doc.clear();

  doc["cmd"] = "set";

  JsonArray objects = doc.createNestedArray("objects");
  JsonObject object = objects.createNestedObject();

  object["type"] = "rules";

  JsonArray data = object.createNestedArray("data");
  data.add(ruleId);

  doc["reqid"] = reqId;
  doc["source"] = macDevice;

  serializeJson(doc, jsonString, 1024);
}

/******************************************************************************/