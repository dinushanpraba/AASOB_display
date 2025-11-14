#include <CallBacks.h>

void handleJsonReceived(const JsonDocument &doc)
{
  Serial.println("Received JSON:");
  serializeJsonPretty(doc, Serial);

  if (doc.containsKey("Nosticker"))
  {
    // doc["Nosticker"];
    Serial.println("No Stikcer Present");
    serializeJsonPretty(doc["Nosticker"], Serial);
  }

  if (doc.containsKey("CountUp"))
  {
    Serial.println("Count Received");
    TotalCount++;
    DailyCount++;

    lv_label_set_text(objects.current_count_lbl, String(TotalCount).c_str());
    lv_label_set_text(objects.daily_count_lbl, String(DailyCount).c_str());

    const char *batchcount = lv_textarea_get_text(objects.batch_count_txtarea);
    if (TotalCount == atoi(batchcount))
    {
      lv_event_t *e;
      delay(12000);
      action_stop_btn_fn(e);
      lv_obj_clear_flag(objects.main_batch_count_finished_panel, LV_OBJ_FLAG_HIDDEN);
    }

    DynamicJsonDocument &countDoc = countSave.getDoc();
    countDoc["DailyCount"] = DailyCount;
    countSave.save();

    // int count = doc["CountUp"];
  }

  if (doc.containsKey("INPUT1"))
  {
    if (doc["INPUT1"])
    {
      lv_led_set_color(objects.motor1_sen, lv_palette_main(LV_PALETTE_GREEN));
    }
    else
    {
      lv_led_set_color(objects.motor1_sen, lv_palette_main(LV_PALETTE_RED));
    }
  }

  if (doc.containsKey("INPUT2"))
  {
    if (doc["INPUT2"])
    {
      lv_led_set_color(objects.motor2_sen, lv_palette_main(LV_PALETTE_GREEN));
    }
    else
    {
      lv_led_set_color(objects.motor2_sen, lv_palette_main(LV_PALETTE_RED));
    }
  }

  if (doc.containsKey("INPUT3"))
  {
    if (doc["INPUT3"])
    {
      lv_led_set_color(objects.motor3_sen, lv_palette_main(LV_PALETTE_GREEN));
    }
    else
    {
      lv_led_set_color(objects.motor3_sen, lv_palette_main(LV_PALETTE_RED));
    }
  }

  if (doc.containsKey("INPUT4"))
  {
    if (doc["INPUT4"])
    {
      lv_led_set_color(objects.conveyor_sen1, lv_palette_main(LV_PALETTE_GREEN));
    }
    else
    {
      lv_led_set_color(objects.conveyor_sen1, lv_palette_main(LV_PALETTE_RED));
    }
  }

  if (doc.containsKey("INPUT5"))
  {
    if (doc["INPUT5"])
    {
      lv_led_set_color(objects.conveyor_sen2, lv_palette_main(LV_PALETTE_GREEN));
    }
    else
    {
      lv_led_set_color(objects.conveyor_sen2, lv_palette_main(LV_PALETTE_RED));
    }
  }

  if (doc.containsKey("NoSticker"))
  {
    lv_event_t *e;
    action_stop_btn_fn(e);
    lv_obj_clear_flag(objects.main_no_sticker_lbl, LV_OBJ_FLAG_HIDDEN);
  }

  if (doc.containsKey("NoBox"))
  {
    lv_obj_clear_flag(objects.main_no_box_warning, LV_OBJ_FLAG_HIDDEN);
  }
}
