/**
 * @file EEPROM_24xx64.h
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief A wrapper class for connecting to an external I2C 24LC64 EEPROM.
 * @version 0.1
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EEPROM_24XX64
#define EEPROM_24XX64

#define PAGE_BUFFER_SIZE 32

#include <Arduino.h>
#include <Wire.h>

class EEPROM_24xx64 {
public:

    void begin();
    
    size_t eepromWrite(int address, u_int16_t offset, byte data, bool updateOnly=true);
    size_t eepromWrite(int address, u_int16_t offset, const u_int8_t * data, size_t quantity);

    byte eepromRead(int address, u_int16_t offset);
    void eepromRead(int address, u_int16_t offset, uint8_t * data, size_t quantity); 

private:

    void beginTransmission(int address, u_int16_t offset);
    
    size_t write(uint8_t data);
    size_t write(const uint8_t * data, size_t quantity);

    int read();
    void read(uint8_t * data, size_t quantity);

    size_t requestFrom(uint8_t address, size_t quantity);
    size_t requestFrom(uint8_t address, size_t quantity, bool stopBit);

    void endReadTransmission();
    void endWriteTransmission();
};

extern EEPROM_24xx64 EEPROM;

#endif // EEPROM_24XX64