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
#include <Mid/mqtt/Mid_mqtt.h>

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
WiFiClientSecure  net;
PubSubClient      client(net);

String bridgeKey = "switchIP";
String reqId = "abcxyz";

JsonArray ruleconfig;

char output[4096];
char TAG[] = "mqtt";
char macAddress[18]; // Buffer to hold the concatenated MAC address

byte mac[6];

const char *MQTT_USER = "component";
const char *MQTT_PASS = " ";
const char *statusTopic = "component/switchIP/status";
const char *controlTopic = "component/switchIP/control";
const char *configTopic = "component/switchIP/config";

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void connectBroker()
{
  ESP_LOGE(TAG, "Attemping MQTT connection...");
  String clientId = "SW-";
  clientId += String(macAddress);
  if(client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS))
  {
    client.subscribe(statusTopic);
    client.subscribe(controlTopic);
    client.subscribe(configTopic);
    ESP_LOGE(TAG, "Connected");
    _ui_flag_modify(ui_Notify, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    _ui_flag_modify(ui_mqttNotify, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    generateJsonCommandPost(bridgeKey.c_str(), reqId.c_str(), ruleconfig, output, macAddress);
    client.publish(configTopic, output);
    responseGetStatus(bridgeKey, reqId, output, macAddress);
    client.publish(statusTopic, output);
  }
  else
  {
    _ui_flag_modify(ui_mqttNotify, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    ESP_LOGE(TAG, "Error rc = %d", client.state());
    ESP_LOGE(TAG, "try again in 2 seconds");
    delay(200);
  }
}

void setupBroker(const char *key, const char *ip, uint16_t port)
{
  net.setInsecure();
  net.setCACert(key);
  client.setKeepAlive(60);
  client.setBufferSize(4096);
  client.setServer(ip, port);
}

/******************************************************************************/