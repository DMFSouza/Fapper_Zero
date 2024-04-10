#include "ui.h"
#include "Fapper_zero.h"

lv_obj_t *ibtn_start;
lv_obj_t *ibtn_stop;
void runBleWindow(lv_obj_t *parent);
static void stop_ibeacom(lv_event_t *e);
static void start_ibeacom(lv_event_t *e);

void runBleWindow(lv_obj_t *parent)
{
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container,260,140);
  lv_obj_align(container,  LV_ALIGN_TOP_MID, 0, 15);
  

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_palette_darken(LV_PALETTE_GREEN, 3));

  static lv_style_t style_btn1;
  lv_style_init(&style_btn1);
  lv_style_set_bg_color(&style_btn1, lv_palette_darken(LV_PALETTE_RED, 3));

    
  ibtn_start = lv_btn_create(container);
  lv_obj_add_style(ibtn_start ,&style_btn,0);
  
  lv_obj_set_size(ibtn_start,110,30);
  lv_obj_add_event_cb(ibtn_start,  start_ibeacom, LV_EVENT_CLICKED, NULL);
  lv_obj_align(ibtn_start,  LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *ilabel_start = lv_label_create(ibtn_start);
  lv_label_set_text(ilabel_start, "IBeacon Start");
  lv_obj_center(ilabel_start);

  ibtn_stop = lv_btn_create(container);
  lv_obj_add_style(ibtn_stop ,&style_btn1,0);
  
  lv_obj_set_size(ibtn_stop,110,30);
  lv_obj_add_flag(ibtn_stop, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(ibtn_stop, stop_ibeacom, LV_EVENT_CLICKED, NULL);
  lv_obj_align(ibtn_stop,  LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *ilabel_stop = lv_label_create(ibtn_stop);
  lv_label_set_text(ilabel_stop, "IBeacon Stop");
  lv_obj_center(ilabel_stop);
}


static void start_ibeacom(lv_event_t *e){
  lv_obj_add_flag(ibtn_start, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ibtn_stop, LV_OBJ_FLAG_HIDDEN);
  ibeacom_start() ;
}
static void stop_ibeacom(lv_event_t *e){
   lv_obj_add_flag(ibtn_stop, LV_OBJ_FLAG_HIDDEN);
   lv_obj_clear_flag(ibtn_start, LV_OBJ_FLAG_HIDDEN);
   ibeacom_stop() ;
}
    


app_t ble_ui = {
    .setup_func_cb = runBleWindow,
    .exit_func_cb = nullptr,
    .user_data = nullptr
};
