// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: test

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_sc1MainPanel;
lv_obj_t * ui_secondButton;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Image1;
lv_obj_t * ui_firstButton;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Image2;
lv_obj_t * ui_thirdButton;
lv_obj_t * ui_Image3;
lv_obj_t * ui_Label4;
lv_obj_t * ui_forthButton;
lv_obj_t * ui_Image5;
lv_obj_t * ui_Label5;


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
void ui_event_Screen2(lv_event_t * e);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_sc2BrightnessPanel;
void ui_event_BrightnessSlider(lv_event_t * e);
lv_obj_t * ui_BrightnessSlider;
lv_obj_t * ui_BrightnessTi;
lv_obj_t * ui_Label6;
lv_obj_t * ui_sc2VolPanel;
void ui_event_muteButton(lv_event_t * e);
lv_obj_t * ui_muteButton;
lv_obj_t * ui_muteT;
void ui_event_lowButton(lv_event_t * e);
lv_obj_t * ui_lowButton;
lv_obj_t * ui_lowT;
void ui_event_highButton(lv_event_t * e);
lv_obj_t * ui_highButton;
lv_obj_t * ui_hightT;
lv_obj_t * ui_volTi;
lv_obj_t * ui_sc2SystemPanel;
void ui_event_SysInfoBut(lv_event_t * e);
lv_obj_t * ui_SysInfoBut;
lv_obj_t * ui_sysInfoT;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;
lv_obj_t * ui_Label7;

// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_sc3MainPanel;
lv_obj_t * ui_sysSpec;
void ui_event_backButton(lv_event_t * e);
lv_obj_t * ui_backButton;
lv_obj_t * ui_backT;

// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Colorwheel1;
void ui_event_Button1(lv_event_t * e);
lv_obj_t * ui_Button1;
lv_obj_t * ui_Label8;
lv_obj_t * ui____initial_actions0;
const lv_img_dsc_t * ui_imgset_30409_treecanopy_v1_[1] = {&ui_img_30409_treecanopy_v1_920485_png};
const lv_img_dsc_t * ui_imgset_2039529013[1] = {&ui_img_1862871016};

int brightnessValue = 50;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Screen1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0);
    }
}

void ui_event_Screen2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0);
    }
}
void ui_event_BrightnessSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_slider_set_text_value(ui_Label6, target, "", "");
    }
    brightnessValue = (int)lv_slider_get_value(target);
}
void ui_event_muteButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_state_modify(ui_lowButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
        _ui_state_modify(ui_highButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
    }
}
void ui_event_lowButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_state_modify(ui_highButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
        _ui_state_modify(ui_muteButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
    }
}
void ui_event_highButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_state_modify(ui_lowButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
        _ui_state_modify(ui_muteButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
    }
}
void ui_event_SysInfoBut(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_Screen3, LV_SCR_LOAD_ANIM_NONE, 500, 10);
    }
}
void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_NONE, 500, 10);
    }
}
void ui_event_backButton(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_NONE, 500, 10);
    }
}
void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_NONE, 500, 10);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
