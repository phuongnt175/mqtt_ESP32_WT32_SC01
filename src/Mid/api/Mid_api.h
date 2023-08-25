#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <esp_log.h>
#include <ui.h>
#include <WiFi.h>
#include <Mid/mqtt/Mid_mqtt.h>

#define eepromTextVariableSize 33

//=======================================================================================
void saveWiFiToEEPPROM(char* ssid_, char* pass_);
void readWiFiFromEEPROM(char* ssid_, char* pass_);
void saveHCInfoToEEPPROM(char* iphc_, char* machc_);
void readHCInfoFromEEPROM(char* iphc_, char* machc_);
void writeEEPROM(int startAdr, int length, char* writeString);
void readEEPROM(int startAdr, int maxLength, char* dest);
void saveStatusToEeprom(byte value);
void eraseEEPROM();
byte getStatusFromEeprom();
void accessPoint_init();
void setupAP(void);
void handleAP();

//=======================================================================================

void addJsonObjectNet(String dns, String gw, String ssid, String ip, String netmask);
void addJsonObjectScan(String bssid, String ssid, int8_t rssi);
void getNetwork();
void scanNetwork();
void handlePut();
void setupApi();