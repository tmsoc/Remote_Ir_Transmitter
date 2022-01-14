
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
        view->irSignalInfo_v("Protocal", 0x21, 0x34);
        break;
    case 1:
        view->ipInfo_v(IPAddress(192,168,1,1));
        break;
    case 2:
        view->backlightSetting_v();
        break;    
    default:
        break;
    }
    delay(3000);
}