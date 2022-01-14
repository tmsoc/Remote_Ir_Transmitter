
#include "controller.h"

Controller::Controller(Ir_lcd &lcd, NavButtons &input, bool navArray[]) : 
backlightSetting(false), currentView(LCD_MAIN_VIEW)
{
    view = &lcd;
    btn = &input;
    btnArray = navArray;
}

void Controller::setBacklightMode(bool alwaysOn) {
    backlightSetting = alwaysOn;
    view->setBacklightMode(alwaysOn);
}

void Controller::homeViewBtnPress() {
    view->setBacklightTimer(ONE_MIN / 4); // TODO - Remove offset
    if (currentView == LCD_MAIN_VIEW) {
        mainViewBtnPress();
    }
    else {
        settingsViewBtnPress();
    }
}

// ------ PRIVATE FUNCTIONS ------

void Controller::mainViewBtnPress() {
    // if the up and down button are both pressed, enter the settings view
    if (btnArray[btn->UP] == HIGH && btnArray[btn->DOWN] == HIGH) {
        currentView = 0;
        view->setup_v(IR_Util::menuList[currentView]);
        view->setBacklightMode(true);
    }
}

void Controller::settingsViewBtnPress() {
    if (btnArray[btn->LEFT] == HIGH) {
        currentView = LCD_MAIN_VIEW;
        view->transmitInfo_v("", "");
        view->setBacklightMode(backlightSetting);
    }
    else if (btnArray[btn->UP] == HIGH || btnArray[btn->DOWN] == HIGH) {
        if (btnArray[btn->UP] == HIGH)
            currentView = (currentView + 1) % MENU_CNT;
        else
            currentView = (currentView == 0) ? MENU_CNT - 1 : --currentView;
        view->setup_v(IR_Util::menuList[currentView]);
    }
    else if (btnArray[btn->RIGHT]) {
        subSettingChoice();
        view->setup_v(IR_Util::menuList[currentView]);
    }
}

void Controller::subSettingChoice() {
    switch (currentView) {
    // get new IR code setting 
    case 0: 
        assignIrControl();
        break;
    case 1:
        view->ipInfo_v(IPAddress(192,168,1,1));
        delay(3000);
        break;
    case 2:
        backlightSettingsConfig();
        break;    
    default:
        break;
    }
}

void Controller::backlightSettingsConfig() {
    view->backlightSetting_v();
    bool choiceMade = false;
    while (!choiceMade) {
        if (btn->readInputs()) {
            if (btnArray[btn->UP] == HIGH && btnArray[btn->DOWN] == HIGH) {
                backlightSetting = true;
                choiceMade = true;
            }
            else if (btnArray[btn->LEFT] == HIGH && btnArray[btn->RIGHT] == HIGH) {
                backlightSetting = false;
                choiceMade = true;
            }
        }
    }
    view->saving_v();
    delay(3000);
    // TODO - Need to save settings to SD card
}

void Controller::assignIrControl() {
    uint8_t device = 0;
    uint8_t function = 0;
    // bool assignmentMade = false;
    bool deviceSelected = false;
    bool functionSelected = false;
    bool continueLoop = true;
    while (continueLoop) {
        if (!deviceSelected) {
            // select a device
            // if return value is false
            //      exit loop back to main settings menu
            deviceSelected = selectDevice(device);
            continueLoop = deviceSelected;
        }
        if (!functionSelected && deviceSelected) {
            // select a function
            // if return value is false (user pressed left)
            //      set deviceSelected back to false to reselect a new device
            // if return value if true (user pressed right)
            functionSelected = selectFunction(function);
            deviceSelected = functionSelected;
        }
        if (deviceSelected && functionSelected) {
            // get the new ir command
            // if the return value if false
            //      set the functionSelected back to false to reselect a function
            // else
            //      save the settings to the sd card
            //      continueLoop = false;
            uint8_t prot = 0;
            uint16_t address = 0;
            uint8_t command = 0;
            bool signalReceived = false;
            signalReceived = receiveIrSignal(prot, address, command);

            if (!signalReceived) {
                functionSelected = signalReceived;
            }
            else {
                saveAllSetting();
                continueLoop = false;
            }
        }
    }
}

bool Controller::selectDevice(uint8_t &dev) {
    view->deviceSelect_v(IR_Util::devices[dev]);
    bool selectionMade = false;
    bool continueLoop = true;
    while (continueLoop) {
        if (btn->readInputs()) {
            if (btnArray[btn->LEFT] == HIGH) {
                continueLoop = false;
            }
            else if (btnArray[btn->RIGHT] == HIGH) {
                selectionMade = true;
                continueLoop = false;
            }
            else if (btnArray[btn->UP] == HIGH) {
                dev = (dev + 1) % DEV_CNT;
                view->deviceSelect_v(IR_Util::devices[dev]);
            }
            else {
                dev = (dev == 0) ? DEV_CNT - 1 : --dev;
                view->deviceSelect_v(IR_Util::devices[dev]);
            }
        }
    }
    return selectionMade;
}

bool Controller::selectFunction(uint8_t &func) {
    view->funcSelect_v(IR_Util::ir_func[func]);
    bool selectionMade = false;
    bool continueLoop = true;
    while (continueLoop) {
        if (btn->readInputs()) {
            if (btnArray[btn->LEFT] == HIGH) {
                continueLoop = false;
            }
            else if (btnArray[btn->RIGHT] == HIGH) {
                selectionMade = true;
                continueLoop = false;
            }
            else if (btnArray[btn->UP] == HIGH) {
                func = (func + 1) % FUNC_CNT;
                view->funcSelect_v(IR_Util::ir_func[func]);
            }
            else {
                func = (func == 0) ? FUNC_CNT - 1 : --func;
                view->funcSelect_v(IR_Util::ir_func[func]);
            }
        }
    }
    return selectionMade;
}

bool Controller::receiveIrSignal(uint8_t &prot, uint16_t &addr, uint8_t &command) {
    view->irSignalInfo_v("Panisonic", 0x21, 0x34);
    delay(3000);
    return true;
    // TODO - Need to implement this function.
}

bool Controller::saveAllSetting() {
    view->saveChange_v();
    bool saveSettings = false;
    bool choiceMade = false;
    while (!choiceMade) {
        if (btn->readInputs()) {
            if (btnArray[btn->UP] == HIGH && btnArray[btn->DOWN] == HIGH) {
                saveSettings = true;
                choiceMade = true;
            }
            else if (btnArray[btn->LEFT] == HIGH && btnArray[btn->RIGHT] == HIGH) {
                choiceMade = true;
            }
        }
    }
    if (saveSettings) {
        view->saving_v();
        delay(3000);
        // TODO - save all setting to sd card
    }
}