#ifndef IR_UTIL_H
#define IR_UTIL_H

#include <Arduino.h>

#define LEFT_PIN 0
#define RIGHT_PIN 1
#define UP_PIN 4
#define DOWN_PIN 5

#define PROTOCOL_BYTE_SIZE 1
#define ADDRESS_BYTE_SIZE 2
#define COMMAND_BYTE_SIZE 2
#define EXTRA_BYTE_SIZE 2
#define NUMBEROFBITS_BYTE_SIZE 2
#define FLAGS_BYTE_SIZE 1
#define DECODED_RAW_DATA_BYTE_SIZE 4
#define IR_MESSAGE_OFFSET 14

#define IR_RECEIVE_PIN 2
#define IR_SEND_PIN 3
// #define ENABLE_LED_FEEDBACK true
// #define IR_FEEDBACK_PIN 6

#define UDP_PORT 6000
#define MAX_UDP_BUFFER 32
#define UDP_HEADER_SIZE 5
#define MESSAGE_TYPE_INDEX 5
#define REMOTE_MESSAGE_TYPE 0x52
#define STATUS_MESSAGE_TYPE 0x53
#define CONFIG_MESSAGE_TYPE 0x43
#define REMOTE_MESSAGE_SIZE 8

namespace UDP_Util {

    const byte UDP_HEADER[] = { 0x01, 0x02, 0x04, 0x08, 0x10 };
    
} // end of UDP_Util

namespace IR_Util {

    #define FUNC_CNT 18
    const String IR_FUNC[] = {  // INDEX
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
    const String DEVICES[] = {"TV", "Receiver"};

    #define MENU_CNT 3
    const String MENU_LIST[] = {"Programmer", "IP Address", "LCD Backlight"};
    
    #define PROGRAMMER_VIEW 0
    #define NETWORK_VIEW 1
    #define BACKLIGHT_VIEW 2
    #define LCD_MAIN_VIEW MENU_CNT + 1

} // namespace IR

#define LCD_ADDRESS 0x27
#define LCD_COL_CNT 16
#define LCD_ROW_CNT 2


#define ONE_MIN 60000



#endif /* IR_UTIL_H */