#include <Mid/mqtt/Mid_mqtt.h>

WiFiClientSecure  net;
PubSubClient      client(net);

char TAG[] = "mqtt";

String bridgeKey = "switch_ip";
String reqId = "abcxyz";

char output[4096];
byte mac[6];
char macAddress[18]; // Buffer to hold the concatenated MAC address

const char *MQTT_USER = "component";
const char *MQTT_PASS = " "; // leave blank if no credentials used
const char *statusTopic = "component/switch_ip/status";
const char *controlTopic = "component/switch_ip/control";
const char *configTopic = "component/switch_ip/config";

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