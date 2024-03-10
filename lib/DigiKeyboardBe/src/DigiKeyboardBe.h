#include "DigiKeyboard.h"
#ifndef DIGIKEYBOARDBE_H
#define DIGIKEYBOARDBE_H

#define BE_MOD_CONTROL_LEFT  (1<<8)
#define BE_MOD_SHIFT_LEFT    (1<<9)
#define BE_MOD_ALT_LEFT      (1<<10)
#define BE_MOD_GUI_LEFT      (1<<11)
#define BE_MOD_CONTROL_RIGHT (1<<12)
#define BE_MOD_SHIFT_RIGHT   (1<<13)
#define BE_MOD_ALT_RIGHT     (1<<14)
#define BE_MOD_GUI_RIGHT     (1<<15)

#define KEY_ARROW_RIGHT 79
#define KEY_ARROW_UP 82
#define KEY_ARROW_DOWN 81
#define KEY_ARROW_LEFT 80

#define KEY_DELETE 0x4C
#define KEY_BACKSPACE 0x2A

#define KEY_CAPSLOCK 57
#define KEY_NUMLOCK 83
#define KEY_ESC 41
#define KEY_PRINT_SCREEN 70
#define KEY_INSERT 0x49
#define KEY_END 0x4D

#define KEY_MUTE 0x7F
#define KEY_VOLUME_UP 0x80
#define KEY_VOLUME_DOWN 0x81
#define KEY_COPY 0x7C
#define KEY_PASTE 0x7D
#define KEY_CUT 0x7B

#define KEY_F1 0x3A
#define KEY_F2 0x3B
#define KEY_F3 0x3C
#define KEY_F4 0x3D
#define KEY_F5 0x3E
#define KEY_F6 0x3F
#define KEY_F7 0x40
#define KEY_F8 0x41
#define KEY_F9 0x42
#define KEY_F10 0x43
#define KEY_F11 0x44
#define KEY_F12 0x45


#define KEY_BE_A       20
#define KEY_BE_B       5
#define KEY_BE_C       6
#define KEY_BE_D       7
#define KEY_BE_E       8
#define KEY_BE_F       9
#define KEY_BE_G       10
#define KEY_BE_H       11
#define KEY_BE_I       12
#define KEY_BE_J       13
#define KEY_BE_K       14
#define KEY_BE_L       15
#define KEY_BE_M       51
#define KEY_BE_N       17
#define KEY_BE_O       18
#define KEY_BE_P       19
#define KEY_BE_Q       4
#define KEY_BE_R       21
#define KEY_BE_S       22
#define KEY_BE_T       23
#define KEY_BE_U       24
#define KEY_BE_V       25
#define KEY_BE_W       29
#define KEY_BE_X       27
#define KEY_BE_Y       28
#define KEY_BE_Z       26

#define KEY_BE_0       98
#define KEY_BE_1       89
#define KEY_BE_2       90
#define KEY_BE_3       91
#define KEY_BE_4       92
#define KEY_BE_5       93
#define KEY_BE_6       94
#define KEY_BE_7       95
#define KEY_BE_8       96
#define KEY_BE_9       97

const uint16_t ascii_to_scan_code_table_be[] PROGMEM = {
        0x00,                 // NUL
        0x00,                 // SOH
        0x00,                 // STX
        0x00,                 // ETX
        0x00,                 // EOT
        0x00,                 // ENQ
        0x00,                 // ACK
        0x00,                 // BEL
        42,                   // BS Backspace
        43,                   // TAB  Tab
        40,                   // LF Enter
        0x00,                 // VT
        0x00,                 // FF
        0x00,                 // CR
        0x00,                 // SO
        0x00,                 // SI
        0x00,                 // DEL
        0x00,                 // DC1
        0x00,                 // DC2
        0x00,                 // DC3
        0x00,                 // DC4
        0x00,                 // NAK
        0x00,                 // SYN
        0x00,                 // ETB
        0x00,                 // CAN
        0x00,                 // EM
        0x00,                 // SUB
        0x00,                 // ESC
        0x00,                 // FS
        0x00,                 // GS
        0x00,                 // RS
        0x00,                 // US
        44,                   // ' '
        37,                   // !
        32,                   // "
        32|BE_MOD_ALT_RIGHT,  // #
        48,                   // $
        52|BE_MOD_SHIFT_LEFT, // %
        30,                   // &
        33,                   // '
        34,                   // (
        45,                   // )
        48|BE_MOD_SHIFT_LEFT, // *
        56|BE_MOD_SHIFT_LEFT, // +
        16,                   // ,
        46,                   // -
        54|BE_MOD_SHIFT_LEFT, // .
        55|BE_MOD_SHIFT_LEFT, // /
        39|BE_MOD_SHIFT_LEFT, // 0
        30|BE_MOD_SHIFT_LEFT, // 1
        31|BE_MOD_SHIFT_LEFT, // 2
        32|BE_MOD_SHIFT_LEFT, // 3
        33|BE_MOD_SHIFT_LEFT, // 4
        34|BE_MOD_SHIFT_LEFT, // 5
        35|BE_MOD_SHIFT_LEFT, // 6
        36|BE_MOD_SHIFT_LEFT, // 7
        37|BE_MOD_SHIFT_LEFT, // 8
        38|BE_MOD_SHIFT_LEFT, // 9
        55,                   // :
        54,                   // ;
        100,                  // <
        56,                   // =
        100|BE_MOD_SHIFT_LEFT,// >
        16|BE_MOD_SHIFT_LEFT, // ?
        31|BE_MOD_ALT_RIGHT,  // @
        20|BE_MOD_SHIFT_LEFT, // A
        5|BE_MOD_SHIFT_LEFT,  // B
        6|BE_MOD_SHIFT_LEFT,  // C
        7|BE_MOD_SHIFT_LEFT,  // D
        8|BE_MOD_SHIFT_LEFT,  // E
        9|BE_MOD_SHIFT_LEFT,  // F
        10|BE_MOD_SHIFT_LEFT, // G
        11|BE_MOD_SHIFT_LEFT, // H
        12|BE_MOD_SHIFT_LEFT, // I
        13|BE_MOD_SHIFT_LEFT, // J
        14|BE_MOD_SHIFT_LEFT, // K
        15|BE_MOD_SHIFT_LEFT, // L
        51|BE_MOD_SHIFT_LEFT, // M
        17|BE_MOD_SHIFT_LEFT, // N
        18|BE_MOD_SHIFT_LEFT, // O
        19|BE_MOD_SHIFT_LEFT, // P
        4|BE_MOD_SHIFT_LEFT,  // Q
        21|BE_MOD_SHIFT_LEFT, // R
        22|BE_MOD_SHIFT_LEFT, // S
        23|BE_MOD_SHIFT_LEFT, // T
        24|BE_MOD_SHIFT_LEFT, // U
        25|BE_MOD_SHIFT_LEFT, // V
        29|BE_MOD_SHIFT_LEFT, // W
        27|BE_MOD_SHIFT_LEFT, // X
        28|BE_MOD_SHIFT_LEFT, // Y
        26|BE_MOD_SHIFT_LEFT, // Z
        34|BE_MOD_ALT_RIGHT,  // [
        29|BE_MOD_ALT_RIGHT,  // bslash
        48|BE_MOD_ALT_RIGHT,  // ]
        47,                   // ^
        46|BE_MOD_SHIFT_LEFT, // _
        52|BE_MOD_ALT_RIGHT,  // `
        20,                   // a
        5,                    // b
        6,                    // C
        7,                    // d
        8,                    // e
        9,                    // f
        10,                   // g
        11,                   // h
        12,                   // i
        13,                   // j
        14,                   // k
        15,                   // l
        51,                   // m
        17,                   // n
        18,                   // O
        19,                   // P
        4,                    // Q
        21,                   // r
        22,                   // s
        23,                   // t
        24,                   // u
        25,                   // v
        29,                   // w
        27,                   // x
        28,                   // y
        26,                   // z
        38|BE_MOD_ALT_RIGHT,  // {
        30|BE_MOD_ALT_RIGHT,  // |
        39|BE_MOD_ALT_RIGHT,  // }
        56|BE_MOD_ALT_RIGHT,  // ~
        0,                    // DEL
        38,                   //Ç
        24,                   //ü
        31,                   //é
        20,                   //â
        20,                   //ä
        39,                   //à
        20,                   //å
        38,                   //ç
        8,                   //ê
        8,                   //ë
        36,                   //è
};

#undef BE_MOD_CONTROL_LEFT
#undef BE_MOD_SHIFT_LEFT
#undef BE_MOD_ALT_LEFT
#undef BE_MOD_GUI_LEFT
#undef BE_MOD_CONTROL_RIGHT
#undef BE_MOD_SHIFT_RIGHT
#undef BE_MOD_ALT_RIGHT
#undef BE_MOD_GUI_RIGHT

class DigiKeyboardDeviceBe : public DigiKeyboardDevice
{
    public:
    size_t write(uint8_t chr) 
    {
        unsigned int temp = pgm_read_word_near(ascii_to_scan_code_table_be + chr);
        unsigned char low = temp & 0xFF;
        unsigned char high = (temp >> 8) & 0xFF;
        sendKeyStroke(low, high);
        return 1;
    }
  
    void sendKeyReport(uchar *array,const unsigned int size)
    {
        while (!usbInterruptIsReady()) 
        {
            // Note: We wait until we can send keyPress
            //       so we know the previous keyPress was
            //       sent.
            usbPoll();
            _delay_ms(5);
        }

        usbSetInterrupt(array, size);
    }
};

DigiKeyboardDeviceBe DigiKeyboardBe = DigiKeyboardDeviceBe();

#endif //DIGIKEYBOARDBE_H