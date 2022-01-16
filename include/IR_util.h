#ifndef IR_UTIL_H
#define IR_UTIL_H

#include <Arduino.h>

#define LEFT_PIN 5
#define RIGHT_PIN 6
#define UP_PIN 7
#define DOWN_PIN 8

#define PROTOCOL_BYTE_SIZE 1
#define ADDRESS_BYTE_SIZE 2
#define COMMAND_BYTE_SIZE 2
#define EXTRA_BYTE_SIZE 2
#define NUMBEROFBITS_BYTE_SIZE 2
#define FLAGS_BYTE_SIZE 1
#define DECODED_RAW_DATA_BYTE_SIZE 4
#define IR_MESSAGE_OFFSET 14

namespace IR_Util {

    #define FUNC_CNT 18
    const String ir_func[] = {  // INDEX
        "Power",                // 0
        "Back/Exit",            // 1
        "Home",                 // 2
        "Up",                   // 3
        "Down",                 // 4
        "Left",                 // 5
        "Right",                // 6
        "Select",               // 7
        "Menu",                 // 8
        "Vol. Up",              // 9
        "Vol. Down",            // 10
        "Ch. Up",               // 11
        "Ch. Down",             // 12
        "Mute",                 // 13
        "Input",                // 14
        "TV Input",             // 15
        "DVD Input",            // 16
        "Phono Input"           // 17
    };

    // // const int size = sizeof(ir_func) / sizeof(ir_func[0]);
    // #define FUNC_SIZE sizeof(IR_Util::ir_func) / sizeof(IR_Util::ir_func[0])

    #define DEV_CNT 2
    const String devices[] = {"TV", "Receiver"};

    #define MENU_CNT 3
    const String menuList[] = {"Programmer", "IP Address", "LCD Backlight"};

} // namespace IR

#define LCD_ADDRESS 0x27
#define LCD_COL_CNT 16
#define LCD_ROW_CNT 2

#define LCD_MAIN_VIEW MENU_CNT + 1

#define ONE_MIN 60000



#endif /* IR_UTIL_H */