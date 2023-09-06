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

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void generateJsonCommandPost(const String& bridgeKey, const String& reqId, JsonArray ruleConfig, char jsonString[4096], char *macDevice);
void generateJsonCmdStatus(const String& bridgeKey, const String& reqId, char jsonString[1024], char *macDevice, const char *ep, boolean flag);
void responseGetStatus(const String& bridgeKey, const String& reqId, char jsonString[], char* macDevice);
void setRuleCmd(const String& reqId ,char jsonString[], char ruleId[], char* macDevice);
/******************************************************************************/