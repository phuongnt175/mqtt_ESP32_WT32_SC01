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
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <esp_log.h>
#include <ui.h>
#include <Mid/jsonMessage/Mid_jsonMessage.h>

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

String randomReqId(int length, String& reqId);
void connectBroker();
void jsonPostCmdTask(void* pvParameters);

/******************************************************************************/