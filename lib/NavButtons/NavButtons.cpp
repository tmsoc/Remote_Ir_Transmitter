/**
 * @file NavButtons.cpp
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief Implementation for NavButtons.h
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "NavButtons.h"

NavButtons::NavButtons(uint8_t left, uint8_t right, uint8_t up, uint8_t down, bool *pinStates) {
    pins[LEFT] = left;
    pins[RIGHT] = right;
    pins[UP] = up;
    pins[DOWN] = down;    
    states = pinStates;
    setPinModes();
    for (uint8_t i = 0; i < NAV_SIZE; i++) {
        states[i] = !digitalRead(pins[i]);
    }
}

bool NavButtons::readInputs() {
    bool stateChanged = false;
    for (uint8_t i = 0; i < NAV_SIZE; i++) {
        bool currentValue = !digitalRead(pins[i]);
        if (currentValue != states[i]) {
            states[i] = currentValue;
            if (currentValue == HIGH)
                stateChanged = true;
        }
    }
    // delay(DEBOUNCE_MS);  // if debouncing is required, uncomment and adjust delay value.
    return stateChanged;
}

void NavButtons::setPinModes() {
    for (uint8_t i = 0; i < NAV_SIZE; i++) {
        pinMode(pins[i], INPUT_PULLUP);
    }
}