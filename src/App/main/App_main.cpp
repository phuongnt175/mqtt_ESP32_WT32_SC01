/*
 * Author: PhuongNT
 * Custom by PhuongNT.
 * Last Change: 25-08-2023
 */
/******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <Arduino.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <lvgl.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Mid/api/Mid_api.h>
#include <Mid/jsonMessage/Mid_jsonMessage.h>
#include <Mid/mqtt/Mid_mqtt.h>
#include <App/main/App_main.h>
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

extern String bridgeKey;
extern String reqId;
extern char output[4096];
extern byte mac[6];
extern char macAddress[18];

extern const char *statusTopic;
extern const char *controlTopic;
extern const char *configTopic;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
const char *ep1 = "-1";
const char *ep2 = "-3";
const char *ep3 = "-5";
const char *ep4 = "-7";

long lastTime = 0;

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
void ui_event_button(lv_event_t *e, ButtonStatus& btn_status, char *ep);

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

void subCallback(lv_obj_t *ui, char* message, ButtonStatus& btn_status, const char *ep)
{
  if(strstr(message, ON_MSG) != NULL)
  {
    ESP_LOGE("main", "ON");
    btn_status = ON;
    if(lv_obj_get_state(ui) == 6 || lv_obj_get_state(ui) == 0)
    {
      generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, true);
      client.publish(statusTopic, output);
      _ui_state_modify(ui, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
    }
    ESP_LOGE("main", "-------------------------------------------------------------");
  }
  else if(strstr(message, OFF_MSG) != NULL)
  {
    ESP_LOGE("main", "OFF");
    btn_status = OFF;
    if(lv_obj_get_state(ui) == 7 || lv_obj_get_state(ui) == 1)
    {
      generateJsonCmdStatus(bridgeKey, reqId, output, macAddress, ep, false);
      client.publish(statusTopic, output);
      lv_obj_clear_state(ui, LV_STATE_CHECKED);
      // _ui_state_modify(ui, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_REMOVE
    }
    ESP_LOGE("main", "-------------------------------------------------------------");
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

  if(String(topic) == controlTopic && strstr(message, "set") != NULL) //
  {
    
    if(json.find(s_ep1)!= std::string::npos)
    {
      subCallback(ui_button1, message, btnStatus1, ep1);
    }
    if(json.find(s_ep2)!= std::string::npos)
    {
      subCallback(ui_button2, message, btnStatus2, ep2);
    }
    if(json.find(s_ep3)!= std::string::npos)
    {
      subCallback(ui_button3, message, btnStatus3, ep3);
    }
    if(json.find(s_ep4)!= std::string::npos)
    {
      subCallback(ui_button4, message, btnStatus4, ep4);
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
      ESP_LOGE("main", "disconnected");
      eraseEEPROM();
      client.disconnect(); //disconnect from mqtt
      ESP.restart();

    }
  }
}

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(3);
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

  ui_init(); 
  setupAP();
  setupApi();

  WiFi.macAddress(mac);
  sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  if(!MDNS.begin("esp32")) {
    //Serial.println("Error starting mDNS");
    ESP_LOGE("main", "error starring mDNS!");
    return;
  }
  
  mDNSService();
  setupBroker(local_root_ca, iphc, PORT);
  client.setCallback(Callback);
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
    ESP_LOGE("main", "No services were found.");
  } 
  else {
    ESP_LOGE("main", "Number of services found: %d", nrOfServices);
    for (int i = 0; i < nrOfServices; i=i+1) 
    {
      ESP_LOGE("main", "---------------");
      ESP_LOGE("main", "Hostname: %s", MDNS.hostname(i));
      ESP_LOGE("main", "IP address: %s", MDNS.IP(i).toString().c_str());
      ESP_LOGE("main", "Port: %d", MDNS.port(i));
      ESP_LOGE("main", "MAC: %s", MDNS.txt(i, "mac").c_str());
      ESP_LOGE("main", "---------------");
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