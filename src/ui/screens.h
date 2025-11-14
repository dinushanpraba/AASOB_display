#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *loadingpage;
    lv_obj_t *settings_page;
    lv_obj_t *io_test_page;
    lv_obj_t *advanced_page;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *no_of_stikcer_rolls_lbl;
    lv_obj_t *obj3;
    lv_obj_t *current_count_lbl;
    lv_obj_t *obj4;
    lv_obj_t *startbtn;
    lv_obj_t *stopbtn;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *batch_count_txtarea;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *daily_count_lbl;
    lv_obj_t *obj9;
    lv_obj_t *main_warning_panel;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *main_no_sticker_lbl;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *main_batch_count_finished_panel;
    lv_obj_t *obj14;
    lv_obj_t *obj15;
    lv_obj_t *main_no_box_warning;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *main_num_keyboard;
    lv_obj_t *obj18;
    lv_obj_t *obj19;
    lv_obj_t *sticker_one_switch;
    lv_obj_t *sticker_01_length;
    lv_obj_t *obj20;
    lv_obj_t *obj21;
    lv_obj_t *sticker_two_switch;
    lv_obj_t *sticker_02_length;
    lv_obj_t *obj22;
    lv_obj_t *obj23;
    lv_obj_t *sticker_three_switch;
    lv_obj_t *sticker_03_length;
    lv_obj_t *obj24;
    lv_obj_t *obj25;
    lv_obj_t *obj26;
    lv_obj_t *obj27;
    lv_obj_t *obj28;
    lv_obj_t *login_panel;
    lv_obj_t *password_txtarea;
    lv_obj_t *notify_panel;
    lv_obj_t *obj29;
    lv_obj_t *settings_keyboard;
    lv_obj_t *obj30;
    lv_obj_t *obj31;
    lv_obj_t *obj32;
    lv_obj_t *obj33;
    lv_obj_t *obj34;
    lv_obj_t *obj35;
    lv_obj_t *obj36;
    lv_obj_t *motor1_sen;
    lv_obj_t *motor2_sen;
    lv_obj_t *motor3_sen;
    lv_obj_t *conveyor_sen1;
    lv_obj_t *conveyor_sen2;
    lv_obj_t *obj37;
    lv_obj_t *obj38;
    lv_obj_t *stepper1_speed;
    lv_obj_t *stepper2_speed;
    lv_obj_t *stepper3_speed;
    lv_obj_t *obj39;
    lv_obj_t *speed_setting_keyboard;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_LOADINGPAGE = 2,
    SCREEN_ID_SETTINGS_PAGE = 3,
    SCREEN_ID_IO_TEST_PAGE = 4,
    SCREEN_ID_ADVANCED_PAGE = 5,
};

void create_screen_main();
void tick_screen_main();

void create_screen_loadingpage();
void tick_screen_loadingpage();

void create_screen_settings_page();
void tick_screen_settings_page();

void create_screen_io_test_page();
void tick_screen_io_test_page();

void create_screen_advanced_page();
void tick_screen_advanced_page();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/