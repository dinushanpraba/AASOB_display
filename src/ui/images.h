#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_cartonbox;
extern const lv_img_dsc_t img_sdsd;
extern const lv_img_dsc_t img_se;
extern const lv_img_dsc_t img_cartonopen;
extern const lv_img_dsc_t img_fba;
extern const lv_img_dsc_t img_robo;
extern const lv_img_dsc_t img_settingsgif;
extern const lv_img_dsc_t img_settingsblack;
extern const lv_img_dsc_t img_sampath;
extern const lv_img_dsc_t img_anselllanka;
extern const lv_img_dsc_t img_home;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[11];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/