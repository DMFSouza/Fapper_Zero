#include "config.h"
#include "Fapper_Zero.h"
#include "ui.h"

static lv_obj_t *slider;
static lv_obj_t * slider_label;

static lv_obj_t *slider1;
static lv_obj_t * slider_label1;

static lv_obj_t *slider2;
static lv_obj_t * slider_label2;
static lv_obj_t * led ;

int encoder_value =0;
int encoder_value1 =0;
int encoder_value2 =0;

static void red_callback(lv_event_t * e);
static void green_callback(lv_event_t * e);
static void blue_callback(lv_event_t * e);
static void call_rgb(lv_event_t * e);
RGBcolor rgb;
static void red_callback(lv_event_t * e)
{
    
    int increment = 0;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    
    int current_value = lv_slider_get_value(slider);
    if (current_value >  encoder_value){
      current_value+=5;
    } else if (current_value <  encoder_value) {
      current_value-=5;
    }
 
    // Verifique se o novo valor está dentro do intervalo permitido pelo slider
    int max_value = lv_slider_get_max_value(slider);
    current_value = (current_value > max_value) ? max_value : current_value;
    current_value = (current_value < 0) ? 0 : current_value;
    lv_slider_set_value(slider, current_value, LV_ANIM_OFF);
       
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    rgb._red= current_value;    
    lv_led_set_color(led, lv_color_make(rgb._red,rgb._green,rgb._blue));
    encoder_value =current_value;
}

static void green_callback(lv_event_t * e)
{
    
    int increment = 0;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    
    int current_value = lv_slider_get_value(slider1);
    if (current_value >  encoder_value1){
      current_value+=5;
    } else if (current_value <  encoder_value1) {
      current_value-=5;
    }
 
    // Verifique se o novo valor está dentro do intervalo permitido pelo slider
    int max_value = lv_slider_get_max_value(slider);
    current_value = (current_value > max_value) ? max_value : current_value;
    current_value = (current_value < 0) ? 0 : current_value;
    lv_slider_set_value(slider1, current_value, LV_ANIM_OFF);
       
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_slider_get_value(slider1));
    lv_label_set_text(slider_label1, buf);
    lv_obj_align_to(slider_label1, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    rgb._green= current_value;   
    lv_led_set_color(led, lv_color_make(rgb._red,rgb._green,rgb._blue));
    encoder_value1 =current_value;
}

static void blue_callback(lv_event_t * e)
{
    
    int increment = 0;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    
    int current_value = lv_slider_get_value(slider2);
    if (current_value >  encoder_value2){
      current_value+=5;
    } else if (current_value <  encoder_value2) {
      current_value-=5;
    }
 
    // Verifique se o novo valor está dentro do intervalo permitido pelo slider
    int max_value = lv_slider_get_max_value(slider2);
    current_value = (current_value > max_value) ? max_value : current_value;
    current_value = (current_value < 0) ? 0 : current_value;
    lv_slider_set_value(slider2, current_value, LV_ANIM_OFF);
       
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d", (int)lv_slider_get_value(slider2));
    lv_label_set_text(slider_label2, buf);
    lv_obj_align_to(slider_label2, slider2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    rgb._blue= current_value;    
    lv_led_set_color(led, lv_color_make(rgb._red,rgb._green,rgb._blue));
    encoder_value2 =current_value;
}


void config_window(lv_obj_t *parent)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, 260, 140);
    lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 15);

    led  = lv_led_create(container);
    lv_obj_align(led, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_led_set_color(led, lv_color_make(rgb._red,rgb._green,rgb._blue));


    static lv_style_t style_indic;
    static lv_style_t style_knob;
    
  
    lv_style_init(&style_indic);
    lv_style_set_bg_color(&style_indic, lv_palette_darken(LV_PALETTE_RED,3));

    lv_style_init(&style_knob);
    lv_style_set_bg_color(&style_knob, lv_palette_darken(LV_PALETTE_RED,3));
    lv_style_set_border_color(&style_knob,  lv_palette_darken(LV_PALETTE_RED,3));
    lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob, 1);
   
    // Crie o slider
    slider = lv_slider_create(container);
    lv_obj_add_style( slider , &style_indic, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
    lv_obj_set_size(slider, 100, 10);
    lv_obj_align(slider, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_slider_set_range(slider, 0, 255);
     
    slider_label = lv_label_create(container);
    lv_label_set_text(slider_label, "0");
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_event_cb(slider, red_callback,LV_EVENT_KEY, NULL);



    static lv_style_t style_indic1;
    static lv_style_t style_knob1;
    
    lv_style_init(&style_indic1);
    lv_style_set_bg_color(&style_indic1, lv_palette_darken(LV_PALETTE_GREEN,3));

    lv_style_init(&style_knob1);
    lv_style_set_bg_color(&style_knob1, lv_palette_darken(LV_PALETTE_GREEN,3));
    lv_style_set_border_color(&style_knob1,  lv_palette_darken(LV_PALETTE_GREEN,3));
    lv_style_set_radius(&style_knob1, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob1, 1);
   
    // Crie o slider
    slider1 = lv_slider_create(container);
    lv_obj_add_style( slider1 , &style_indic1, LV_PART_INDICATOR);
    lv_obj_add_style(slider1, &style_knob1, LV_PART_KNOB);
    lv_obj_set_size(slider1, 100, 10);
    lv_obj_align(slider1, LV_ALIGN_TOP_LEFT, 0, 35);
    lv_slider_set_range(slider1, 0, 255);
     
    slider_label1 = lv_label_create(container);
    lv_label_set_text(slider_label1, "0");
    lv_obj_align_to(slider_label1, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_event_cb(slider1, green_callback,LV_EVENT_KEY, NULL);


    static lv_style_t style_indic2;
    static lv_style_t style_knob2;
    
    lv_style_init(&style_indic2);
    lv_style_set_bg_color(&style_indic2, lv_palette_darken(LV_PALETTE_BLUE,3));

    lv_style_init(&style_knob2);
    lv_style_set_bg_color(&style_knob2, lv_palette_darken(LV_PALETTE_BLUE,3));
    lv_style_set_border_color(&style_knob2,  lv_palette_darken(LV_PALETTE_BLUE,3));
    lv_style_set_radius(&style_knob2, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob2, 1);
   
    // Crie o slider
    slider2 = lv_slider_create(container);
    lv_obj_add_style( slider2 , &style_indic2, LV_PART_INDICATOR);
    lv_obj_add_style(slider2, &style_knob2, LV_PART_KNOB);
    lv_obj_set_size(slider2, 100, 10);
    lv_obj_align(slider2, LV_ALIGN_TOP_LEFT, 0, 70);
    lv_slider_set_range(slider2, 0, 255);
     
    slider_label2 = lv_label_create(container);
    lv_label_set_text(slider_label2, "0");
    lv_obj_align_to(slider_label2, slider2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_add_event_cb(slider2, blue_callback,LV_EVENT_KEY, NULL);
    
    lv_obj_t *btn = lv_btn_create(container);
    lv_obj_add_event_cb(btn,call_rgb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn,  LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_t *label1 = lv_label_create(btn);
    lv_label_set_text(label1, "set");
   
}
static void call_rgb(lv_event_t * e)
{
   rgb_init();
}
void runConfigWindow(lv_obj_t *parent)
{
    config_window(parent);
}

app_t config_ui = {
    .setup_func_cb = runConfigWindow,
    .exit_func_cb = nullptr,
    .user_data = nullptr
};
