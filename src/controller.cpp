#include <Arduino.h>

#include "Controller.h"

Controller::Controller(Ir_lcd &lcd, NavButtons &input, bool navArray[], IR_Util::IRFunction functions[]) : 
backlightSetting(false), currentView(LCD_MAIN_VIEW)
{
    view = &lcd;
    btn = &input;
    btnArray = navArray;
    func = functions;
}

void Controller::setBacklightMode(bool alwaysOn) {
    backlightSetting = alwaysOn;
    view->setBacklightMode(alwaysOn);
}

void Controller::homeViewBtnPress() {
    view->setBacklightTimer(ONE_MIN / 4); // TODO - Remove offset    
    // if the up and down button are both pressed, enter the settings view
    if (btnArray[btn->UP] == HIGH && btnArray[btn->DOWN] == HIGH) {
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


void Controller::importStoredData() {
    // Eeprom eeprom;   // TODO - Remove later
    for (size_t i = 0; i < DEV_CNT * FUNC_CNT; i++) {
        byte buffer[FUNCTION_SIZE];
        size_t offset = i * FUNCTION_SIZE;
        EEPROM.eepromRead(EEPROM_ADDRESS, offset, buffer, FUNCTION_SIZE);
        memcpy(&func[i], buffer, FUNCTION_SIZE);
        
        // For troubleshooting
        // Serial.print(i);
        // Serial.print(") ");
        // Serial.print(func[i].assigned, HEX);
        // Serial.print(" ");
        // Serial.print(func[i].protocol, HEX);
        // Serial.print(" ");
        // Serial.print(func[i].address, HEX);
        // Serial.print(" ");
        // Serial.println(func[i].command, HEX);        
    }
    backlightSetting = EEPROM.eepromRead(EEPROM_ADDRESS, BACKLIGHT_EEPROM_OFFSET);
    view->setBacklightMode(backlightSetting);
}


void Controller::exportLocalData() const {
    for (size_t i = 0; i < DEV_CNT * FUNC_CNT; i++) {
        byte buffer[FUNCTION_SIZE];
        size_t offset = i * FUNCTION_SIZE;
        memcpy(buffer, &func[i], FUNCTION_SIZE);
        EEPROM.eepromWrite(EEPROM_ADDRESS, offset, buffer, FUNCTION_SIZE);
    }
}


// ------ PRIVATE FUNCTIONS ------


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


void Controller::subProgrammerChoice(uint8_t action, uint8_t device, uint8_t function) {
    switch (action)
    {
    case ASSIGN_FUNCTION:
        assignIrSignal(device, function);
        break;
    case DELETE_FUNCTION:
        deleteIrSignal(device, function);
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
    EEPROM.eepromWrite(EEPROM_ADDRESS, BACKLIGHT_EEPROM_OFFSET, byte(backlightSetting));
    delay(THREE_SEC);
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
            if (func[device * FUNC_CNT + function].assigned) {
                view->transmitInfo_v(IR_Util::DEVICES[device], IR_Util::IR_FUNC[function]);
                transmitIRFunction(func[device * FUNC_CNT + function]);
            }
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
            // if the return value if true (Changes were made)
            //      save the settings to EEPROM
            //      continueLoop = false;
            // else
            //      set the functionSelected back to false to reselect a function
            
            IR_Util::IRFunction f = func[device * FUNC_CNT + function];
            if (f.assigned) {
                decode_type_t prot = (decode_type_t) f.protocol;                
                view->irSignalInfo_v(getProtocolString(prot), f.address, f.command);
            }
            else {
                view->irSignalInfo_v("Unassigned", 0, 0);
            }
            delay(THREE_SEC);

            selectFunctionAction(device, function);
            functionSelected = false;
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



void Controller::selectFunctionAction(uint8_t device, uint8_t function) {
    uint8_t currentAction = ASSIGN_FUNCTION;
    view->programActionSelect_v(IR_Util::PROGRAM_OPTION_LIST[currentAction]);
    bool continueLoop = true;
    while (continueLoop) {
        if (btn->readInputs()) {
            if (btnArray[btn->LEFT] == HIGH) {
                continueLoop = false;
            }
            else if (btnArray[btn->RIGHT] == HIGH) {
                subProgrammerChoice(currentAction, device, function);
                view->programActionSelect_v(IR_Util::PROGRAM_OPTION_LIST[currentAction]);
                continueLoop = false;
            }
            else if (btnArray[btn->UP] ==  HIGH) {
                currentAction = (currentAction + 1) % PROGRAM_OPTION_CNT;
                view->programActionSelect_v(IR_Util::PROGRAM_OPTION_LIST[currentAction]);
            }
            else {
                currentAction = (currentAction == 0) ? PROGRAM_OPTION_CNT - 1 : --currentAction;
                view->programActionSelect_v(IR_Util::PROGRAM_OPTION_LIST[currentAction]);
            }
        }
    }
}


void Controller::assignIrSignal(uint8_t device, uint8_t function) {
    view->irSignalInfo_v("", 0x0, 0x0);
    IR_Util::IRFunction readSignal;
    readSignal.assigned = false;
    IrReceiver.resume();
    bool saveReadSignal = false;
    bool continueLoop = true;
    while (continueLoop) {
        if (IrReceiver.decode()) {
            IRData *signal = IrReceiver.read();
            if (signal->protocol != UNKNOWN) {
                readSignal.assigned = true;
                readSignal.protocol = (uint8_t) signal->protocol;
                readSignal.address = signal->address;
                readSignal.command = signal->command;
                view->irSignalInfo_v(getProtocolString(signal->protocol), signal->address, signal->command);
            }
            IrReceiver.resume();
        }

        if (btn->readInputs()) {
            if (btnArray[btn->LEFT] == HIGH) {
                continueLoop = false;
            }
            else if (btnArray[btn->RIGHT] == HIGH && readSignal.assigned) {
                saveReadSignal = saveAllSetting();
                continueLoop = false;
            }
        }
    }

    if (saveReadSignal) {
        // Serial.println("\nBefore Assignment");
        // IR_Util::IRFunction test_1[DEV_CNT * FUNC_CNT];
        // for (size_t i = 0; i < DEV_CNT * FUNC_CNT; i++) {
        //     byte buffer[FUNCTION_SIZE];
        //     size_t offset = i * FUNCTION_SIZE;
        //     EEPROM.eepromRead(EEPROM_ADDRESS, offset, buffer, FUNCTION_SIZE);
        //     memcpy(&test_1[i], buffer, FUNCTION_SIZE);
            
        //     // For troubleshooting
        //     Serial.print(i);
        //     Serial.print(") ");
        //     Serial.print(test_1[i].assigned, HEX);
        //     Serial.print(" ");
        //     Serial.print(test_1[i].protocol, HEX);
        //     Serial.print(" ");
        //     Serial.print(test_1[i].address, HEX);
        //     Serial.print(" ");
        //     Serial.println(test_1[i].command, HEX);        
        // }

        func[device * FUNC_CNT + function].assigned = readSignal.assigned;
        func[device * FUNC_CNT + function].protocol = readSignal.protocol;
        func[device * FUNC_CNT + function].address = readSignal.address;
        func[device * FUNC_CNT + function].command = readSignal.command;
        
        // Serial.println(func[device * FUNC_CNT + function].assigned, HEX);
        // Serial.println(func[device * FUNC_CNT + function].protocol, HEX);
        // Serial.println(func[device * FUNC_CNT + function].address, HEX);
        // Serial.println(func[device * FUNC_CNT + function].command, HEX);

        view->saving_v();
        exportLocalData();
        delay(THREE_SEC);

        // Serial.println("\nAfter Assignment");
        // for (size_t i = 0; i < DEV_CNT * FUNC_CNT; i++) {
        //     byte buffer[FUNCTION_SIZE];
        //     size_t offset = i * FUNCTION_SIZE;
        //     EEPROM.eepromRead(EEPROM_ADDRESS, offset, buffer, FUNCTION_SIZE);
        //     memcpy(&test_1[i], buffer, FUNCTION_SIZE);
            
        //     // For troubleshooting
        //     Serial.print(i);
        //     Serial.print(") ");
        //     Serial.print(test_1[i].assigned, HEX);
        //     Serial.print(" ");
        //     Serial.print(test_1[i].protocol, HEX);
        //     Serial.print(" ");
        //     Serial.print(test_1[i].address, HEX);
        //     Serial.print(" ");
        //     Serial.println(test_1[i].command, HEX);        
        // }
    }
}


void Controller::deleteIrSignal(uint8_t device, uint8_t function) {

    IR_Util::IRFunction f = func[device * FUNC_CNT + function];

    if (f.assigned) {
        view->irSignalInfo_v(getProtocolString((decode_type_t) f.protocol), f.address, f.command);
    }
    else {
        view->irSignalInfo_v("Unassigned", 0, 0);
        delay(THREE_SEC);
    }

    bool deleteSignal = false;
    bool continueLoop = true;
    while (continueLoop && f.assigned) {
        if (btn->readInputs()) {
            if (btnArray[btn->LEFT] == HIGH) {
                continueLoop = false;
            }
            else if (btnArray[btn->RIGHT] == HIGH) {
                deleteSignal = saveAllSetting();
                continueLoop = false;
            }
        }
    }

    if (deleteSignal) {
        func[device * FUNC_CNT + function].assigned = false;
        view->saving_v();
        exportLocalData();
        delay(THREE_SEC);
    }
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
    return saveSettings;
}


void Controller::transmitIRFunction(const IR_Util::IRFunction& function) const {
    switch (function.protocol)
    {
    case DENON:
        // Serial.println("DENON Send");
        IrSender.sendDenon(function.address, function.command, IR_REPEAT_CNT);
        break;
    case JVC:
        // Serial.println("JVC Send");
        // casts are required to specify the right function
        IrSender.sendJVC((uint8_t) function.address, (uint8_t) function.command, IR_REPEAT_CNT);
        break;
    case LG:
        // Serial.println("LG Send");
        IrSender.sendLG(function.address, function.command, IR_REPEAT_CNT);
        break;
    case NEC:
        // Serial.println("NEC Send");
        IrSender.sendNEC(function.address, function.command, IR_REPEAT_CNT);
        break;
    case PANASONIC:
        // Serial.println("Panasonic Send");
        IrSender.sendPanasonic(function.address, function.command, IR_REPEAT_CNT);
        break;
    case SAMSUNG:
        // Serial.println("SAMSUNG Send");
        IrSender.sendSamsung(function.address, function.command, IR_REPEAT_CNT);
        break;
    case SHARP:
        // Serial.println("SHARP Send");
        IrSender.sendSharp(function.address, function.command, IR_REPEAT_CNT);
        break;
    case SONY:
        // Serial.println("SONY Send");
        IrSender.sendSony(function.address, function.command, IR_REPEAT_CNT);
        break;
    default:
        // Serial.println("Send Unknown");
        break;
    }
}
