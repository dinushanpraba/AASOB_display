#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_start_btn_fn(lv_event_t * e);
extern void action_stop_btn_fn(lv_event_t * e);
extern void action_settings_btn_fn(lv_event_t * e);
extern void action_open_num_keypad(lv_event_t * e);
extern void action_close_num_keypad(lv_event_t * e);
extern void action_goto_iotest_page(lv_event_t * e);
extern void action_motor1_feed(lv_event_t * e);
extern void action_motor2_feed(lv_event_t * e);
extern void action_motor3_feed(lv_event_t * e);
extern void action_conveyor_feed(lv_event_t * e);
extern void action_goto_home_page(lv_event_t * e);
extern void action_save_all_data(lv_event_t * e);
extern void action_sticker1_feed(lv_event_t * e);
extern void action_sticker2_feed(lv_event_t * e);
extern void action_sticker3_feed(lv_event_t * e);
extern void action_save_speed_settings(lv_event_t * e);
extern void action_unlock_btn_fn(lv_event_t * e);
extern void action_set_speed_fn(lv_event_t * e);
extern void action_login_defocused(lv_event_t * e);
extern void action_save_completed_btn(lv_event_t * e);
extern void action_kb_event_cb(lv_event_t * e);
extern void action_cancel_main_warning(lv_event_t * e);
extern void action_reset_daily_count_lbl(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/