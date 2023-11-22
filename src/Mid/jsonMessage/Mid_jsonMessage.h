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
#include <ArduinoJson.h>
#include <ui.h>
#include <FS.h>
#include <LITTLEFS.h>

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

String getReqId(char* message);
void generateJsonCommandPost(const String& bridgeKey, String& reqId, char jsonString[], char *macDevice);
void generateJsonCmdStatus(const String& bridgeKey, String& reqId, char jsonString[], char *macDevice, const char *ep, boolean flag);
void responseGetStatus(const String& bridgeKey, String& reqId, char jsonString[], char* macDevice);
void advanceStatusCmd(const String& bridgeKey, String& reqId, String ruleConfig, char jsonString[], char *macDevice, char *macHC);
void activeRuleCmd(String& reqId ,char jsonString[], const char ruleId[]);

void writeJsonToFile(const char *path, String output);
void appendJsonToFile(const char *path, String output);
String readJsonFromFile(const char *path);

const char *readRuleIDValue(const char *path, int num);
const char *readIconKeyValue(const char *path, int num);
const char *readNameValue(const char *path, int num);
int readEnableValue(const char *path, int num);
/******************************************************************************/