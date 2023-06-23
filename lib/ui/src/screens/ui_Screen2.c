// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: test

#include "../ui.h"

void ui_Screen2_screen_init(void)
{
    ui_Screen2 = lv_obj_create(NULL);
    lv_obj_add_flag(ui_Screen2, LV_OBJ_FLAG_SCROLL_ONE);     /// Flags
    lv_obj_set_scrollbar_mode(ui_Screen2, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(ui_Screen2, LV_DIR_VER);

    ui_sc2BrightnessPanel = lv_obj_create(ui_Screen2);
    lv_obj_set_width(ui_sc2BrightnessPanel, 450);
    lv_obj_set_height(ui_sc2BrightnessPanel, 80);
    lv_obj_set_x(ui_sc2BrightnessPanel, 0);
    lv_obj_set_y(ui_sc2BrightnessPanel, 30);
    lv_obj_set_align(ui_sc2BrightnessPanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_sc2BrightnessPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_sc2BrightnessPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sc2BrightnessPanel, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BrightnessSlider = lv_slider_create(ui_sc2BrightnessPanel);
    lv_slider_set_range(ui_BrightnessSlider, 10, 100);
    lv_slider_set_value(ui_BrightnessSlider, 50, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_BrightnessSlider) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_BrightnessSlider, 0,
                                                                                                     LV_ANIM_OFF);
    lv_obj_set_width(ui_BrightnessSlider, 380);
    lv_obj_set_height(ui_BrightnessSlider, 20);
    lv_obj_set_x(ui_BrightnessSlider, 9);
    lv_obj_set_y(ui_BrightnessSlider, 15);
    lv_obj_set_align(ui_BrightnessSlider, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_BrightnessSlider, lv_color_hex(0xAE7070), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BrightnessSlider, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_BrightnessSlider, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BrightnessSlider, 150, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_BrightnessSlider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BrightnessSlider, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_BrightnessTi = lv_label_create(ui_sc2BrightnessPanel);
    lv_obj_set_width(ui_BrightnessTi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BrightnessTi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_BrightnessTi, -154);
    lv_obj_set_y(ui_BrightnessTi, -21);
    lv_obj_set_align(ui_BrightnessTi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BrightnessTi, "Brightness :");
    lv_obj_set_style_text_align(ui_BrightnessTi, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label6 = lv_label_create(ui_sc2BrightnessPanel);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label6, -93);
    lv_obj_set_y(ui_Label6, -21);
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, "50");

    ui_sc2VolPanel = lv_obj_create(ui_Screen2);
    lv_obj_set_width(ui_sc2VolPanel, 450);
    lv_obj_set_height(ui_sc2VolPanel, 90);
    lv_obj_set_x(ui_sc2VolPanel, 0);
    lv_obj_set_y(ui_sc2VolPanel, 10);
    lv_obj_set_align(ui_sc2VolPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_sc2VolPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_sc2VolPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sc2VolPanel, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_muteButton = lv_btn_create(ui_sc2VolPanel);
    lv_obj_set_width(ui_muteButton, 100);
    lv_obj_set_height(ui_muteButton, 50);
    lv_obj_set_x(ui_muteButton, -150);
    lv_obj_set_y(ui_muteButton, 9);
    lv_obj_set_align(ui_muteButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_muteButton, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_muteButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_muteButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_muteButton, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_muteT = lv_label_create(ui_muteButton);
    lv_obj_set_width(ui_muteT, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_muteT, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_muteT, LV_ALIGN_CENTER);
    lv_label_set_text(ui_muteT, "MUTE");
    lv_obj_set_style_text_color(ui_muteT, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_muteT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lowButton = lv_btn_create(ui_sc2VolPanel);
    lv_obj_set_width(ui_lowButton, 100);
    lv_obj_set_height(ui_lowButton, 50);
    lv_obj_set_x(ui_lowButton, 0);
    lv_obj_set_y(ui_lowButton, 10);
    lv_obj_set_align(ui_lowButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_lowButton, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_lowButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_lowButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_lowButton, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lowT = lv_label_create(ui_lowButton);
    lv_obj_set_width(ui_lowT, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lowT, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lowT, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lowT, "LOW");
    lv_obj_set_style_text_color(ui_lowT, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lowT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_highButton = lv_btn_create(ui_sc2VolPanel);
    lv_obj_set_width(ui_highButton, 100);
    lv_obj_set_height(ui_highButton, 50);
    lv_obj_set_x(ui_highButton, 150);
    lv_obj_set_y(ui_highButton, 10);
    lv_obj_set_align(ui_highButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_highButton, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_highButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_highButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_highButton, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_hightT = lv_label_create(ui_highButton);
    lv_obj_set_width(ui_hightT, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_hightT, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_hightT, LV_ALIGN_CENTER);
    lv_label_set_text(ui_hightT, "HIGH");
    lv_obj_set_style_text_color(ui_hightT, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_hightT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_volTi = lv_label_create(ui_sc2VolPanel);
    lv_obj_set_width(ui_volTi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_volTi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_volTi, -159);
    lv_obj_set_y(ui_volTi, -32);
    lv_obj_set_align(ui_volTi, LV_ALIGN_CENTER);
    lv_label_set_text(ui_volTi, "Volume");

    ui_sc2SystemPanel = lv_obj_create(ui_Screen2);
    lv_obj_set_width(ui_sc2SystemPanel, 450);
    lv_obj_set_height(ui_sc2SystemPanel, 70);
    lv_obj_set_x(ui_sc2SystemPanel, 0);
    lv_obj_set_y(ui_sc2SystemPanel, 105);
    lv_obj_set_align(ui_sc2SystemPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_sc2SystemPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_sc2SystemPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sc2SystemPanel, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SysInfoBut = lv_btn_create(ui_sc2SystemPanel);
    lv_obj_set_width(ui_SysInfoBut, 200);
    lv_obj_set_height(ui_SysInfoBut, 50);
    lv_obj_set_x(ui_SysInfoBut, -100);
    lv_obj_set_y(ui_SysInfoBut, 0);
    lv_obj_set_align(ui_SysInfoBut, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SysInfoBut, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SysInfoBut, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_SysInfoBut, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SysInfoBut, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sysInfoT = lv_label_create(ui_SysInfoBut);
    lv_obj_set_width(ui_sysInfoT, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_sysInfoT, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_sysInfoT, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_sysInfoT, "SYSTEM INFORMATION");
    lv_obj_set_style_text_color(ui_sysInfoT, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_sysInfoT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button2 = lv_btn_create(ui_sc2SystemPanel);
    lv_obj_set_width(ui_Button2, 180);
    lv_obj_set_height(ui_Button2, 50);
    lv_obj_set_align(ui_Button2, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label7 = lv_label_create(ui_Button2);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "CHANGE THEME COLOR");
    lv_obj_set_style_text_color(ui_Label7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_BrightnessSlider, ui_event_BrightnessSlider, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_muteButton, ui_event_muteButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_lowButton, ui_event_lowButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_highButton, ui_event_highButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SysInfoBut, ui_event_SysInfoBut, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Screen2, ui_event_Screen2, LV_EVENT_ALL, NULL);

}
