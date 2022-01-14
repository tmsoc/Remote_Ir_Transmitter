

#include <Arduino.h>

// IRremote by Armin Joachimsmeyer
// https://registry.platformio.org/libraries/z3t0/IRremote
// https://github.com/Arduino-IRremote/Arduino-IRremote?utm_source=platformio&utm_medium=piohome
// #include <IRremote.hpp>

#include "controller.h"
#include "Ir_lcd.h"
#include "IR_util.h"
#include "NavButtons.h"

// lcd display
Ir_lcd lcd(LCD_ADDRESS, LCD_COL_CNT, LCD_ROW_CNT);
bool backLightSetting = false;
// navigational buttons
bool navArray[NAV_SIZE];
NavButtons navInput(LEFT_PIN, RIGHT_PIN, UP_PIN, DOWN_PIN, navArray);
// for main view display
uint8_t currentView = LCD_MAIN_VIEW;

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.initializing_v(3);
    delay(2000);

    // ----- SECTION TO IMPORT PROGRAM DATA ------

    lcd.transmitInfo_v("", "");
    lcd.setBacklightMode(backLightSetting);   // TODO - Need to import from sd card
    lcd.setBacklightTimer(ONE_MIN / 4); // TODO - Remove offset
} // end of setup

void loop() {
    lcd.backlightTimeout();
    if (navInput.readInputs()) {
        lcd.setBacklightTimer(ONE_MIN / 4);
        // going from the main view to settings
        if (currentView == LCD_MAIN_VIEW && navArray[navInput.UP] == HIGH && navArray[navInput.DOWN] == HIGH) {
            currentView = 0;
            lcd.setup_v(IR_Util::menuList[currentView]);
            lcd.backlight(true);
        } 
        // exiting settings to the main view
        else if (currentView != LCD_MAIN_VIEW && navArray[navInput.LEFT] == HIGH) {
            currentView = LCD_MAIN_VIEW;
            lcd.transmitInfo_v("", "");
            lcd.backlight(backLightSetting);
        } 
        // iterating through menu options within the settings
        else if (currentView != LCD_MAIN_VIEW && (navArray[navInput.UP] == HIGH || navArray[navInput.DOWN] == HIGH)) {
            if (navArray[navInput.UP] == HIGH) {
                currentView = (currentView + 1) % MENU_CNT;
            }
            else {
                currentView = (currentView == 0) ? MENU_CNT - 1 : --currentView;
            }
            lcd.setup_v(IR_Util::menuList[currentView]);
        }
        // entering a settings submenu
        else if (currentView != LCD_MAIN_VIEW && navArray[navInput.RIGHT]) {
            // TODO - replace with switch

            // program to record new ir command
            if (currentView == 0) {
                lcd.irSignalInfo_v("protocal", 0x21, 0x34);
                delay(2000);

            }
            else if (currentView == 1) {
                lcd.ipInfo_v(IPAddress(192,168,1,1));
                delay(2000);
            }
            else if (currentView == 2) {
                lcd.backlightSetting_v();
                delay(2000);
            }

            // TODO - Temp code to go back to settings
            lcd.setup_v(IR_Util::menuList[currentView]);
            // TODO - End of temp code
        }
    }
} // end of loop