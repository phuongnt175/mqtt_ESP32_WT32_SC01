// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: wt32sc01

#ifndef _WT32SC01_UI_H
#define _WT32SC01_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_lockScreen
void ui_lockScreen_screen_init(void);
void ui_event_lockScreen(lv_event_t * e);
extern lv_obj_t * ui_lockScreen;
extern lv_obj_t * ui_Label2;
// SCREEN: ui_MainScreen
void ui_MainScreen_screen_init(void);
void ui_event_MainScreen(lv_event_t * e);
extern lv_obj_t * ui_MainScreen;
extern lv_obj_t * ui_lightBtnOne;
extern lv_obj_t * ui_Image5;
extern lv_obj_t * ui_lightBtnTwo;
extern lv_obj_t * ui_Image2;
extern lv_obj_t * ui_curtainBtn;
extern lv_obj_t * ui_Image3;
extern lv_obj_t * ui_acBtn;
extern lv_obj_t * ui_Image4;
// SCREEN: ui_secondScreen
void ui_secondScreen_screen_init(void);
void ui_event_secondScreen(lv_event_t * e);
extern lv_obj_t * ui_secondScreen;
extern lv_obj_t * ui_function1;
extern lv_obj_t * ui_Label3;
extern lv_obj_t * ui_function2;
extern lv_obj_t * ui_Label1;
extern lv_obj_t * ui_function3;
extern lv_obj_t * ui_Label5;
extern lv_obj_t * ui_function4;
extern lv_obj_t * ui_Label6;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_bulb_png);    // assets\bulb.png
LV_IMG_DECLARE(ui_img_curtain_png);    // assets\curtain.png
LV_IMG_DECLARE(ui_img_2047347433);    // assets\air-conditioner.png
LV_IMG_DECLARE(ui_img_30409_treecanopy_v1_920485_png);    // assets\30409_treecanopy_v1_920485.png
LV_IMG_DECLARE(ui_img_1862871016);    // assets\wallpapersden.com_beautiful-scenery-mountains-lake_480x320.png

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
