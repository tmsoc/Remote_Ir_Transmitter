
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <WiFi101.h>

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>

#include "IR_util.h"
#include "Ir_lcd.h"
#include "NavButtons.h"
#include "EEPROM_24xx64.h"

class Controller {
public:
    /**
     * Construct a new Controller object
     *
     * @param view The view to be controlled
     * @param input Inputs that the controller responds to
     * @param navArray The input array that contains the stored input values
     * @param functions
     */
    Controller(Ir_lcd &view, NavButtons &input, bool navArray[], IR_Util::IRFunction functions[]);

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

    /**
     * 
     * 
     */
    void importStoredData();

    /**
     * 
     * 
     */
    void exportLocalData() const;

private:

    // Menu navigation
    void settingsLoop();
    bool settingsViewBtnPress();
    void subSettingChoice();
    void subProgrammerChoice(uint8_t action, uint8_t device, uint8_t function);
    // LCD backlight configuration
    void backlightSettingsConfig();
    // Network functions
    void ipAddressDisplay();
    bool verifyUdpHeader(byte buffer[], u_int16_t bufferLen);
    void decodeUdpRemote(byte buffer[], u_int16_t bufferLen);
    // Function configurations
    void assignIrControl();
    bool selectDevice(uint8_t &dev);
    bool selectFunction(uint8_t &func);
    void selectFunctionAction(uint8_t device, uint8_t function);
    void assignIrSignal(uint8_t device, uint8_t function);
    void deleteIrSignal(uint8_t device, uint8_t function);
    // EEPROM
    bool saveAllSetting();
    // IR transmit/receive
    void transmitIRFunction(const IR_Util::IRFunction& function) const;
    // Utility Functions
    


    Ir_lcd *view;
    NavButtons *btn;
    bool *btnArray;
    bool backlightSetting;
    uint8_t currentView;
    IR_Util::IRFunction *func;
};

#endif /* CONTROLLER_H */