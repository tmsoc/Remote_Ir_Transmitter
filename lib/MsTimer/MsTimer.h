/**
 * @file MsTimer.h
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief Simple timer class
 * @version 1.0
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef MS_TIMER_H
#define MS_TIMER_H

#include <Arduino.h>

class MsTimer {
public:
    /**
     * Construct a new Ms Timer object
     */
    MsTimer();

    /**
     * Set the Timer object. 
     * 
     * @param ms amount of milliseconds that the timer will be active
     */
    void setTimer(unsigned int ms);

    /**
     * Get the Status object
     * 
     * @return true The timer is active
     * @return false The timer is not active
     */
    bool getStatus();

    /**
     * @brief Clears the timer and sets it to not active. 
     */
    void clearTimer();

private:
    unsigned long setTime;
    unsigned long triggerTime;
};


#endif
