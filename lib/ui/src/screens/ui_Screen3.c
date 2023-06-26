// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: test

#include "../ui.h"

void ui_Screen3_screen_init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_sc3MainPanel = lv_obj_create(ui_Screen3);
    lv_obj_set_width(ui_sc3MainPanel, 440);
    lv_obj_set_height(ui_sc3MainPanel, 280);
    lv_obj_set_x(ui_sc3MainPanel, 20);
    lv_obj_set_y(ui_sc3MainPanel, 20);
    lv_obj_clear_flag(ui_sc3MainPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_sc3MainPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_sc3MainPanel, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sysSpec = lv_label_create(ui_sc3MainPanel);
    lv_obj_set_width(ui_sysSpec, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_sysSpec, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_sysSpec, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_sysSpec,
                      "Devive name: WT32-SC01.\nSoC:	ESP32-WROVER-B module.\nCPU:	Dual-core Xtensa@ 32-bit LX6 MCU.\nConnectivity:	802.11 b/g/n 2.4GHz Wi-Fi.\nBluetooth: v4.2 BR/EDR and Bluetooth LE\nstandards.\nDisplay:	3.5 inch, LCD @320x480.\nCapacitive: with 2-point multitouch.");

    ui_backButton = lv_btn_create(ui_sc3MainPanel);
    lv_obj_set_width(ui_backButton, 100);
    lv_obj_set_height(ui_backButton, 50);
    lv_obj_set_x(ui_backButton, 151);
    lv_obj_set_y(ui_backButton, 180);
    lv_obj_add_flag(ui_backButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_backButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_backButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_backButton, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_backT = lv_label_create(ui_backButton);
    lv_obj_set_width(ui_backT, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_backT, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_backT, LV_ALIGN_CENTER);
    lv_label_set_text(ui_backT, "BACK");
    lv_obj_set_style_text_color(ui_backT, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_backT, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_backButton, ui_event_backButton, LV_EVENT_ALL, NULL);

}