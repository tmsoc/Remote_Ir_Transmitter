/**
 * @file NavButtons.h
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief Simple navigation input library for arduino
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 * This class is used for simple up, down, left, & right input readings from 
 * digital inputs on an arduino. 
 * 
 * The NavButtons class requires a given boolean array of size 4 to store the 
 * recored the input values to. Every time the readInputs() function is called, 
 * the current input states are copied into the array. The bool return value
 * when calling the array can be used to to see if the falling edge of the button
 * press has occurred. 
 * 
 * The digital inputs use the internal pull-up resistor resulting in an active 
 * low operation. 
 * 
*/

#ifndef NAV_BUTTONS_H
#define NAV_BUTTONS_H

#include <Arduino.h>

#define NAV_SIZE 4
#define DEBOUNCE_MS 10

class NavButtons {

public:
    /**
     * Construct a new Nav Buttons object
     * 
     * @param left The left input pin
     * @param right The right input pin
     * @param up The up input pin
     * @param down The down input pin
     * @param pinStates boolean array of size 4
     */
    NavButtons(uint8_t left, uint8_t right, uint8_t up, uint8_t down, bool *pinStates);

    /**
     * Reads the states of each input and stores their values to the 
     * given boolean array. If there is a change in state of an input 
     * pin from HIGH to LOW, a boolean true is returned. 
     * 
     * @return true a button has been pressed
     * @return false no buttons have been pressed
     */
    bool readInputs();

    /**
     * Constants used to identify what index is used for within the states array
     */
    const uint8_t LEFT = 0; 
    const uint8_t RIGHT = 1;
    const uint8_t UP = 2;
    const uint8_t DOWN = 3;    

private:
    
    void setPinModes();

    uint8_t pins[NAV_SIZE];
    bool *states;
};


#endif