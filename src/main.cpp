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
#include "EEPROM_24xx64.h"

// WiFi_Secrets.h contains the wifi ssid and password
// contains the following definitions
// #define SECRET_SSID ""
// #define SECRET_PASS ""
#include "WiFi_Secrets.h"

// IR remote stored function
IR_Util::IRFunction functions[DEV_CNT * FUNC_CNT];

// lcd display
Ir_lcd lcd(LCD_ADDRESS, LCD_COL_CNT, LCD_ROW_CNT);

// navigational buttons
bool navArray[NAV_SIZE];
NavButtons navInput(LEFT_PIN, RIGHT_PIN, UP_PIN, DOWN_PIN, navArray);

// WiFi variables
WiFiUDP udp;
byte buffer[MAX_UDP_BUFFER];
u_int16_t wifiStatus = WL_IDLE_STATUS;

// // for main view display
// uint8_t currentView = LCD_MAIN_VIEW;

// The controller
Controller ctrl(lcd, navInput, navArray, functions); 

void setup() {
    // Serial.begin(115200);
    // delay(5000);
    // Serial.println(sizeof(decode_type_t));

    // Initializes the lcd display
    lcd.init();
    lcd.initializing_v(1);
    delay(500);

    // Connecting to WiFi
    wifiStatus = WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(500);
    udp.begin(UDP_PORT);
    lcd.initializing_v(2);

    // ----- SECTION TO IMPORT PROGRAM DATA ------
    Eeprom eeprom;
    delay(5);
    eeprom.begin();
    ctrl.importStoredData();
    // ctrl.exportLocalData();
    delay(500);


    lcd.initializing_v(3);
    delay(500);


    lcd.transmitInfo_v("", "");
    // ctrl.setBacklightMode(false);       // TODO - Need to import from data
    lcd.setBacklightTimer(ONE_MIN / 4); // TODO - Remove offset
    
}   // end of setup()

void loop() {
    // backlight timer
    lcd.backlightTimeout();
    
    // checks wifi status
    if (wifiStatus != WL_CONNECTED) {
        while (wifiStatus != WL_CONNECTED) {
            lcd.wifiNotConnected_v();
            wifiStatus = WiFi.begin(SECRET_SSID, SECRET_PASS);
            delay(1000);
            if (navInput.readInputs()) {
                ctrl.homeViewBtnPress();
            }
        }
        lcd.transmitInfo_v("", "");
    }

    // checks to see if a udp message has been received.
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
        u_int16_t bufferLen = min(packetSize, MAX_UDP_BUFFER);
        udp.readBytes(buffer, bufferLen);
        ctrl.decodeUdpPacket(buffer, bufferLen);
        udp.flush();
    }

    // user has pressed a button
    if (navInput.readInputs()) {
        ctrl.homeViewBtnPress();
    }

}   // end of loop()