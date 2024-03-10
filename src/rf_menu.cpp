#include "rf_menu.h"
#include "ui.h"
#include "global_flags.h"
#include "pin_config.h"
#include <vector>
#include <FS.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <SD_MMC.h>

static lv_obj_t * chart;
static const lv_coord_t ecg_sample[] = {
0000,
};
/*====================================================
 * VARIAVEIS TELA
 ====================================================*/
static lv_obj_t *container_obj= nullptr;
static lv_obj_t *parent_obj= nullptr;

static void rx_config(lv_event_t *e);
void lv_Receiver();
static void event_receiver(lv_event_t * e);

static void tx_config(lv_event_t *e);
void lv_Transmitter();
static void event_transmitter(lv_event_t * e);

static void jamming_config(lv_event_t *e);
void lv_jamming();
static void event_jamming(lv_event_t * e);

static void event_handler(lv_event_t *e)
{
        if (e == nullptr) {
            return;
        }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Option: %s", buf);
    }
}


void lv_Rf_tabview(lv_obj_t *parent)
{

     parent_obj = parent;

    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_t *list1 = lv_list_create(container);
    lv_obj_set_size(list1, 67, 140);
    lv_obj_set_align(list1, LV_ALIGN_TOP_LEFT);

    lv_obj_t *btn = lv_list_add_btn(list1,NULL,"RX");
    lv_obj_add_event_cb(btn, rx_config, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *btn2 = lv_list_add_btn(list1,NULL, "TX");
    lv_obj_add_event_cb(btn2, tx_config, LV_EVENT_CLICKED, NULL);
    

    lv_obj_t * btn3= lv_list_add_btn(list1,NULL,"JAMMER");
    lv_obj_add_event_cb(btn3, jamming_config, LV_EVENT_CLICKED, NULL);
   

}

static void rx_config(lv_event_t *e)
{


   lv_Receiver();
   
}

static void event_receiver(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
    }
}

void lv_Receiver()
{

    container_obj=lv_obj_create(parent_obj);
    lv_obj_set_size(container_obj,235,140);
    lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 40, 15); 
    lv_obj_t * label1 = lv_label_create(container_obj);


    
    /*A style to make the selected option larger*/
    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_12);

    lv_obj_add_style(label1, &style_sel, LV_PART_SELECTED);
    lv_label_set_text(label1,"Frequency In Mhz:");
    lv_obj_set_pos(label1,0, -10);
    
    lv_obj_t * float_btn = lv_btn_create(container_obj);
    lv_obj_set_size(float_btn, 5, 5);
    lv_obj_set_pos(float_btn, 92, 45);
    lv_obj_set_style_radius(float_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_state(float_btn, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(float_btn, lv_color_white(), LV_STATE_DISABLED);
    
    lv_obj_t *roller;
    /*A roller on the left with left aligned text, and custom width*/
    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller, "0\n1\n2\n3\n4\n5\n6\n7\n8", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 2);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller,LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, -10, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 1, LV_ANIM_OFF);

    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 3);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 23, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 4);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 55, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);



    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 5);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 98, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);



    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 6);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 130, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    lv_obj_t *btn = lv_btn_create(container_obj);
    lv_obj_set_size(btn, 35, 25);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label,LV_SYMBOL_PLAY);
    lv_obj_set_pos(btn,170, 0);

    lv_obj_t *btn2 = lv_btn_create(container_obj);
    lv_obj_set_size(btn2, 35, 25);
    lv_obj_set_style_bg_color(btn2, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn2, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_obj_center(label2);
    lv_label_set_text(label2,LV_SYMBOL_STOP);
    lv_obj_set_pos(btn2,170, 30);


    chart = lv_chart_create(container_obj);
    lv_obj_set_size(chart, 215, 60);
    lv_obj_set_pos(chart, -10,60);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0000, 8580);

    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);

    lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    uint32_t pcnt = sizeof(ecg_sample) / sizeof(ecg_sample[0]);
    lv_chart_set_point_count(chart, pcnt);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t *)ecg_sample);

   

   
}


static void tx_config(lv_event_t *e)
{


   lv_Transmitter();
   
}

static void event_transmitter(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
    }
}

void lv_Transmitter()
{

    container_obj=lv_obj_create(parent_obj);
    lv_obj_set_size(container_obj,235,140);
    lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 40, 15); 
    lv_obj_t * label1 = lv_label_create(container_obj);


    
    /*A style to make the selected option larger*/
    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_12);

    lv_obj_add_style(label1, &style_sel, LV_PART_SELECTED);
    lv_label_set_text(label1,"Frequency In Mhz:");
    lv_obj_set_pos(label1,0, -10);
    
    lv_obj_t * float_btn = lv_btn_create(container_obj);
    lv_obj_set_size(float_btn, 5, 5);
    lv_obj_set_pos(float_btn, 92, 45);
    lv_obj_set_style_radius(float_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_state(float_btn, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(float_btn, lv_color_white(), LV_STATE_DISABLED);
    
    lv_obj_t *roller;
    /*A roller on the left with left aligned text, and custom width*/
    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller, "0\n1\n2\n3\n4\n5\n6\n7\n8", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 2);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller,LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, -10, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 1, LV_ANIM_OFF);

    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 3);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 23, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 4);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 55, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);



    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 5);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 98, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);



    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 6);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 130, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    lv_obj_t *btn = lv_btn_create(container_obj);
    lv_obj_set_size(btn, 35, 25);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label,LV_SYMBOL_PLAY);
    lv_obj_set_pos(btn,170, 0);

    lv_obj_t *btn2 = lv_btn_create(container_obj);
    lv_obj_set_size(btn2, 35, 25);
    lv_obj_set_style_bg_color(btn2, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn2, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_obj_center(label2);
    lv_label_set_text(label2,LV_SYMBOL_STOP);
    lv_obj_set_pos(btn2,170, 30);


    chart = lv_chart_create(container_obj);
    lv_obj_set_size(chart, 215, 60);
    lv_obj_set_pos(chart, -10,60);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0000, 8580);

    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);

    lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    uint32_t pcnt = sizeof(ecg_sample) / sizeof(ecg_sample[0]);
    lv_chart_set_point_count(chart, pcnt);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t *)ecg_sample);

   

   
}


static void jamming_config(lv_event_t *e)
{


   lv_jamming();
   
}

static void event_jamming(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
    }
}

void lv_jamming()
{

    container_obj=lv_obj_create(parent_obj);
    lv_obj_set_size(container_obj,235,140);
    lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 40, 15); 
    lv_obj_t * label1 = lv_label_create(container_obj);


    
    /*A style to make the selected option larger*/
    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_12);

    lv_obj_add_style(label1, &style_sel, LV_PART_SELECTED);
    lv_label_set_text(label1,"Frequency In Mhz:");
    lv_obj_set_pos(label1,0, -10);
    
    lv_obj_t * float_btn = lv_btn_create(container_obj);
    lv_obj_set_size(float_btn, 5, 5);
    lv_obj_set_pos(float_btn, 92, 45);
    lv_obj_set_style_radius(float_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_state(float_btn, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(float_btn, lv_color_white(), LV_STATE_DISABLED);
    
    lv_obj_t *roller;
    /*A roller on the left with left aligned text, and custom width*/
    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller, "0\n1\n2\n3\n4\n5\n6\n7\n8", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 2);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller,LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, -10, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 1, LV_ANIM_OFF);

    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 3);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 23, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 4);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 55, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);



    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 5);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 98, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);



    roller = lv_roller_create(container_obj);
    lv_roller_set_options(roller,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller, 6);
    lv_obj_set_width(roller, 35);
    lv_obj_set_height(roller,45);
    lv_obj_add_style(roller, &style_sel, LV_PART_SELECTED);
    lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_pos(roller, 130, 10);
    lv_obj_add_event_cb(roller, event_receiver, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller, 0, LV_ANIM_OFF);

    lv_obj_t *btn = lv_btn_create(container_obj);
    lv_obj_set_size(btn, 35, 25);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label,LV_SYMBOL_PLAY);
    lv_obj_set_pos(btn,170, 0);

    lv_obj_t *btn2 = lv_btn_create(container_obj);
    lv_obj_set_size(btn2, 35, 25);
    lv_obj_set_style_bg_color(btn2, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn2, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_obj_center(label2);
    lv_label_set_text(label2,LV_SYMBOL_STOP);
    lv_obj_set_pos(btn2,170, 30);


    chart = lv_chart_create(container_obj);
    lv_obj_set_size(chart, 215, 60);
    lv_obj_set_pos(chart, -10,60);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0000, 8580);

    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);

    lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    uint32_t pcnt = sizeof(ecg_sample) / sizeof(ecg_sample[0]);
    lv_chart_set_point_count(chart, pcnt);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t *)ecg_sample);

   

   
}

void runRfWindow(lv_obj_t *parent)
{
  lv_Rf_tabview(parent);

}



app_t rf_ui = {
    .setup_func_cb = runRfWindow,
    .exit_func_cb = nullptr,
    .user_data = nullptr
};
