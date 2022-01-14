

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
// navigational buttons
bool navArray[NAV_SIZE];
NavButtons navInput(LEFT_PIN, RIGHT_PIN, UP_PIN, DOWN_PIN, navArray);

// // for main view display
// uint8_t currentView = LCD_MAIN_VIEW;

// The controller
Controller ctr(lcd, navInput, navArray);


void setup() {
    // Serial.begin(115200);
    lcd.init();
    lcd.initializing_v(3);
    delay(2000);

    // ----- SECTION TO IMPORT PROGRAM DATA ------

    lcd.transmitInfo_v("", "");
    ctr.setBacklightMode(false);    // TODO - Need to import from sd card
    lcd.setBacklightTimer(ONE_MIN / 4); // TODO - Remove offset
} // end of setup

void loop() {
    
    lcd.backlightTimeout();

    if (navInput.readInputs()) {
        ctr.homeViewBtnPress();
    }

} // end of loop