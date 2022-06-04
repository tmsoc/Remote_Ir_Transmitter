#ifndef IR_UTIL_H
#define IR_UTIL_H

#include <Arduino.h>

// #include "EEPROM_24xx64.h"

#define UP_PIN 0
#define DOWN_PIN 1
#define LEFT_PIN 4
#define RIGHT_PIN 5

#define ASSIGNED_BYTE_SIZE 1
#define PROTOCOL_BYTE_SIZE 1
#define ADDRESS_BYTE_SIZE 2
#define COMMAND_BYTE_SIZE 2
#define FUNCTION_SIZE 6

#define IR_RECEIVE_PIN 2
#define IR_SEND_PIN 3
#define IR_REPEAT_CNT 0

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

    #define PROGRAM_OPTION_CNT 2
    const String PROGRAM_OPTION_LIST[] = {"Assign", "Delete"};

    #define ASSIGN_FUNCTION 0
    #define DELETE_FUNCTION 1
    
    #define PROGRAMMER_VIEW 0
    #define NETWORK_VIEW 1
    #define BACKLIGHT_VIEW 2
    #define LCD_MAIN_VIEW MENU_CNT + 1

    /**
     * The required stored data to transmit, store, and send 
     * IR messages.
     */
    struct IRFunction {
        bool assigned;
        uint8_t protocol;
        uint16_t address;
        uint16_t command;
    };

} // namespace IR_Util

#define LCD_ADDRESS 0x27
#define LCD_COL_CNT 16
#define LCD_ROW_CNT 2


// typedef EEPROM_24xx64 Eeprom;
#define EEPROM_ADDRESS 0x50
#define BACKLIGHT_EEPROM_OFFSET 1000


#define ONE_MIN 60000
#define THIRTY_SEC 30000
#define FIVE_SEC 5000
#define THREE_SEC 3000


#endif /* IR_UTIL_H */