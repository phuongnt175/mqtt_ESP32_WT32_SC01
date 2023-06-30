SET(SOURCES screens/ui_lockScreen.c
    screens/ui_MainScreen.c
    screens/ui_secondScreen.c
    ui.c
    ui_helpers.c
    images/ui_img_bulb_png.c
    images/ui_img_curtain_png.c
    images/ui_img_2047347433.c
    images/ui_img_30409_treecanopy_v1_920485_png.c
    images/ui_img_1862871016.c)

add_library(ui ${SOURCES})
