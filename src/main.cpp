#include <Arduino.h>
#include <lvgl.h>
int Mode = 0;
#include "lovyanGfxSetup.h"
#include "ui/ui.h"
#include "ui/actions.h"
#include "ui/images.h"
#include <WiFi.h>
#include <time.h>

#include <Adafruit_NeoPixel.h>
// #include <esp_io_expander.hpp>

#include <Callbacks.h>
#include <CanJsonComm.h>
#include <config_handle.h>

void LoadSlectedSteppers();
#define CANTX 15
#define CANRX 16
#define PIXEL_LED 6 // PIXEL LED data pin
int TotalCount = 0;
int DailyCount = 0;
int BatchCount = 0;

Adafruit_NeoPixel strip(5, PIXEL_LED, NEO_GRB + NEO_KHZ800);
CanJsonComm canNode(0x200, 0x100, (gpio_num_t)CANTX, (gpio_num_t)CANRX);
ConfigManager configManager("/configManager.json");

ConfigManager countSave("/countSave.json");

#define EXAMPLE_I2C_SDA_PIN (8)
#define EXAMPLE_I2C_SCL_PIN (9)
#define EXAMPLE_I2C_ADDR (ESP_IO_EXPANDER_I2C_CH422G_ADDRESS)

// esp_expander::CH422G *expander = NULL;

#define TFT_HOR_RES SCREEN_WIDTH
#define TFT_VER_RES SCREEN_HEIGHT
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

LGFX tft;
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

static bool motor1State = false; // persists between calls
static bool motor2State = false; // persists between calls
static bool motor3State = false; // persists between calls

static bool MainStartBtn = false;
extern "C" void action_start_btn_fn(lv_event_t *e)
{

  const char *batchcount = lv_textarea_get_text(objects.batch_count_txtarea);

  if (atoi(batchcount))
  {
    Serial.print("Batch Count Set To: ");
    Serial.println(batchcount);

    DynamicJsonDocument &selectedStyle = configManager.getDoc();
    canNode.sendJson(selectedStyle);
    delay(1000);
    StaticJsonDocument<64> docSend;
    docSend["RUN"] = "START";
    canNode.sendJson(docSend);

    lv_obj_set_style_bg_color(objects.startbtn, lv_color_hex(0xff12cb3e), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  else
  {
    Serial.println("Batch Count Not Set, Defaulting to 0");
    lv_obj_clear_flag(objects.main_warning_panel, LV_OBJ_FLAG_HIDDEN);
  }
}
extern "C" void action_stop_btn_fn(lv_event_t *e)
{
  TotalCount = 0;
  lv_label_set_text(objects.current_count_lbl, String(TotalCount).c_str());
  lv_obj_set_style_bg_color(objects.startbtn, lv_color_hex(0xff947206), LV_PART_MAIN | LV_STATE_DEFAULT);
  StaticJsonDocument<64> docSend;
  docSend["RUN"] = "STOP";
  canNode.sendJson(docSend);
  delay(1000);
}
extern "C" void action_settings_btn_fn(lv_event_t *e)
{
  loadScreen(SCREEN_ID_SETTINGS_PAGE);
  // DynamicJsonDocument &selectedStyle = configManager.getDoc();
  // serializeJsonPretty(selectedStyle, Serial);

  // char sticker1[10];
  // char sticker2[10];
  // char sticker3[10];

  // // Serial.println(String(styleData["BothBeltSyncRatio"]).c_str());

  // snprintf(sticker1, sizeof(sticker1), "%d", selectedStyle["sticker1"] | 0);
  // snprintf(sticker2, sizeof(sticker2), "%d", selectedStyle["sticker2"] | 0);
  // snprintf(sticker3, sizeof(sticker3), "%d", selectedStyle["sticker3"] | 0);

  // lv_textarea_set_text(objects.sticker_01_length, sticker1);
  // lv_textarea_set_text(objects.sticker_02_length, sticker2);
  // lv_textarea_set_text(objects.sticker_03_length, sticker3);

  DynamicJsonDocument &selectedStyle = configManager.getDoc();

  // Load sticker1
  if (selectedStyle.containsKey("sticker1"))
  {
    bool sticker1Enabled = selectedStyle["sticker1"]["enabled"] | false;
    if (sticker1Enabled)
      lv_obj_add_state(objects.sticker_one_switch, LV_STATE_CHECKED); // turn ON
    else
      lv_obj_clear_state(objects.sticker_one_switch, LV_STATE_CHECKED); // turn OFF

    // also restore length if you want
    float sticker1Length = selectedStyle["sticker1"]["length"];
    sticker1Length = sticker1Length / 60;
    lv_textarea_set_text(objects.sticker_01_length, String(sticker1Length).c_str());
  }

  // Load sticker2
  if (selectedStyle.containsKey("sticker2"))
  {
    bool sticker2Enabled = selectedStyle["sticker2"]["enabled"] | false;
    if (sticker2Enabled)
      lv_obj_add_state(objects.sticker_two_switch, LV_STATE_CHECKED);
    else
      lv_obj_clear_state(objects.sticker_two_switch, LV_STATE_CHECKED);

    float sticker2Length = selectedStyle["sticker2"]["length"];
    sticker2Length = sticker2Length / 60;
    lv_textarea_set_text(objects.sticker_02_length, String(sticker2Length, 2).c_str());
  }

  // Load sticker3
  if (selectedStyle.containsKey("sticker3"))
  {
    bool sticker3Enabled = selectedStyle["sticker3"]["enabled"] | false;
    if (sticker3Enabled)
      lv_obj_add_state(objects.sticker_three_switch, LV_STATE_CHECKED);
    else
      lv_obj_clear_state(objects.sticker_three_switch, LV_STATE_CHECKED);

    float sticker3Length = selectedStyle["sticker3"]["length"];
    sticker3Length = sticker3Length / 60;
    lv_textarea_set_text(objects.sticker_03_length, String(sticker3Length).c_str());
  }
}

extern "C" void action_open_num_keypad(lv_event_t *e)
{
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
  // lv_keyboard_set_textarea(objects.keyboard_num, ta);

  lv_obj_clear_flag(objects.settings_keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.speed_setting_keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(objects.main_num_keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_keyboard_set_textarea(objects.settings_keyboard, ta);
  lv_keyboard_set_textarea(objects.speed_setting_keyboard, ta);
  lv_keyboard_set_textarea(objects.main_num_keyboard, ta);
}
extern "C" void action_close_num_keypad(lv_event_t *e)
{
  lv_obj_add_flag(objects.settings_keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.speed_setting_keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.main_num_keyboard, LV_OBJ_FLAG_HIDDEN);
}
extern "C" void action_kb_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_target(e);

  if (code == LV_EVENT_READY)
  {
    Serial.println("Tick (✓) key pressed!");
    // You can also hide the keyboard or process the text

    const char *BatchCountchar = lv_textarea_get_text(objects.batch_count_txtarea);
    DynamicJsonDocument &countDoc = countSave.getDoc();
    countDoc["BatchCount"] = atoi(BatchCountchar);
    countSave.save();

    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
  else if (code == LV_EVENT_CANCEL)
  {
    Serial.println("Cancel (X) key pressed!");
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
}

extern "C" void action_cancel_main_warning(lv_event_t *e)
{
  lv_obj_add_flag(objects.main_warning_panel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.main_no_sticker_lbl, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.main_no_box_warning, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(objects.main_batch_count_finished_panel, LV_OBJ_FLAG_HIDDEN);
}
extern "C" void action_goto_iotest_page(lv_event_t *e)
{
  loadScreen(SCREEN_ID_IO_TEST_PAGE);
  StaticJsonDocument<64> docSend;
  docSend["TESTIO"] = "START";
  canNode.sendJson(docSend);
}
extern "C" void action_motor1_feed(lv_event_t *e)
{

  // Toggle state each button press
  motor1State = !motor1State;
  StaticJsonDocument<64> docSend;
  docSend["MOTOR1RUN"] = motor1State;
  canNode.sendJson(docSend);

  Serial.print("MOTOR1RUN: ");
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  Serial.println(motor1State ? "true" : "false");
  if (motor1State)
  {

    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff6bb939), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0xff), LV_PART_MAIN | LV_STATE_CHECKED);
  }
  else
  {
    // #2196f3
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
  }
}
extern "C" void action_motor2_feed(lv_event_t *e)
{
  motor2State = !motor2State;
  StaticJsonDocument<64> docSend;
  docSend["MOTOR2RUN"] = motor2State;
  canNode.sendJson(docSend);

  Serial.print("MOTOR1RUN: ");
  Serial.println(motor2State ? "true" : "false");
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  if (motor2State)
  {

    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff6bb939), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0xff), LV_PART_MAIN | LV_STATE_CHECKED);
  }
  else
  {
    // #2196f3
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
  }
}
extern "C" void action_motor3_feed(lv_event_t *e)
{
  motor3State = !motor3State;
  StaticJsonDocument<64> docSend;
  docSend["MOTOR3RUN"] = motor3State;
  canNode.sendJson(docSend);

  Serial.print("MOTOR3RUN: ");
  Serial.println(motor3State ? "true" : "false");
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  if (motor3State)
  {

    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff6bb939), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0xff), LV_PART_MAIN | LV_STATE_CHECKED);
  }
  else
  {
    // #2196f3
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
  }
}
extern "C" void action_conveyor_feed(lv_event_t *e)
{
}
extern "C" void action_goto_home_page(lv_event_t *e)
{
  loadScreen(SCREEN_ID_MAIN);
  LoadSlectedSteppers();
}

extern "C" void action_save_all_data(lv_event_t *e)
{

  // DynamicJsonDocument &selectedStyle = configManager.getDoc();
  // const char *sticker1 = lv_textarea_get_text(objects.sticker_01_length);
  // const char *sticker2 = lv_textarea_get_text(objects.sticker_02_length);
  // const char *sticker3 = lv_textarea_get_text(objects.sticker_03_length);

  // selectedStyle["sticker1"] = atoi(sticker1);
  // selectedStyle["sticker2"] = atof(sticker2);
  // selectedStyle["sticker3"] = atoi(sticker3);
  // serializeJsonPretty(selectedStyle, Serial);
  // configManager.save();

  DynamicJsonDocument &selectedStyle = configManager.getDoc();

  // Get switch state
  bool sticker1Enabled = lv_obj_has_state(objects.sticker_one_switch, LV_STATE_CHECKED);
  bool sticker2Enabled = lv_obj_has_state(objects.sticker_two_switch, LV_STATE_CHECKED);
  bool sticker3Enabled = lv_obj_has_state(objects.sticker_three_switch, LV_STATE_CHECKED);

  // Get textareas
  const char *sticker1 = lv_textarea_get_text(objects.sticker_01_length);
  const char *sticker2 = lv_textarea_get_text(objects.sticker_02_length);
  const char *sticker3 = lv_textarea_get_text(objects.sticker_03_length);

  // Create sticker JSON
  int Steps1 = atoi(sticker1) * 60;
  int Steps2 = atoi(sticker2) * 60;
  int Steps3 = atoi(sticker3) * 60;

  selectedStyle["sticker1"]["enabled"] = sticker1Enabled;
  selectedStyle["sticker1"]["length"] = Steps1;

  selectedStyle["sticker2"]["enabled"] = sticker2Enabled;
  selectedStyle["sticker2"]["length"] = Steps2;

  selectedStyle["sticker3"]["enabled"] = sticker3Enabled;
  selectedStyle["sticker3"]["length"] = Steps3;
  // JsonObject sticker1Obj = selectedStyle.createNestedObject("sticker1");
  // sticker1Obj["enabled"] = sticker1Enabled;
  // sticker1Obj["length"] = atoi(sticker1);

  // JsonObject sticker2Obj = selectedStyle.createNestedObject("sticker2");
  // sticker2Obj["enabled"] = sticker2Enabled;
  // sticker2Obj["length"] = atof(sticker2);

  // JsonObject sticker3Obj = selectedStyle.createNestedObject("sticker3");
  // sticker3Obj["enabled"] = sticker3Enabled;
  // sticker3Obj["length"] = atoi(sticker3);

  // Print and save
  serializeJsonPretty(selectedStyle, Serial);
  configManager.save();

  lv_obj_clear_flag(objects.notify_panel, LV_OBJ_FLAG_HIDDEN);
}

extern "C" void action_sticker1_feed(lv_event_t *e)
{
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  // lv_obj_set_style_bg_color(btn, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED);
  // lv_obj_set_style_bg_color(btn, lv_color_hex(0xff947206), LV_PART_MAIN | LV_STATE_DEFAULT);
  action_motor1_feed(e);
}
extern "C" void action_sticker2_feed(lv_event_t *e)
{
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  action_motor2_feed(e);
}
extern "C" void action_sticker3_feed(lv_event_t *e)
{
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  action_motor3_feed(e);
}

extern "C" void action_save_speed_settings(lv_event_t *e)
{
  const char *stepper1_speed = lv_textarea_get_text(objects.stepper1_speed);
  const char *stepper2_speed = lv_textarea_get_text(objects.stepper2_speed);
  const char *stepper3_speed = lv_textarea_get_text(objects.stepper3_speed);

  DynamicJsonDocument &selectedStyle = configManager.getDoc();
  selectedStyle["sticker1"]["speed"] = atoi(stepper1_speed);
  selectedStyle["sticker2"]["speed"] = atoi(stepper2_speed);
  selectedStyle["sticker3"]["speed"] = atoi(stepper3_speed);
  configManager.save();
  serializeJsonPretty(selectedStyle, Serial);
}
extern "C" void action_reset_daily_count_lbl(lv_event_t *e)
{
  TotalCount = 0;
  Serial.println("Reset Total Count Pressed..");

  lv_label_set_text(objects.current_count_lbl, String(TotalCount).c_str());
}
extern "C" void action_unlock_btn_fn(lv_event_t *e)
{

  DynamicJsonDocument &selectedStyle = configManager.getDoc();
  const char *password = lv_textarea_get_text(objects.password_txtarea);
  if (strcmp(password, "19911103") == 0)
  {
    loadScreen(SCREEN_ID_ADVANCED_PAGE);
    int speed1 = selectedStyle["sticker1"]["speed"] | 0;

    // Serial.println()
    lv_textarea_set_text(objects.stepper1_speed, String(speed1).c_str());

    int speed2 = selectedStyle["sticker2"]["speed"] | 0;
    lv_textarea_set_text(objects.stepper2_speed, String(speed2).c_str());

    int speed3 = selectedStyle["sticker3"]["speed"] | 0;
    lv_textarea_set_text(objects.stepper3_speed, String(speed3).c_str());
  }
  else
  {
    // lv_label_set_text(objects.label_status, "Incorrect Password");
    Serial.println("Incorrect Password");
  }
  lv_obj_add_flag(objects.login_panel, LV_OBJ_FLAG_HIDDEN);
}
extern "C" void action_set_speed_fn(lv_event_t *e)
{
  lv_obj_clear_flag(objects.login_panel, LV_OBJ_FLAG_HIDDEN);
}
extern "C" void action_login_defocused(lv_event_t *e)
{
  lv_obj_add_flag(objects.login_panel, LV_OBJ_FLAG_HIDDEN);
}

extern "C" void action_save_completed_btn(lv_event_t *e)
{
  lv_obj_add_flag(objects.notify_panel, LV_OBJ_FLAG_HIDDEN);
}

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  else
  {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = touchX;
    data->point.y = touchY;
#if 0
    Serial.printf("x: %03d, y: %03d\n", data->point.x, data->point.y);
#endif
  }
}

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)px_map, w * h);
  tft.endWrite();

  /* Call it to tell LVGL you are ready. */
  lv_disp_flush_ready(disp);
}
void setup()
{

  //// Vibartion with I2C exapander

  // expander = new esp_expander::CH422G(EXAMPLE_I2C_SCL_PIN, EXAMPLE_I2C_SDA_PIN, EXAMPLE_I2C_ADDR);
  // expander->init();
  // expander->begin();

  // Serial.println("Set the OC pin to push-pull output mode.");
  // expander->enableOC_PushPull();

  // // Serial.println("Set the OC pin to open_drain output mode.");
  // // expander->enableOC_OpenDrain();

  // //  Serial.println("Set the IO0-7 pin to input mode.");
  // //  expander->enableAllIO_Input();

  // Serial.println("Set the IO0-7 pin to output mode.");
  // expander->enableAllIO_Output();
  // expander->digitalWrite(5, LOW);

  //////////////////

  String LVGL_Arduino = "Hello Arduino ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.begin(9600);
  Serial.println(LVGL_Arduino);

  canNode.begin();
  Serial.println("CAN Node Initialized");
  canNode.onJsonReceived(handleJsonReceived);

  if (!configManager.begin())
  {
    Serial.println("Failed to initialize ConfigManager!");
  }

  if (!countSave.begin())
  {
    Serial.println("Failed to initialize countSave!");
  }
  else
  {
    DynamicJsonDocument &countDoc = countSave.getDoc();
    BatchCount = countDoc["BatchCount"] | 0;
    DailyCount = countDoc["DailyCount"] | 0;
    Serial.print("Restored BatchCount: ");
    Serial.println(BatchCount);
    Serial.print("Restored DailyCount: ");
    Serial.println(DailyCount);
  }

  tft.begin();
  tft.setRotation(0);
  tft.setBrightness(255);

  lv_init();
  lv_tick_set_cb((lv_tick_get_cb_t)millis);

  /* Register print function for debugging. */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  /* Create a display. */
  lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  /* Initialize the (dummy) input device driver. */
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, my_touchpad_read);

  Serial.println("Setup done");

#if 0
/* Create a simple label. */
lv_obj_t *label = lv_label_create( lv_scr_act() );
lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#endif
  // ui_init();

  create_screens();
  loadScreen(SCREEN_ID_MAIN);
  LoadSlectedSteppers();
  lv_task_handler();
}

void LoadSlectedSteppers()
{
  Serial.println("Loading Selected Steppers");
  DynamicJsonDocument &selectedStyle = configManager.getDoc();
  serializeJsonPretty(selectedStyle, Serial);
  // lv_label_set_text(objects.no_of_stikcer_rolls_lbl, );

  int trueCount = 0;

  for (JsonPair kv : selectedStyle.as<JsonObject>())
  {
    JsonObject sticker = kv.value().as<JsonObject>();
    if (sticker["enabled"] == true)
    {
      trueCount++;
    }
  }

  char buf[8];
  snprintf(buf, sizeof(buf), "%d", trueCount);

  // Set LVGL label text
  lv_label_set_text(objects.no_of_stikcer_rolls_lbl, buf);

  Serial.print("Number of enabled = ");
  Serial.println(trueCount);

  lv_textarea_set_text(objects.batch_count_txtarea, String(BatchCount).c_str());
  lv_label_set_text(objects.daily_count_lbl, String(DailyCount).c_str());
}

void loop()
{
  // expander->digitalWrite(4, HIGH);
  // delay(500);
  // expander->digitalWrite(4, LOW);
  canNode.loop();
  lv_task_handler(); /* Let LVGL do its work. */
  ui_tick();
  // put your main code here, to run repeatedly:
}
