
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

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

private:

    // button presses within the main view or settings views
    void mainViewBtnPress();
    void settingsViewBtnPress();

    void subSettingChoice();

    Ir_lcd *view;
    NavButtons *btn;
    bool *btnArray;
    bool backlightSetting;
    uint8_t currentView;
};

#endif /* CONTROLLER_H */