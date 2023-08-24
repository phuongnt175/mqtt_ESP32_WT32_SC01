/*
 * Author: PhuongNT
 * Custom by PhuongNT.
 * Last Change: 21-08-2023
 */
/******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Mid/button/button.hpp>
#include <Mid/mt8901/mt8901.hpp>
#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <lvgl.h>
#include <Mid/api/api.h>
#include <App/main/main.h>
#include <string>
// #include <App/CmdMessage/cmdPost.hpp>
// #include <App/CmdMessage./cmdSet.hpp>
// #include <App/CmdMessage/cmdStatus.hpp>

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/

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

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
const char *MQTT_USER = "component"; // leave blank if no credentials used
const char *MQTT_PASS = " "; // leave blank if no credentials used
const char *statusTopic = "component/switch_ip/status";
const char *controlTopic = "component/switch_ip/control";
const char *configTopic = "component/switch_ip/config";

const char *ep1 = "-1";
const char *ep2 = "-3";
const char *ep3 = "-5";
const char *ep4 = "-7";

char output[4096];
byte mac[6];
char macAddress[18]; // Buffer to hold the concatenated MAC address
long lastTime = 0;
//==============================================================================

//==============================================================================
WiFiClientSecure  net;
PubSubClient      client(net);
IPAddress         ADDRESS;

int PORT = 38883;
// bool res;
ButtonStatus btnStatus1 = OFF;
ButtonStatus btnStatus2 = OFF;
ButtonStatus btnStatus3 = OFF;
ButtonStatus btnStatus4 = OFF;

String bridgeKey = "switch_ip";
String reqId = "abcxyz";

static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

extern lv_obj_t* ui_resetWifi;

char TAG[] = "Main";

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

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void mDNSService();
void init_lv_group();
void ui_event_button(lv_event_t *e, ButtonStatus& btn_status, char *ep);
void generateJsonCommandPost(const String& bridgeKey, const String& reqId, char jsonString[4096], char *macDevice);
void generateJsonCmdStatus(const String& bridgeKey, const String& reqId, char jsonString[1024], char *macDevice, const char *ep, boolean flag);
void responseGetStatus(const String& bridgeKey, const String& reqId, char jsonString[], char* macDevice);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

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

void connectBroker()
{
  while(!client.connected())
  {
    ESP_LOGE(TAG, "Attemping MQTT connection...");
    String clientId = "SW-";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS))
    {
      client.subscribe(statusTopic);
      client.subscribe(controlTopic);
      client.subscribe(configTopic);
      ESP_LOGE(TAG, "Connected");
      generateJsonCommandPost(bridgeKey.c_str(), reqId.c_str(), output, macAddress);
      client.publish(configTopic, output);
      responseGetStatus(bridgeKey, reqId, output, macAddress);
      client.publish(statusTopic, output);
    }
    else
    {
      ESP_LOGE(TAG, "Error rc = %d", client.state());
      ESP_LOGE(TAG, "try again in 2 seconds");
      delay(200);
    }
  }
}

void SubCallback(lv_obj_t *ui, char* message, ButtonStatus& btn_status, const char *ep)
{
  if(strstr(message, ON_MSG) != NULL)
  {
    ESP_LOGE(TAG, "ON");
    btn_status = ON;
    if(lv_obj_get_state(ui) == 6 || lv_obj_get_state(ui) == 0)
    {
      generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, true);
      client.publish(statusTopic, output);
      _ui_state_modify(ui, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
    }
    ESP_LOGE(TAG, "-------------------------------------------------------------");
  }
  else if(strstr(message, OFF_MSG) != NULL)
  {
    ESP_LOGE(TAG, "OFF");
    btn_status = OFF;
    if(lv_obj_get_state(ui) == 7 || lv_obj_get_state(ui) == 1)
    {
      generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, false);
      client.publish(statusTopic, output);
      lv_obj_clear_state(ui, LV_STATE_CHECKED);
      // _ui_state_modify(ui, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_REMOVE
    }
    ESP_LOGE(TAG, "-------------------------------------------------------------");
  }
}

void Callback(char* topic, byte* payload, unsigned int length) {

  payload[length+1] = '\0'; //NULL terminator used to terminate the char array
  char* message = (char*)payload;
  std::string json = std::string(message);
  char endpoint1[50];
  char endpoint2[50];
  char endpoint3[50];
  char endpoint4[50];

  sprintf(endpoint1, "%s-%s%s", bridgeKey.c_str(), macAddress, ep1);
  sprintf(endpoint2, "%s-%s%s", bridgeKey.c_str(), macAddress, ep2);
  sprintf(endpoint3, "%s-%s%s", bridgeKey.c_str(), macAddress, ep3);
  sprintf(endpoint4, "%s-%s%s", bridgeKey.c_str(), macAddress, ep4);

  std::string s_ep1 = std::string(endpoint1);
  std::string s_ep2 = std::string(endpoint2);
  std::string s_ep3 = std::string(endpoint3);
  std::string s_ep4 = std::string(endpoint4);

  ESP_LOGE(TAG, "%s", endpoint1);
  ESP_LOGE(TAG, "%s", endpoint2);
  ESP_LOGE(TAG, "%s", endpoint3);
  ESP_LOGE(TAG, "%s", endpoint4);

  if(String(topic) == controlTopic && strstr(message, "set") != NULL) //
  {
    
    if((json.find(s_ep1))!= std::string::npos)
    {
      ESP_LOGE(TAG, "1");
      SubCallback(ui_button1, message, btnStatus1, ep1);
    }
    if(json.find(s_ep2)!= std::string::npos)
    {
      ESP_LOGE(TAG, "2");
      SubCallback(ui_button2, message, btnStatus2, ep2);
    }
    if(json.find(s_ep3)!= std::string::npos)
    {
      ESP_LOGE(TAG, "3");
      SubCallback(ui_button3, message, btnStatus3, ep3);
    }
    if(json.find(s_ep4)!= std::string::npos)
    {
      ESP_LOGE(TAG, "4");
      SubCallback(ui_button4, message, btnStatus4, ep4);
    }
  }

  if(String(topic) == controlTopic && strstr(message, "get") != NULL) //response status when app send get message status
  {
    if(strstr(message, macAddress) != NULL)
    {
      responseGetStatus(bridgeKey, reqId, output, macAddress);
      client.publish(statusTopic, output);
    }
  }

  if(String(topic) == configTopic && strstr(message, "delete") != NULL) //remove device
  {
    if(strstr(message, macAddress) != NULL)
    {
      ESP_LOGE(TAG, "disconnected");
      eraseEEPROM();
      client.disconnect(); //disconnect from mqtt
      ESP.restart();

    }
  }
}

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  tft.setBrightness(153);

  pinMode(45, OUTPUT);
  digitalWrite(45, 128);

  lv_init();
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


  setupAP();
  setupApi();

  WiFi.macAddress(mac);
  sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  if(!MDNS.begin("esp32")) {
    //Serial.println("Error starting mDNS");
    ESP_LOGE(TAG, "error starring mDNS!");
    return;
  }
  
  mDNSService();
  net.setInsecure();
  net.setCACert(local_root_ca);
  client.setKeepAlive(60);
  client.setBufferSize(4096);
  client.setServer(iphc, PORT);
  client.setCallback(Callback);
  ui_init();
}

void loop() {
  long now = millis();
  lv_timer_handler();
  handleAP();
  if(!client.connected())
  {
    client.setKeepAlive(60); // setting keep alive to 60 seconds
    if(accessPointMode == false)
    {
      connectBroker();
    }
  }
  else{
    client.loop();
  }

  if(now - lastTime > 600000) // Send status periodically after 10 minutes
  {
    responseGetStatus(bridgeKey, reqId, output, macAddress);
    client.publish(statusTopic, output);
    lastTime = now;
  }
}

void mDNSService()
{
  MDNS.addService(SERVICE_NAME, SERVICE_PROTOCOL, SERVICE_PORT);
  MDNS.addServiceTxt(SERVICE_NAME, SERVICE_PROTOCOL, "manufacturer", "LUMI");
  MDNS.addServiceTxt(SERVICE_NAME, SERVICE_PROTOCOL, "mac", "f4:12:fa:cf:4e:b4");
  
  int nrOfServices = MDNS.queryService("lumismarthome", SERVICE_PROTOCOL);
  if (nrOfServices == 0) {
    ESP_LOGE(TAG, "No services were found.");
  } 
  else {
    ESP_LOGE(TAG, "Number of services found: %d", nrOfServices);
    for (int i = 0; i < nrOfServices; i=i+1) 
    {
      ESP_LOGE(TAG, "---------------");
      ESP_LOGE(TAG, "Hostname: %s", MDNS.hostname(i));
      ESP_LOGE(TAG, "IP address: %s", MDNS.IP(i).toString().c_str());
      ESP_LOGE(TAG, "Port: %d", MDNS.port(i));
      ESP_LOGE(TAG, "MAC: %s", MDNS.txt(i, "mac").c_str());
      ESP_LOGE(TAG, "---------------");
    }
  }
}

void ui_event_button(lv_event_t *e, ButtonStatus& btn_status, const char *ep) {
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  
  if (event_code == LV_EVENT_VALUE_CHANGED) {
    if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
      if (btn_status == OFF) {
        generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, true);
        client.publish(statusTopic, output);
        btn_status = ON;
      }
    } else {
      if (btn_status == ON) {
        generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, false);
        client.publish(statusTopic, output);
        btn_status = OFF;
      }
    }
  }
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