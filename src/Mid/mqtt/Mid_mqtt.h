#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <esp_log.h>
#include <Mid/jsonMessage/Mid_jsonMessage.h>

void connectBroker();
void setupBroker(const char *key, const char *ip, uint16_t port);