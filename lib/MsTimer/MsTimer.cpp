/**
 * @file MsTimer.cpp
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief Simple timer class
 * @version 1.0
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "MsTimer.h"

MsTimer::MsTimer() {
    clearTimer();
}

void MsTimer::setTimer(unsigned int ms) {
    setTime = millis();
    triggerTime = setTime + ms;
}

bool MsTimer::getStatus() const {
    // gets the current time ms time
    unsigned long current = millis();
    // tests if the timer has not been set
    if (setTime == triggerTime)
        return false;
    // tests if time has been reached or expired 
    // accounts for millis() overflow
    if (setTime < triggerTime && triggerTime <= current)
        return false;
    if (setTime < triggerTime && current < setTime)
        return false;
    if (setTime > triggerTime && current > triggerTime && current < setTime)
        return false;
    // The timer is still active
    return true;    
}

void MsTimer::clearTimer() {
    setTime = 0;
    triggerTime = 0;
}
