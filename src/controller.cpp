#include <Arduino.h>

#include "Controller.h"

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
    // if (currentView == LCD_MAIN_VIEW) {
    //     mainViewBtnPress();
    // }
    // else {
    //     settingsViewBtnPress();
    // }
    
    // if the up and down button are both pressed, enter the settings view
    if (btnArray[btn->UP] == HIGH && btnArray[btn->DOWN] == HIGH) {
        // currentView = PROGRAMMER_VIEW;
        // view->setup_v(IR_Util::MENU_LIST[currentView]);
        // view->setBacklightMode(true);
        settingsLoop();
        view->setBacklightTimer(ONE_MIN / 4); // TODO - remove offset
    }
}

void Controller::decodeUdpPacket(byte buffer[], u_int16_t bufferLen) {
    if (verifyUdpHeader(buffer, bufferLen)) {
        switch (buffer[MESSAGE_TYPE_INDEX])
        {
        case REMOTE_MESSAGE_TYPE:
            decodeUdpRemote(buffer, bufferLen);
            break;
        case STATUS_MESSAGE_TYPE:
            // TODO - Future Implementation
            break;
        case CONFIG_MESSAGE_TYPE:
            // TODO - Future Implementation
            break;
        default:
            break;
        }   
    }
}

// ------ PRIVATE FUNCTIONS ------

// void Controller::mainViewBtnPress() {
//     // if the up and down button are both pressed, enter the settings view
//     if (btnArray[btn->UP] == HIGH && btnArray[btn->DOWN] == HIGH) {
//         // currentView = PROGRAMMER_VIEW;
//         // view->setup_v(IR_Util::MENU_LIST[currentView]);
//         // view->setBacklightMode(true);
//         settingsLoop();
//         view->setBacklightTimer(ONE_MIN / 4); // TODO - remove offset
//     }
// }

void Controller::settingsLoop() {
    currentView = PROGRAMMER_VIEW;
    view->setup_v(IR_Util::MENU_LIST[currentView]);
    view->setBacklightMode(true);
    bool continueLoop = true;
    while (continueLoop) {
        if (btn->readInputs()) {
            continueLoop = !settingsViewBtnPress();
        }
    }
}

bool Controller::settingsViewBtnPress() {
    bool exitSettings = false;
    if (btnArray[btn->LEFT] == HIGH) {
        currentView = LCD_MAIN_VIEW;
        view->transmitInfo_v("", "");
        view->setBacklightMode(backlightSetting);
        exitSettings = true;
    }
    else if (btnArray[btn->UP] == HIGH || btnArray[btn->DOWN] == HIGH) {
        if (btnArray[btn->UP] == HIGH)
            currentView = (currentView + 1) % MENU_CNT;
        else
            currentView = (currentView == 0) ? MENU_CNT - 1 : --currentView;
        view->setup_v(IR_Util::MENU_LIST[currentView]);
    }
    else if (btnArray[btn->RIGHT]) {
        subSettingChoice();
        view->setup_v(IR_Util::MENU_LIST[currentView]);
    }
    return exitSettings;
}

void Controller::subSettingChoice() {
    switch (currentView) {
    // get new IR code setting 
    case PROGRAMMER_VIEW: 
        assignIrControl();
        break;
    case NETWORK_VIEW:
        ipAddressDisplay();
        break;
    case BACKLIGHT_VIEW:
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

void Controller::ipAddressDisplay() {
    IPAddress ip = WiFi.localIP();
    view->ipInfo_v(ip);
    bool continueLoop = true;
    while (continueLoop) {
        if (btn->readInputs()) {
            if (btnArray[btn->LEFT]) 
                continueLoop = false;
        }
    }
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
    view->deviceSelect_v(IR_Util::DEVICES[dev]);
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
                view->deviceSelect_v(IR_Util::DEVICES[dev]);
            }
            else {
                dev = (dev == 0) ? DEV_CNT - 1 : --dev;
                view->deviceSelect_v(IR_Util::DEVICES[dev]);
            }
        }
    }
    return selectionMade;
}

bool Controller::selectFunction(uint8_t &func) {
    view->funcSelect_v(IR_Util::IR_FUNC[func]);
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
                view->funcSelect_v(IR_Util::IR_FUNC[func]);
            }
            else {
                func = (func == 0) ? FUNC_CNT - 1 : --func;
                view->funcSelect_v(IR_Util::IR_FUNC[func]);
            }
        }
    }
    return selectionMade;
}

bool Controller::receiveIrSignal(uint8_t &prot, uint16_t &addr, uint8_t &command) {
    view->irSignalInfo_v("Panasonic", 0x21, 0x34);
    delay(3000);
    return true;
    // TODO - Need to implement this function.
    // Remove by reference and have the variables local within scope
    // store the values to memory
    // add a function to test the signal by pressing UP
    // if values are received return true
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
    return saveSettings;
}


bool Controller::verifyUdpHeader(byte buffer[], u_int16_t bufferLen) {
    bool validHeader = false;
    if (bufferLen > UDP_HEADER_SIZE) {
        validHeader = true;
        byte verifier = 0x01;
        for (size_t i = 0; i < UDP_HEADER_SIZE; i++) {
            if (buffer[i] != verifier) {
                validHeader = false;
            }
            verifier = verifier << 1;
        }
    }
    return validHeader;
}


void Controller::decodeUdpRemote(byte buffer[], u_int16_t bufferLen) {
    if (bufferLen == REMOTE_MESSAGE_SIZE) {
        
        byte device = buffer[MESSAGE_TYPE_INDEX + 1];
        byte function = buffer[MESSAGE_TYPE_INDEX + 2];

        if (device < DEV_CNT && function < FUNC_CNT) {
            view->transmitInfo_v(IR_Util::DEVICES[device], IR_Util::IR_FUNC[function]);
            // TODO - implement the transmitting the IR signal
        }
    }
}