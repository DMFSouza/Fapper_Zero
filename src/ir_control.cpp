#include "ir_control.h"
#include "ui.h"
#include "global_flags.h"
#include "pin_config.h"
#include "WORLD_IR_CODES.h"
#include <FreeRTOS.h>
#include "IRremoteESP8266.h"
#include "IRsend.h"


void sendAllCodes();
uint8_t read_bits(uint8_t count);
void delay_ten_us(uint16_t us);

// Constantes
#define PWM_CHANNEL 0
#define PWM_FREQ 38000
#define PWM_RESOLUTION 8
#define MAX_WAIT_TIME 65535
#define DELAY_CNT 10
#define NOP __asm__ __volatile__("nop\n\t")
#define NUM_CORES 2


void delay_ten_us(uint16_t us);
uint8_t read_bits(uint8_t count);
void sendAllCodes(); 

void ir_transmitter(lv_event_t *e);
uint8_t read_bits(uint8_t count);
uint16_t rawData[300];
void ir_gui(lv_obj_t *parent);

#define MAX_WAIT_TIME 65535 //tens of us (ie: 655.350ms)

IRsend irsend(IRLED);


extern const IrCode* const NApowerCodes[];
extern const IrCode* const EUpowerCodes[];
extern uint8_t num_NAcodes, num_EUcodes;

uint8_t bitsleft_r = 0;
uint8_t bits_r=0;
uint16_t code_ptr = 0;
volatile const IrCode * powerCode;

uint16_t ontime, offtime;
uint8_t i,num_codes;
uint8_t region;



void ir_gui(lv_obj_t *parent)
{

    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
    lv_obj_t *btn = lv_btn_create(container);
    lv_obj_set_size(btn, 35, 35);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);
    lv_obj_add_event_cb(btn,ir_transmitter, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label,LV_SYMBOL_POWER);
    lv_obj_align(btn,  LV_ALIGN_CENTER, 0,0);
    


    lv_obj_t *btn1 = lv_btn_create(container);
    lv_obj_set_size(btn1,35, 35);
    lv_obj_set_style_bg_color(btn1, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn1, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label1 = lv_label_create(btn1);
    lv_obj_center(label1);
    lv_label_set_text(label1,LV_SYMBOL_PLUS);
    lv_obj_align(btn1,  LV_ALIGN_CENTER, +50,0);



    lv_obj_t *btn2 = lv_btn_create(container);
    lv_obj_set_size(btn2, 35, 35);
    lv_obj_set_style_bg_color(btn2, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn2, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label2 = lv_label_create(btn2);
    lv_obj_center(label2);
    lv_label_set_text(label2,LV_SYMBOL_MINUS);
    lv_obj_align(btn2,  LV_ALIGN_CENTER, -50,0);



    lv_obj_t *btn3 = lv_btn_create(container);
    lv_obj_set_size(btn3, 35, 35);
    lv_obj_set_style_bg_color(btn3, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn3, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label3 = lv_label_create(btn3);
    lv_obj_center(label3);
    lv_label_set_text(label3,LV_SYMBOL_UP);
    lv_obj_align(btn3,  LV_ALIGN_CENTER, 0,-50);


    lv_obj_t *btn4 = lv_btn_create(container);
    lv_obj_set_size(btn4, 35, 35);
    lv_obj_set_style_bg_color(btn4, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_outline_color(btn4, lv_color_white(), LV_STATE_FOCUS_KEY);

    lv_obj_t *label4 = lv_label_create(btn4);
    lv_obj_center(label4);
    lv_label_set_text(label4,LV_SYMBOL_DOWN);
    lv_obj_align(btn4,  LV_ALIGN_CENTER, 0,+50);
    

   
}


void ir_transmitter(lv_event_t *e) {
   
    irsend.begin();
     Serial.println("entrou o task");

    region = true; // Define a região como NA
    Serial.println("funcao principal");

    for (int nX = 0; nX < 10; nX++) {
         delay_ten_us(500);
         
         yield();
         sendAllCodes();
    }
      yield();
}
 
void sendAllCodes() 
{
  bool endingEarly = false; //will be set to true if the user presses the button during code-sending 
      
  // determine region from REGIONSWITCH: 1 = NA, 0 = EU (defined in main.h)
  //if (digitalRead(REGIONSWITCH)) {
   // region = NA;
  //  num_codes = num_NAcodes;
  //}
 // else {
    region = EU;
    num_codes = num_EUcodes;
 // }

  // for every POWER code in our collection
  for (i=0 ; i<num_codes; i++) 
  {

    // point to next POWER code, from the right database
    if (region == NA) {
      powerCode = NApowerCodes[i];
    }
    else {
      powerCode = EUpowerCodes[i];
    }
    
  
    const uint8_t freq = powerCode->timer_val;
  
    const uint8_t numpairs = powerCode->numpairs;
   
    const uint8_t bitcompression = powerCode->bitcompression;
 
    code_ptr = 0;
    for (uint8_t k=0; k<numpairs; k++) {
      uint16_t ti;

      // Read the next 'n' bits as indicated by the compression variable
      // The multiply by 4 because there are 2 timing numbers per pair
      // and each timing number is one word long, so 4 bytes total!
      ti = (read_bits(bitcompression)) * 2;

      // read the onTime and offTime from the program memory
      ontime = powerCode->times[ti];  // read word 1 - ontime
      offtime = powerCode->times[ti+1];  // read word 2 - offtime

     

      rawData[k*2] = ontime * 10;
      rawData[(k*2)+1] = offtime * 10;
      yield();
    }

    // Send Code with library
    irsend.sendRaw(rawData, (numpairs*2) , freq);
    Serial.print("\n");
    yield();
    //Flush remaining bits, so that next code starts
    //with a fresh set of 8 bits.
    bitsleft_r=0;

    // delay 205 milliseconds before transmitting next POWER code
    delay_ten_us(20500);
  
        yield();
      
      endingEarly = true;
      delay_ten_us(50000); //500ms delay 
     
      //pause for ~1.3 sec to give the user time to release the button so that the code sequence won't immediately start again.
      delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
      delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
      break; //exit the POWER code "for" loop
    
    
  } //end of POWER code for loop

  if (endingEarly==false)
  {
    //pause for ~1.3 sec, then flash the visible LED 8 times to indicate that we're done
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
    delay_ten_us(MAX_WAIT_TIME); // wait 655.350ms
   
  }

} 


uint8_t read_bits(uint8_t count)
{
  uint8_t i;
  uint8_t tmp=0;

  // we need to read back count bytes
  for (i=0; i<count; i++) {
    // check if the 8-bit buffer we have has run out
    if (bitsleft_r == 0) {
      // in which case we read a new byte in
      bits_r = powerCode->codes[code_ptr++];
      
      // and reset the buffer size (8 bites in a byte)
      bitsleft_r = 8;
    }
    // remove one bit
    bitsleft_r--;
    // and shift it off of the end of 'bits_r'
    tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count-1-i));
  }
  // return the selected bits in the LSB part of tmp
  return tmp;
}

void delay_ten_us(uint16_t us) {
  uint8_t timer;
  while (us != 0) {
    // for 8MHz we want to delay 80 cycles per 10 microseconds
    // this code is tweaked to give about that amount.
    for (timer=0; timer <= DELAY_CNT; timer++) {
      NOP;
      NOP;
    }
    NOP;
    us--;
  }
}


void runIRWindow(lv_obj_t *parent)
{
  
  ir_gui(parent);

}


app_t ir_ui = {
    .setup_func_cb = runIRWindow,
    .exit_func_cb = nullptr,
    .user_data = nullptr
};
