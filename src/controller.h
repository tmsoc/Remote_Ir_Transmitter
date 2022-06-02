
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <WiFi101.h>

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>

#include "IR_util.h"
#include "Ir_lcd.h"
#include "NavButtons.h"

class Controller {
public:
    /**
     * Construct a new Controller object
     * 
     * @param view The view to be controlled
     * @param input Inputs that the controller responds to
     * @param navArray The input array that contains the stored input values
     */
    Controller(Ir_lcd &view, NavButtons &input, bool navArray[]);

    // void loadSavedIrMessages() const;

    /**
     * Sets the backlight mode to always on or on a timer.
     * 
     * @param alwayOn The default backlight settings
     */
    void setBacklightMode(bool alwayOn);

    /**
     * The action event when the view is either in the home view or within
     * the top level settings view.
     */
    void homeViewBtnPress();

    /**
     * Decodes the given UDP message and performs the required action.
     * 
     * @param buffer An array of bytes containing the payload of the UDP message
     * @param bufferLen The number of bytes within the buffer array.
     */
    void decodeUdpPacket(byte buffer[], u_int16_t bufferLen);

private:

    void settingsLoop();

    // button presses within the main view or settings views
    // void mainViewBtnPress();
    bool settingsViewBtnPress();

    void subSettingChoice();
    void backlightSettingsConfig();
    void ipAddressDisplay();
    void assignIrControl();
    bool selectDevice(uint8_t &dev);
    bool selectFunction(uint8_t &func);
    bool receiveIrSignal(uint8_t &prot, uint16_t &addr, uint8_t &command);
    bool saveAllSetting();

    // Network functions
    bool verifyUdpHeader(byte buffer[], u_int16_t bufferLen);
    void decodeUdpRemote(byte buffer[], u_int16_t bufferLen);

    Ir_lcd *view;
    NavButtons *btn;
    bool *btnArray;
    bool backlightSetting;
    uint8_t currentView;
    // IRData *functions;
};

#endif /* CONTROLLER_H */