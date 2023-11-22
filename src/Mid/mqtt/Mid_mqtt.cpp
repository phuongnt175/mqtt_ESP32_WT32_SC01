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

char TAG[] = "mqtt";

String bridgeKey = "deviceIP";
String reqId;

TaskHandle_t postHandle;

bool receivedAckMessage = false;

char output[4096];
byte mac[6];
char macAddress[18]; // Buffer to hold the concatenated MAC address

const char *MQTT_USER = "component";
const char *MQTT_PASS = " "; // leave blank if no credentials used
char statusTopic[50];
char controlTopic[50];
char configTopic[50];
char deviceTopic[50];

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

String randomReqId(int length, String& reqId) {
  String characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  
  reqId = "";

  for (int i = 0; i < length; i++) {
    reqId += characters[random(0, characters.length())];
  }
  return reqId;
}

void connectBroker()
{
  ESP_LOGE(TAG, "Attemping MQTT connection...");
  String clientId = "SW-";
  clientId += String(random(0xffff), HEX);
  if(client.connect(clientId.c_str(), MQTT_USER, MQTT_PASS))
  {
    client.subscribe(statusTopic);
    client.subscribe(controlTopic);
    client.subscribe(configTopic);
    client.subscribe(deviceTopic);
    ESP_LOGE(TAG, "Connected");
    if (postHandle == NULL) {
      xTaskCreate(jsonPostCmdTask, "JsonPostTask", 10000, NULL, 1, &postHandle);
    }
  }
  else
  {
    ESP_LOGE(TAG, "Error rc = %d", client.state());
    ESP_LOGE(TAG, "try again in 2 seconds");
    delay(200);
  }
}

void jsonPostCmdTask(void* pvParameters) {

  const TickType_t delayPeriod = pdMS_TO_TICKS(5000); // 5 seconds

  String ruleConfig;
  ruleConfig = readJsonFromFile("/data.txt");
  
  int attemptCount = 0;
  bool stopTask = false;

  while (attemptCount < 5 && stopTask == false) {
    ESP_LOGE("mqtt", "%d", receivedAckMessage);
    // Check for acknowledgement message
    if (receivedAckMessage == true) {

      randomReqId( 15, reqId);
      responseGetStatus(bridgeKey, reqId, output, macAddress);
      client.publish(statusTopic, output);

      stopTask = true; // Set stopTask flag to true to exit the loop
    } else {
      randomReqId( 15, reqId);
      ESP_LOGE("mqtt", "reqId: %s", reqId.c_str());
      generateJsonCommandPost(bridgeKey, reqId, output, macAddress);
      client.publish(configTopic, output);
      attemptCount++;
      if (receivedAckMessage == false) {
        vTaskDelay(delayPeriod);
      }
    }
  }

  vTaskDelete(postHandle);
  ESP_LOGE("main", "delete postHandle");
}

/******************************************************************************/