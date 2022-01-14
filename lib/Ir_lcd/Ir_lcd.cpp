/**
 * @file Ir_lcd.cpp
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief Wrapper class for the LiquidCrystal_I2C lib
 * @version 1.0
 * @date 2022-01-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include "Ir_lcd.h"

byte left[8] =   {0x1, 0x3, 0x7, 0xf, 0x7, 0x3, 0x1, 0x0};
byte right[8] =  {0x10, 0x18, 0x1c, 0x1e, 0x1c, 0x18, 0x10, 0x0};
byte up[8] =     {0x4, 0xe, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0};
byte down[8] =   {0x0, 0x0, 0x0, 0x0, 0x1f, 0xe, 0x4, 0x0};
byte upDown[8] = {0x4, 0xe, 0x1f, 0x0, 0x1f, 0xe, 0x4, 0x0};

Ir_lcd::Ir_lcd(uint8_t addr, uint8_t cols, uint8_t rows)
:  lcd(addr, cols, rows), timer() {}

void Ir_lcd::init() {
    lcd.init();
    createChar();   // has to be called after init and before everything else!!
    lcd.backlight();
    lcd.clear();
}

void Ir_lcd::backlight(bool on) {
    bl_alwaysOn = on;
    if (on) lcd.backlight();
    else    lcd.noBacklight();
}

void Ir_lcd::setBacklightTimer(unsigned int ms) {
    timer.setTimer(ms);
    lcd.backlight();
}

void Ir_lcd::backlightTimeout() {
    if (bl_alwaysOn || timer.getStatus())
        lcd.backlight();
    else
        lcd.noBacklight();
}

void Ir_lcd::setBacklightMode(bool alwaysOn) {
    bl_alwaysOn = alwaysOn;
}

void Ir_lcd::initializing_v(uint8_t periodCnt) {
    lcd.clear();
    lcd.print("Initializing");
    // 4 is the max number of perods
    uint8_t iter = min(periodCnt, 4); 
    for (uint8_t i = 0; i < iter; i++)
        lcd.print(".");
}

void Ir_lcd::transmitInfo_v(const String &dev, const String &func) {
    lcd.clear();
    lcd.print("Dev: ");
    lcd.print(dev);
    lcd.setCursor(0, 1);
    lcd.print("Func: ");
    lcd.print(func);
}

void Ir_lcd::setup_v(const String &menu) {
    headerFooter("Settings Menu:", menu);
}

void Ir_lcd::ipInfo_v(const IPAddress &ip) {
    lcd.clear();
    lcd.print("IP:");
    lcd.setCursor(0, 1);
    lcd.print(ip);
}

void Ir_lcd::deviceSelect_v(const String &dev) {
    headerFooter("Device:", dev);
}

void Ir_lcd::funcSelect_v(const String &func) {
    headerFooter("Function:", func);
}

void Ir_lcd::irSignalInfo_v(const String &protocal, uint16_t address, uint8_t command) {
    lcd.clear();
    lcd.print("P: ");
    lcd.print(protocal);
    lcd.setCursor(0, 1);
    lcd.print("A: ");
    lcd.print(address, HEX);
    lcd.setCursor(8, 1);
    lcd.print("C: ");
    lcd.print(command, HEX);
}

void Ir_lcd::saving_v() {
    lcd.clear();
    lcd.print("Saving...");
}

void Ir_lcd::saveChange_v() {
    lcd.clear();
    lcd.print("Save Changes:");
    lcd.setCursor(0, 1);
    printUpDown();
    lcd.print("Save ");
    printLeft();
    printRight();
    lcd.print("Exit");
}

void Ir_lcd::backlightSetting_v() {
    lcd.clear();
    lcd.print("Backlight:");
    lcd.setCursor(0, 1);
    printUpDown();
    lcd.print("On ");
    printLeft();
    printRight();
    lcd.print("Off");
}


// ------------- PRIVATE FUNCTIONS ----------------------


void Ir_lcd::printLeft() {
    lcd.write(0);
}

void Ir_lcd::printRight() { 
    lcd.write(1); 
}

void Ir_lcd::printUp() {
    lcd.write(2); 
}
void Ir_lcd::printDown() {
    lcd.write(3);
}
void Ir_lcd::printUpDown() {
    lcd.write(4);
}

void Ir_lcd::createChar() {
    lcd.createChar(0, left);
    lcd.createChar(1, right);
    lcd.createChar(2, up);
    lcd.createChar(3, down);
    lcd.createChar(4, upDown);
}

void Ir_lcd::headerFooter(const String &header, const String &footer) {
    lcd.clear();
    lcd.print(header);
    lcd.setCursor(0, 1);
    lcd.print(footer);
}