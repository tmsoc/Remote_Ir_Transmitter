#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <IRremote.hpp>

#include "Controller.h"
#include "Ir_lcd.h"
#include "IR_util.h"
#include "NavButtons.h"

// WiFi_Secrets.h contains the wifi ssid and password
// contains the following definitions
// #define SECRET_SSID ""
// #define SECRET_PASS ""
#include "WiFi_Secrets.h"

// 2D array of stored IR messages
// IRData messages[DEV_CNT][FUNC_CNT];

// lcd display
Ir_lcd lcd(LCD_ADDRESS, LCD_COL_CNT, LCD_ROW_CNT);
// navigational buttons
bool navArray[NAV_SIZE];
NavButtons navInput(LEFT_PIN, RIGHT_PIN, UP_PIN, DOWN_PIN, navArray);

// // for main view display
// uint8_t currentView = LCD_MAIN_VIEW;

// The controller
Controller ctrl(lcd, navInput, navArray); // Needs functions, sender, & receiver

void setup() {
    // Serial.begin(115200);

    lcd.init();
    lcd.initializing_v(3);
    delay(2000);

    // ----- SECTION TO IMPORT PROGRAM DATA ------

    lcd.transmitInfo_v("", "");
    ctrl.setBacklightMode(false);       // TODO - Need to import from data
    lcd.setBacklightTimer(ONE_MIN / 4); // TODO - Remove offset
    
}   // end of setup()

void loop() {
    lcd.backlightTimeout();

    if (navInput.readInputs()) {
        ctrl.homeViewBtnPress();
    }

}   // end of loop()