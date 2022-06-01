/**
 * @file Ir_lcd.h
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief Wrapper class for the LiquidCrystal_I2C lib
 * @version 1.0
 * @date 2022-01-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef IR_LCD
#define IR_LCD

// LiquidCrystal_I2C by Frank de Brabander version 1.1.4
//https://github.com/johnrickman/LiquidCrystal_I2C?utm_source=platformio&utm_medium=piohome
#include <LiquidCrystal_I2C.h>
#include "MsTimer.h"    // used for backlight

class Ir_lcd {
public:

    Ir_lcd(uint8_t addr, uint8_t cols, uint8_t rows);

    void init();

    void backlight(bool on);

    void setBacklightTimer(unsigned int ms);

    void backlightTimeout();

    void setBacklightMode(bool alwaysOn);

    void initializing_v(uint8_t periodCnt);

    void transmitInfo_v(const String &dev, const String &func);

    void setup_v(const String &menu);

    void ipInfo_v(const IPAddress &ip);
    
    void deviceSelect_v(const String &dev);
    
    void funcSelect_v(const String &func);
    
    void irSignalInfo_v(const String &protocal, uint16_t address, uint8_t command);

    void wifiNotConnected_v();

    void saving_v();

    void saveChange_v();

    void backlightSetting_v();

private:

    void printLeft();
    void printRight();
    void printUp();
    void printDown();
    void printUpDown();
    void createChar();
    void headerFooter(const String &header, const String &footer);

    LiquidCrystal_I2C lcd;
    MsTimer timer;
    bool bl_alwaysOn;
};

#endif