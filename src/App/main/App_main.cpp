/*
 * Author: PhuongNT
 * Custom by PhuongNT.
 * Last Change: 25-08-2023
 */
/******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <Arduino.h>
#include <ESPmDNS.h>
#include <lvgl.h>
#include <App/main/App_main.h>
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_SCENE 10
#define SERVICE_NAME "airplay"
#define SERVICE_PROTOCOL "tcp"
#define SERVICE_PORT 5600

#define ON_MSG    "true"
#define OFF_MSG   "false"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
extern boolean accessPointMode;
extern char iphc[eepromTextVariableSize];
extern char machc[eepromTextVariableSize];

extern String bridgeKey;
extern String reqId;
extern char output[4096];
extern byte mac[6];
extern char macAddress[18];

extern char statusTopic[50];
extern char controlTopic[50];
extern char configTopic[50];
extern char deviceTopic[50];

extern bool receivedAckMessage;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
const char *ep1 = "-1";
const char *ep2 = "-3";
const char *ep3 = "-5";
const char *ep4 = "-7";
long lastTime = 0;
JsonArray ruleConfigValue_set;
uint8_t sceneNum;
int enableStatus;
const char *icon;
const char *name;

const char local_root_ca[] PROGMEM = R"=====(
-----BEGIN CERTIFICATE-----
MIIDqTCCApGgAwIBAgIJAK7m4E783cWuMA0GCSqGSIb3DQEBCwUAMGsxCzAJBgNV
BAYTAlZOMQswCQYDVQQIDAJITjELMAkGA1UEBwwCSE4xDTALBgNVBAoMBExVTUkx
CzAJBgNVBAsMAlJEMQ4wDAYDVQQDDAVsb2NhbDEWMBQGCSqGSIb3DQEJARYHYWJj
QDEyMzAeFw0xOTA5MjMxMDU0NDZaFw0yOTA5MjAxMDU0NDZaMGsxCzAJBgNVBAYT
AlZOMQswCQYDVQQIDAJITjELMAkGA1UEBwwCSE4xDTALBgNVBAoMBExVTUkxCzAJ
BgNVBAsMAlJEMQ4wDAYDVQQDDAVsb2NhbDEWMBQGCSqGSIb3DQEJARYHYWJjQDEy
MzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALoCzS2fxY3waRvIhewR
BIyBgkFQlagZYV7CGV0xorxHtjJs3+Q4nGxn/Xvl2dF4HE3WstJ+1JcSLYuLgpB3
Z9es68jvlCWX4zIa8Gne27mQSncdTuRV3K038RCKD8Ms00f1xd7cQFNZCHxPSdlV
TXydu4nXwsL8FAzOUXiHB7KT6s2F3JEPqhn9pgNGe7ciQZTfdTSEQPJ5w2wWrnnQ
7FccMxQmyyJNMfM3cwv26yhEFTIIKX9/9JCbqe75QIyeAxoTAUmJWtMvBBjT+HJ0
daGM1N60f6PsBYI4Y5o+NUsJa1ahPNvX44M4q/FxhbAyYOruztMyJFyYpiyxZpkO
8QsCAwEAAaNQME4wHQYDVR0OBBYEFO6WUjahqnRNCyzA54s78gae3LTsMB8GA1Ud
IwQYMBaAFO6WUjahqnRNCyzA54s78gae3LTsMAwGA1UdEwQFMAMBAf8wDQYJKoZI
hvcNAQELBQADggEBAF1ElC5P2hDpnaOiFarHkkVvvwrdry3H/jCckdffkUZFoAqa
AmeY1Zv4czcEVNDdkGh5nBSBlXxySvpR16Y6HM+4DlBlhv1FuBgR+LdHIU1jH86u
GNFX/Fq/jMv4rxBdJP9dgWnMW2vAnucU1DHqSgDD2TDFrvuz5EJADh73FKByYG7a
nVI0Ke+huGvqVv9ynmHBmWE1J4DjGD08IPypgTiS7GkRG3V/KpLpyV2M9FEAbmeP
KENRFSPMPeyRyfzitR98wTtsORlF4I1+fYcPGSh0pQK1mK1X1bI/BWmtnRMqBSXD
Eou01zV/f6o0PDqrnMlYhFi5gTg2bbqLYmLFgyw=
-----END CERTIFICATE-----
)=====";
//==============================================================================

//==============================================================================
extern WiFiClientSecure  net;
extern PubSubClient      client;
IPAddress         ADDRESS;

int PORT = 38883;
// bool res;
ButtonStatus btnStatus1 = OFF;
ButtonStatus btnStatus2 = OFF;
ButtonStatus btnStatus3 = OFF;
ButtonStatus btnStatus4 = OFF;

static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

extern lv_obj_t* ui_resetWifi;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void mDNSService();
void checkIpHCmDNS();
void ui_event_button(lv_event_t *e, ButtonStatus& btn_status, char *ep);
void ui_event_scene(lv_event_t *e, int num);
void spiffsInit();
void saveSceneNum(uint8_t value);
uint8_t getSceneNum();
void sceneModify(lv_obj_t* ui, lv_obj_t* ui_label, int num);
void writeRuleId(JsonArray json);
void updateScene(uint8_t num);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
//hàm đọc giá trị cảm ứng
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
  uint16_t touchX, touchY;
  if (tft.getTouch(&touchX, &touchY)) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;

  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void SubCallback(lv_obj_t *ui, char* message, ButtonStatus& btn_status, const char *ep)
{
  if(strstr(message, ON_MSG) != NULL)
  {
    btn_status = ON;
    if(lv_obj_get_state(ui) == 6 || lv_obj_get_state(ui) == 0)
    {
      reqId = getReqId(message);
      generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, true);
      client.publish(statusTopic, output);
      _ui_state_modify(ui, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
    }
  }
  else if(strstr(message, OFF_MSG) != NULL)
  {
    btn_status = OFF;
    reqId = getReqId(message);
    generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, false);
    client.publish(statusTopic, output);
    _ui_state_modify(ui, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_TOGGLE
  }
}

void Callback(char* topic, byte* payload, unsigned int length) {

  payload[length+1] = '\0'; //NULL terminator used to terminate the char array
  char* message = (char*)payload;
  std::string json = std::string(message);
  char hash1[50];
  char hash3[50];
  char hash5[50];
  char hash7[50];

  sprintf(hash1, "%s-%s%s", bridgeKey.c_str(), macAddress, ep1);
  sprintf(hash3, "%s-%s%s", bridgeKey.c_str(), macAddress, ep2);
  sprintf(hash5, "%s-%s%s", bridgeKey.c_str(), macAddress, ep3);
  sprintf(hash7, "%s-%s%s", bridgeKey.c_str(), macAddress, ep4);

  std::string s_ep1 = std::string(hash1);
  std::string s_ep2 = std::string(hash3);
  std::string s_ep3 = std::string(hash5);
  std::string s_ep4 = std::string(hash7);

  if(String(topic) == controlTopic && strstr(message, "set") != NULL) //
  {
    ESP_LOGE("main", "%s", message);
    if((json.find(s_ep1))!= std::string::npos)
    {
      SubCallback(ui_button1, message, btnStatus1, ep1);
    }
    if((json.find(s_ep2))!= std::string::npos)
    {
      SubCallback(ui_button2, message, btnStatus2, ep2);
    }
    if((json.find(s_ep3))!= std::string::npos)
    {
      SubCallback(ui_button3, message, btnStatus3, ep3);
    }
    if((json.find(s_ep4))!= std::string::npos)
    {
      SubCallback(ui_button4, message, btnStatus4, ep4);
    }
  }

  if(String(topic) == controlTopic && strstr(message, "get") != NULL) //response status when app send get message status
  {
    if(strstr(message, macAddress) != NULL)
    {
      reqId = getReqId(message);
      responseGetStatus(bridgeKey, reqId, output, macAddress);
      client.publish(statusTopic, output);
    }
  }

  if(String(topic) == configTopic && strstr(message, "delete") != NULL) //remove device
  {
    if(strstr(message, macAddress) != NULL)
    {
      ESP_LOGE("main", "disconnected");
      eraseEEPROM();
      WiFi.disconnect(true);// disconnect wifi 
      client.disconnect(); // disconnect mqtt
      _ui_flag_modify(ui_Notify, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE); //display Notify
      return setupApi(); // check wifi connection and active softAP
    }
  }

  if(String(topic) == configTopic && strstr(message, "set_scene") != NULL) //add delete scene
  {
    ESP_LOGE("main", "%s", message);
    const size_t jsonSize = strlen(message) + 1; // Add 1 for null terminator
    char* json = new char[jsonSize];
    strncpy(json, message, jsonSize);

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
      ESP_LOGE("main", "deserializeJson() failed: %s", error.c_str());
    }
    JsonArray ruleConfigValue_set = doc["objects"][0]["data"][0]["params"][0]["ruleConfig"];
    sceneNum = ruleConfigValue_set.size();
    saveSceneNum(sceneNum);
    sceneNum = getSceneNum();
    ESP_LOGE("main", "Number of scene: %d", sceneNum);
    String ruleConfig;
    serializeJson(ruleConfigValue_set, ruleConfig);
    writeJsonToFile("/data.txt", ruleConfig);
    reqId = getReqId(message);
    advanceStatusCmd(bridgeKey, reqId, ruleConfig, output, macAddress, machc);
    client.publish(statusTopic, output);

    writeRuleId(ruleConfigValue_set);
    updateScene(sceneNum);
  }

  if(String(topic) == configTopic && strstr(message, "get_scene") != NULL) //response get mess
  {
    String ruleConfig;
    ruleConfig = readJsonFromFile("/data.txt");
    reqId = getReqId(message);
    advanceStatusCmd(bridgeKey, reqId, ruleConfig, output, macAddress, machc);
    client.publish(statusTopic, output);
  }

  if(String(topic) == statusTopic && strstr(message, "ack") != NULL)
  {
    receivedAckMessage = true;
  }

  if(String(topic) == deviceTopic && strstr(message, "change_scene") != NULL)
  {
    ESP_LOGE("main", "%s", message);
    const size_t jsonSize = strlen(message) + 1; // Add 1 for null terminator
    char* json = new char[jsonSize];
    strncpy(json, message, jsonSize);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
      ESP_LOGE("main", "deserializeJson() failed: %s", error.c_str());
    }
    const char *ruleIdValue = doc["objects"][0]["data"][0]["params"][0]["ruleConfig"][0]["ruleid"];
    ESP_LOGE("main", "ruleid change: %s", ruleIdValue);
    const char *nameValue = doc["objects"][0]["data"][0]["params"][0]["ruleConfig"][0]["name"];
    int enableValue = doc["objects"][0]["data"][0]["params"][0]["ruleConfig"][0]["enable"];
    const char *iconKeyValue = doc["objects"][0]["data"][0]["params"][0]["ruleConfig"][0]["iconkey"];

    String ruleConfigTemp = readJsonFromFile("/data.txt");
    ESP_LOGE("main", "read old ruleconfig");

    DynamicJsonDocument doc1(1024);
    DeserializationError error1 = deserializeJson(doc1, ruleConfigTemp);
    if (error) {
      ESP_LOGE("main", "deserializeJson() failed: %s", error.c_str());
    }

    JsonArray ruleConfigArray = doc1.as<JsonArray>();

    for (JsonVariant ruleConfig : ruleConfigArray) {
    const char *currentRuleId = ruleConfig["ruleid"];

      if (strcmp(currentRuleId, ruleIdValue) == 0) {
      // The ruleId matches the ruleIdValue
      // Update the properties
      JsonObject ruleConfigObject = ruleConfig.as<JsonObject>();
      ruleConfigObject["name"] = nameValue;
      ruleConfigObject["enable"] = enableValue;
      ruleConfigObject["iconkey"] = iconKeyValue;
      }
    }
    String updateRuleConfig;
    serializeJson(ruleConfigArray, updateRuleConfig);
    sceneNum = ruleConfigArray.size();
    saveSceneNum(sceneNum);
    sceneNum = getSceneNum();
    ESP_LOGE("main", "Number of scene: %d", sceneNum);
    ESP_LOGE("main", "ruleconfig change: %s", updateRuleConfig.c_str());
    writeJsonToFile("/data.txt", updateRuleConfig);
    writeRuleId(ruleConfigArray);
    updateScene(sceneNum);
    reqId = getReqId(message);
    advanceStatusCmd(bridgeKey, reqId, updateRuleConfig, output, macAddress, machc);
    client.publish(statusTopic, output);
  }

  if(String(topic) == deviceTopic && strstr(message, "del_scene") != NULL)
  {
    const size_t jsonSize = strlen(message) + 1; // Add 1 for null terminator
    char* json = new char[jsonSize];
    strncpy(json, message, jsonSize);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
      ESP_LOGE("main", "deserializeJson() failed: %s", error.c_str());
    }
    const char *ruleIdValue = doc["objects"][0]["data"][0]["params"][0]["ruleid"];
    ESP_LOGE("main", "ruleid change: %s", ruleIdValue);
    String ruleConfigTemp = readJsonFromFile("/data.txt");
    DynamicJsonDocument doc1(1024);
    DeserializationError error1 = deserializeJson(doc1, ruleConfigTemp);
    if (error) {
      ESP_LOGE("main", "deserializeJson() failed: %s", error.c_str());
    }
    JsonArray ruleConfigArray = doc1.as<JsonArray>();

    for (size_t i = 0; i < ruleConfigArray.size(); i++) {
    // Get the current object
    JsonObject object = ruleConfigArray[i].as<JsonObject>();

    // Check if the object has a "ruleId" property
    if (object.containsKey("ruleid")) {
      // Compare the "ruleId" property with the provided ruleId
      if (strcmp(object["ruleid"].as<const char*>(), ruleIdValue) == 0) {
        // Remove the object at the current index
        ruleConfigArray.remove(i);
        break; // Exit the loop after removing the object
        }
      }
    }
    
    String updateRuleConfig;
    serializeJson(ruleConfigArray, updateRuleConfig);
    sceneNum = ruleConfigArray.size();
    saveSceneNum(sceneNum);
    sceneNum = getSceneNum();
    ESP_LOGE("main", "Number of scene: %d", sceneNum);
    ESP_LOGE("main", "ruleconfig change: %s", updateRuleConfig.c_str());
    writeJsonToFile("/data.txt", updateRuleConfig);
    writeRuleId(ruleConfigArray);
    updateScene(sceneNum);
    reqId = getReqId(message);
    advanceStatusCmd(bridgeKey, reqId, updateRuleConfig, output, macAddress, machc);
    client.publish(statusTopic, output);
  }
}

void uiTask(void *pvParameters)
{
    while (1) {
        //lv_refr_now(NULL);
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void setup() {
  Serial.begin(115200);
  spiffsInit();
  randomSeed(micros());

  tft.begin();
  tft.setRotation(3);
  tft.setBrightness(153);

  pinMode(45, OUTPUT);
  digitalWrite(45, 128);

  lv_init();
  xTaskCreate(uiTask, "uiTask", 10000, NULL, 1, NULL);
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();
  sceneNum = getSceneNum();
  ESP_LOGE("main", "Scene's number : %d", sceneNum);
  updateScene(sceneNum);
  setupAP();
  setupApi();
  WiFi.macAddress(mac);
  sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  sprintf(statusTopic, "component/deviceIP/%s/status", macAddress);
  sprintf(controlTopic, "component/deviceIP/%s/control", macAddress);
  sprintf(configTopic, "component/deviceIP/%s/config", macAddress);
  sprintf(deviceTopic, "component/deviceIP/config");

  if(!MDNS.begin("esp32")) {
    ESP_LOGE("main", "error starring mDNS!");
    return;
  }
  
  mDNSService();
  net.setInsecure();
  net.setCACert(local_root_ca);
  client.setKeepAlive(60);
  client.setBufferSize(4096);
  client.setServer(iphc, PORT);
  client.setCallback(Callback);
}

void loop() {
  long now = millis();
  handleAP();
  if(!client.connected())
  {
    client.setKeepAlive(60); // setting keep alive to 60 seconds
    if(accessPointMode == false)
    {
      connectBroker();
      checkIpHCmDNS();
    }
  }
  else{
    client.loop();
  }

  if(now - lastTime > 600000) // Send status periodically after 10 minutes
  {
    randomReqId( 15, reqId);
    responseGetStatus(bridgeKey, reqId, output, macAddress);
    client.publish(statusTopic, output);
    String ruleConfig;
    ruleConfig = readJsonFromFile("/data.txt");
    lastTime = now;
  }
  vTaskDelay(pdMS_TO_TICKS(100));
}

void mDNSService()
{ 
  int nrOfServices = MDNS.queryService("lumismarthome", SERVICE_PROTOCOL);
  if (nrOfServices == 0) {
    ESP_LOGE("main", "No services were found.");
  } 
  else {
    ESP_LOGE("main", "Number of services found: %d", nrOfServices);
    for (int i = 0; i < nrOfServices; i=i+1) 
    {
      ESP_LOGE("main", " ");
      ESP_LOGE("main", "Hostname: %s", MDNS.hostname(i));
      ESP_LOGE("main", "IP address: %s", MDNS.IP(i).toString().c_str());
      ESP_LOGE("main", "Port: %d", MDNS.port(i));
      ESP_LOGE("main", "MAC: %s", MDNS.txt(i, "mac").c_str());
    }
  }
}

void checkIpHCmDNS()
{
  int nrOfServices = MDNS.queryService("lumismarthome", SERVICE_PROTOCOL);
  if (nrOfServices == 0) {
    return;
  } 
  else {
    ESP_LOGE("main", "Number of services found: %d", nrOfServices);
    for (int i = 0; i < nrOfServices; i++) 
    {
      String macString = MDNS.txt(i, "mac");
      for (int j = 0; j < 18; j++) {
        macString[j] = toupper(macString[j]);
      }
      const char* scanMac = macString.c_str();
      ESP_LOGE("main", "%s", scanMac);
      char scanIP[16];
      sprintf(scanIP, "%s", MDNS.IP(i).toString().c_str());
      ESP_LOGE("main", "%s", scanIP);
      if(strcmp(machc, scanMac) == 0)
      {
        saveHCInfoToEEPPROM(scanIP, machc);
        readHCInfoFromEEPROM(iphc, machc);
        client.setServer(iphc, PORT);
        ESP_LOGE("main", "change iphc");
        return connectBroker();
      }
    }
  }
}

void spiffsInit()
{
  if (!LittleFS.begin()) {
    ESP_LOGE("main","Failed to initialize LittleFS");
    return;
  }

  File root = LittleFS.open("/");
  if (!root.isDirectory()) {
    ESP_LOGE("main", "Failed to open directory");
    return;
  }

  // Iterate through each file in the root directory
  File file = root.openNextFile();
  while (file) {
    // Print the name of each file
    ESP_LOGE("main", "%s", String(file.name()).c_str());

    // Close the current file
    file.close();

    // Open the next file
    file = root.openNextFile();
  }

  ESP_LOGE("main", "data read: %s", String(readJsonFromFile("/data.txt")).c_str());


  size_t totalBytes = LittleFS.totalBytes();
  size_t usedBytes = LittleFS.usedBytes();

  ESP_LOGE("main", "total bytes: %d", totalBytes);
  ESP_LOGE("main", "used bytes: %d", usedBytes);
}

void saveSceneNum(uint8_t value)
{
  EEPROM.begin(200);
  EEPROM.put(190, value);
  EEPROM.commit();
  EEPROM.end();
}

uint8_t getSceneNum() {
  uint8_t value;
  EEPROM.begin(200);
  EEPROM.get(190, value);
  EEPROM.end();
  return value;
}
void sceneModify(lv_obj_t* ui, lv_obj_t* ui_label, int num)
{
  const lv_img_dsc_t* image_array[40] = {&ui_img_minus1_png, &ui_img_0_png, &ui_img_1_png, &ui_img_2_png, &ui_img_3_png, &ui_img_4_png, &ui_img_5_png,
  &ui_img_6_png, &ui_img_7_png, &ui_img_8_png, &ui_img_9_png, &ui_img_10_png, &ui_img_11_png, &ui_img_12_png, &ui_img_13_png, &ui_img_14_png,
  &ui_img_15_png, &ui_img_16_png, &ui_img_17_png, &ui_img_18_png, &ui_img_19_png, &ui_img_20_png, &ui_img_21_png, &ui_img_22_png, &ui_img_23_png,
  &ui_img_24_png, &ui_img_25_png, &ui_img_26_png, &ui_img_27_png, &ui_img_28_png, &ui_img_29_png, &ui_img_30_png, &ui_img_31_png, &ui_img_32_png,
  &ui_img_33_png, &ui_img_34_png, &ui_img_35_png, &ui_img_36_png, &ui_img_37_png, &ui_img_38_png};

  enableStatus = readEnableValue("/ruleId.txt", num);
  icon = readIconKeyValue("/ruleId.txt", num);
  name = readNameValue("/ruleId.txt", num);
  int iconValue = std::stoi(icon) + 1;

  _ui_state_modify(ui, LV_STATE_DISABLED, enableStatus);

  lv_obj_set_style_bg_img_src( ui, image_array[iconValue], LV_PART_MAIN | LV_STATE_DEFAULT );
  lv_label_set_text(ui_label, name);

  free((void*)name);
  free((void*)icon);
}

void updateScene(uint8_t num) {
  lv_obj_t* ui_scenes[] = {ui_scene1, ui_scene2, ui_scene3, ui_scene4, ui_scene5, ui_scene6, ui_scene7, ui_scene8, ui_scene9, ui_scene10}; // can fix more scenes
  lv_obj_t* ui_canhs[] = {ui_canh1, ui_canh2, ui_canh3, ui_canh4, ui_canh5, ui_canh6, ui_canh7, ui_canh8, ui_canh9, ui_canh10};

  for(uint8_t i = num; i < MAX_SCENE; i++) //i < max_scene
  {
    ESP_LOGE("main", "scene number: %d", i+1);
    _ui_flag_modify(ui_scenes[i], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
  }
  for (uint8_t i = 0; i < num; i++) {
    ESP_LOGE("main", "i = %d", i);
    ESP_LOGE("main", "num = %d", num);
    _ui_flag_modify(ui_scenes[i], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
  }

  for (uint8_t i = 0; i < num; i++) {
    ESP_LOGE("main", "i = %d", i);
    ESP_LOGE("main", "num = %d", num);
    sceneModify(ui_scenes[i], ui_canhs[i], i + 1);
  }
  lv_obj_set_y( ui_back, num*320 );
  if(num == 0)
  {
    lv_obj_set_style_bg_img_src(ui_scenes[0], NULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    _ui_flag_modify(ui_scenes[0], LV_OBJ_FLAG_HIDDEN, 1);
    lv_label_set_text(ui_canhs[0], "Không có cảnh");
    lv_obj_set_y( ui_back, 320);
  }
}

void writeRuleId(JsonArray json)
{
  File file = LittleFS.open("/ruleId.txt", "w");
      if (!file) {
        Serial.println("Failed to open file");
        return;
    }
    for(JsonVariant value : json) {
      JsonObject jsonObject = value.as<JsonObject>();

      int enable = jsonObject["enable"];
      const char* name = jsonObject["name"];
      int  iconkey = jsonObject["iconkey"];
      const char* ruleid = jsonObject["ruleid"];
      file.print("enable: ");
      file.println(enable);
      file.print("name: ");
      file.println(name);
      file.print("iconkey: ");
      file.println(iconkey);
      file.print("ruleid: ");
      file.println(ruleid);
      file.println();
    }
    file.close();
    String text = readJsonFromFile("/ruleId.txt");
    ESP_LOGE("main", "%s", text.c_str());
}
//UI_EVENT_HANDLE
//==============================================================================

//==============================================================================

void ui_event_button(lv_event_t *e, ButtonStatus& btn_status, const char *ep) {
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  
  if (event_code == LV_EVENT_VALUE_CHANGED) {
    if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
      if (btn_status == OFF) {
        randomReqId( 15, reqId);
        generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, true);
        client.publish(statusTopic, output);
        btn_status = ON;
      }
    } else {
      if (btn_status == ON) {
        randomReqId( 15, reqId);
        generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, false);
        client.publish(statusTopic, output);
        btn_status = OFF;
      }
    }
  }
}

void ui_event_sceneSetting(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_SettingScreen, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_SettingScreen_screen_init);
    }
}

void ui_event_back(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_mainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, &ui_mainScreen_screen_init);
    }
}

void ui_event_scene(lv_event_t *e, int num)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if(event_code == LV_EVENT_CLICKED && num <= sceneNum)
  {
      const char* ruleId = readRuleIDValue("/ruleId.txt", num);
      randomReqId( 15, reqId);
      activeRuleCmd(reqId, output, ruleId);
      client.publish(controlTopic, output);
  }
}

void ui_event_scene1(lv_event_t *e)
{
  ui_event_scene(e, 1);
}

void ui_event_scene2(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 2);
}

void ui_event_scene3(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 3);
}

void ui_event_scene4(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 4);
}

void ui_event_scene5(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 5);
}

void ui_event_scene6(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 6);
}

void ui_event_scene7(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 7);
}

void ui_event_scene8(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 8);
}

void ui_event_scene9(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 9);
}

void ui_event_scene10(lv_event_t *e)
{
  //code here
  ui_event_scene(e, 10);
}

void ui_event_button1(lv_event_t *e) {
  ui_event_button(e, btnStatus1, ep1);
}

void ui_event_button2(lv_event_t *e) {
  ui_event_button(e, btnStatus2, ep2);
}

void ui_event_button3(lv_event_t *e) {
  ui_event_button(e, btnStatus3, ep3);
}

void ui_event_button4(lv_event_t *e) {
  ui_event_button(e, btnStatus4, ep4);
}

/******************************************************************************/