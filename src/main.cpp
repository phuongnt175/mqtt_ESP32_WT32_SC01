#include <lvgl.h>
#include <lv_conf.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <FT6236.h>
#include <ui.h>
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>

#define SDA_FT6236 18
#define SCL_FT6236 19
FT6236 ts = FT6236();

String btnStatus1 = "OFF";
String btnStatus2 = "OFF";
String btnStatus3 = "OFF";
String btnStatus4 = "OFF";

const char *ssid = "LUMI_TEST";
const char *password = "lumivn274!";

#define MQTT_SERVER       "broker.hivemq.com"
#define MQTT_PORT         1883
#define MQTT_USER         "gnouhp"
#define MQTT_PASSWORD     "Phuong98"

const char *MQTT_BTN1_TOPIC = "MQTT_ESP32/BTN1";
const char *MQTT_BTN2_TOPIC = "MQTT_ESP32/BTN2";
const char *MQTT_BTN3_TOPIC = "MQTT_ESP32/BTN3";
const char *MQTT_BTN4_TOPIC = "MQTT_ESP32/BTN4";

WiFiClient  wifiClient;
PubSubClient  client(wifiClient);

void controlHandler(void);
void btnHandler(lv_obj_t *ui, String btnStatus, const char *topic);

extern lv_obj_t * ui_secondButton;
extern lv_obj_t * ui_firstButton;
extern lv_obj_t * ui_thirdButton;
extern lv_obj_t * ui_forthButton;
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Screen2;
extern lv_obj_t * ui_Screen3;
extern lv_obj_t * ui_Screen4;
extern lv_obj_t * ui_Colorwheel1;
class LGFX : public lgfx::LGFX_Device
{
lgfx::Panel_ST7796     _panel_instance;
lgfx::Bus_SPI       _bus_instance;   
lgfx::Light_PWM     _light_instance;

public:
  LGFX(void)
  {
    { 
      auto cfg = _bus_instance.config();    
      cfg.spi_host = VSPI_HOST;    
      cfg.spi_mode = 0;            
      cfg.freq_write = 40000000;   //40000000
      cfg.freq_read  = 16000000;    //16000000
      cfg.spi_3wire  = false;       
      cfg.use_lock   = true;        
      cfg.dma_channel = 1;         
      cfg.pin_sclk = 14;      //cau hinh chan SPI     
      cfg.pin_mosi = 13;          
      cfg.pin_miso = 12;          
      cfg.pin_dc   = 21;          
      _bus_instance.config(cfg);    
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();   
      cfg.pin_cs           =    15;  
      cfg.pin_rst          =    22;  
      cfg.pin_busy         =    -1;  
      cfg.memory_width     =   320;  
      cfg.memory_height    =   480;  
      cfg.panel_width      =   320;  
      cfg.panel_height     =   480;  
      cfg.offset_x         =     0; 
      cfg.offset_y         =     0;  
      cfg.offset_rotation  =     0; 
      cfg.dummy_read_pixel =     8;  
      cfg.dummy_read_bits  =     1;  
      cfg.readable         =  true;
      cfg.invert           = false;    
      cfg.rgb_order        = false;    
      cfg.dlen_16bit       = false;  
      cfg.bus_shared       =  true;  

      _panel_instance.config(cfg);
    }
    
    {  
      auto cfg = _light_instance.config();

      cfg.pin_bl = 23;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    setPanel(&_panel_instance);
  }
};

LGFX tft;

extern int brightnessValue;


/*Change to your screen resolution*/
static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
   if(ts.touched()){
     data->state = LV_INDEV_STATE_PR;
     TS_Point p = ts.getPoint();
     data->point.x = p.y;
     data->point.y = tft.height() - p.x;
   }else{
     data->state = LV_INDEV_STATE_REL;
   }
}

void connectBroker()
{
  while(!client.connected())
  {
    Serial.print("Attemping MQTT connection...");
    String clientId = "esp32";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str()))
    {
      Serial.println("Connected");
      client.subscribe(MQTT_BTN1_TOPIC);
      client.subscribe(MQTT_BTN2_TOPIC);
      client.subscribe(MQTT_BTN3_TOPIC);
      client.subscribe(MQTT_BTN4_TOPIC);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 2 seconds");
      delay(200);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length)
{
  char status[20];
  Serial.println("==========new message from broker==========");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: ");
  Serial.write(payload, length);
  Serial.println();
  for(int i =0; i< length; i++)
  {
    status[i] = payload[i];
  }
  Serial.println(status);
  if(String(topic) == MQTT_BTN1_TOPIC)
  {
    if(String(status) == "OFF")
    {
      btnStatus1 = "OFF";
      Serial.println(lv_obj_get_state(ui_firstButton));
      _ui_state_modify(ui_firstButton, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_REMOVE
      Serial.println("Living Room Light OFF");
    }
    else if(String(status) == "ON")
    {
      btnStatus1 = "ON";
      Serial.println(lv_obj_get_state(ui_firstButton));
      if(lv_obj_get_state(ui_firstButton) == 2 || lv_obj_get_state(ui_firstButton) == 0)
      {
      _ui_state_modify(ui_firstButton, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
      }
      Serial.println("Living Room Light ON");
    }
  }

  if(String(topic) == MQTT_BTN2_TOPIC)
  {
    if(String(status) == "OFF")
    {
      btnStatus2 = "OFF";
      Serial.println(lv_obj_get_state(ui_secondButton));
      _ui_state_modify(ui_secondButton, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_REMOVE
      Serial.println("Kitchen Room Light OFF");
    }
    else if(String(status) == "ON")
    {
      btnStatus2 = "ON";
      Serial.println(lv_obj_get_state(ui_secondButton));
      if(lv_obj_get_state(ui_secondButton) == 2 || lv_obj_get_state(ui_secondButton) == 0)
      {
        _ui_state_modify(ui_secondButton, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
      }
      Serial.println("Kitchen Room Light ON");
    }
  }

  if(String(topic) == MQTT_BTN3_TOPIC)
  {
    if(String(status) == "OFF")
    {
      btnStatus3 = "OFF";
      Serial.println(lv_obj_get_state(ui_thirdButton));
      _ui_state_modify(ui_thirdButton, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_REMOVE
      Serial.println("Curtain Mode OFF");
    }
    else if(String(status) == "ON")
    {
      btnStatus3 = "ON";
      Serial.println(lv_obj_get_state(ui_thirdButton));
      if(lv_obj_get_state(ui_thirdButton) == 2 || lv_obj_get_state(ui_thirdButton) == 0)
      {
        _ui_state_modify(ui_thirdButton, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
      }
      Serial.println("Curtain Mode ON");
    }
  }

  if(String(topic) == MQTT_BTN4_TOPIC)
  {
    if(String(status) == "OFF")
    {
      btnStatus4 = "OFF";
      Serial.println(lv_obj_get_state(ui_forthButton));
      _ui_state_modify(ui_forthButton, LV_STATE_CHECKED, 1);// _UI_STATE_MODIFY_REMOVE
      Serial.println("AC Mode OFF");
    }
    else if(String(status) == "ON")
    {
      btnStatus4 = "ON";
      Serial.println(lv_obj_get_state(ui_forthButton));
      if(lv_obj_get_state(ui_forthButton) == 2 || lv_obj_get_state(ui_forthButton) == 0)
      {
        _ui_state_modify(ui_forthButton, LV_STATE_CHECKED, 2);// _UI_STATE_MODIFY_TOGGLE
      }
      Serial.println("AC Mode ON");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(500);

  tft.begin();
  tft.setRotation(1);
  tft.setBrightness(255);

  if(!ts.begin(40, SDA_FT6236, SCL_FT6236)){
    Serial.println("Unable to start the capacitive touch Screen.");
  }

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

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connectBroker();
  Serial.println("Start transfer");

  ui_init();
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  controlHandler();
  
  tft.setBrightness(brightnessValue); //change screen brightness according to slider's value

  //set screen bg color acording to color wheel selected
  lv_obj_set_style_bg_color(ui_Screen1, lv_colorwheel_get_rgb(ui_Colorwheel1), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Screen2, lv_colorwheel_get_rgb(ui_Colorwheel1), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Screen3, lv_colorwheel_get_rgb(ui_Colorwheel1), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_Screen4, lv_colorwheel_get_rgb(ui_Colorwheel1), LV_PART_MAIN | LV_STATE_DEFAULT);

  delay( 5 );
}

void controlHandler(void)
{
  client.loop();
  if(!client.connected())
  {
    connectBroker();
  }

  btnHandler(ui_firstButton, btnStatus1, MQTT_BTN1_TOPIC);
  btnHandler(ui_secondButton, btnStatus2, MQTT_BTN2_TOPIC);
  btnHandler(ui_thirdButton, btnStatus3, MQTT_BTN3_TOPIC);
  btnHandler(ui_forthButton, btnStatus4, MQTT_BTN4_TOPIC);
  
}

void btnHandler(lv_obj_t *ui, String btnStatus, const char *topic)
{
  if(lv_obj_get_state(ui) == 34 || lv_obj_get_state(ui) == 35) // state 34 35 is when touch in LCD
  {
    delay(500);
    if(btnStatus == "ON")
    {
      client.publish(topic, "OFF");
      btnStatus = "OFF";
    }
    else if(btnStatus = "OFF")
    {
      client.publish(topic, "ON");
      btnStatus = "ON";
    }
  }
}