/**
 * @file EEPROM_24xx64.cpp
 * @author Tony Samaniego (tms.ocmp@gmail.com)
 * @brief A wrapper class for connecting to an external I2C 24LC64 EEPROM.
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "EEPROM_24xx64.h"

void EEPROM_24xx64::begin() {
    Wire.begin();
}


size_t EEPROM_24xx64::eepromWrite(int address, u_int16_t offset, byte data, bool updateOnly) {
    size_t bytesWritten = 0;

    if (updateOnly) {
        byte storedData = eepromRead(address, offset);            
        if (data != storedData) {
            beginTransmission(address, offset);
            bytesWritten = write(data);
            endWriteTransmission();
        }
    } else {
        beginTransmission(address, offset);
        bytesWritten = write(data);
        endWriteTransmission();
    }
    return bytesWritten;
}


size_t EEPROM_24xx64::eepromWrite(int address, u_int16_t offset, const u_int8_t * data, size_t quantity) {
    size_t bytesWritten = 0;
    byte storedData[PAGE_BUFFER_SIZE];

    if (quantity <= PAGE_BUFFER_SIZE) {
        eepromRead(address, offset, storedData, quantity);
        for (size_t i = 0; i < quantity; i++) {
            if (storedData[i] != data[i]) {
                beginTransmission(address, offset + i);
                bytesWritten += write(data[i]);
                endWriteTransmission();
            }
        }
    } else {
        size_t divisors = quantity / PAGE_BUFFER_SIZE;
        size_t remainder = quantity % PAGE_BUFFER_SIZE;

        // looks at each section of data in chunks of PAGE_BUFFER_SIZE
        for (size_t i = 0; i < divisors; i++) {
            
            size_t baseEpOffset = (i * PAGE_BUFFER_SIZE) + offset;
            eepromRead(address, baseEpOffset, storedData, PAGE_BUFFER_SIZE);
            
            for (size_t j = 0; j < PAGE_BUFFER_SIZE; j++) {
                size_t dataOffset = i * PAGE_BUFFER_SIZE + j;
                if (storedData[j] != data[dataOffset]) {
                    beginTransmission(address, baseEpOffset + j);
                    bytesWritten += write(data[dataOffset]);
                    endWriteTransmission();
                }
            }
        }
        
        // writes the remainder of the data array
        size_t baseEpOffset = divisors * PAGE_BUFFER_SIZE + offset;
        eepromRead(address, baseEpOffset, storedData, remainder);

        for (size_t i = 0; i < remainder; i++) {
            size_t dataOffset = divisors * PAGE_BUFFER_SIZE + i; 
            if (storedData[i] != data[dataOffset]) {
                beginTransmission(address, baseEpOffset + i);
                bytesWritten += write(data[dataOffset]);
                endWriteTransmission();
            }
        }
    }
    return bytesWritten;
}


byte EEPROM_24xx64::eepromRead(int address, u_int16_t offset) {
    beginTransmission(address, offset);
    endReadTransmission();
    requestFrom(address, 1);
    return read();
}


void EEPROM_24xx64::eepromRead(int address, u_int16_t offset, uint8_t * data, size_t quantity) {
    beginTransmission(address, offset);
    endReadTransmission();
    requestFrom(address, quantity, true);
    read(data, quantity);
}


void EEPROM_24xx64::beginTransmission(int address, uint16_t offset) {
    Wire.beginTransmission(address);
    Wire.write(offset >> 8);
    Wire.write(offset & 0xFF);
}


size_t EEPROM_24xx64::write(uint8_t data) {
    return Wire.write(data);
}


size_t EEPROM_24xx64::write(const uint8_t * data, size_t quantity) {
    return Wire.write(data, quantity);
}


int EEPROM_24xx64::read() {
    return Wire.read();
}


void EEPROM_24xx64::read(uint8_t * data, size_t quantity) {
    for (size_t i = 0; i < quantity; i++) {
        data[i] = read();
    }
}


size_t EEPROM_24xx64::requestFrom(uint8_t address, size_t quantity) {
    return Wire.requestFrom(address, quantity);
}


size_t EEPROM_24xx64::requestFrom(uint8_t address, size_t quantity, bool stopBit) {
    return Wire.requestFrom(address, quantity, stopBit);
}


void EEPROM_24xx64::endReadTransmission() {
    Wire.endTransmission();
}


void EEPROM_24xx64::endWriteTransmission() {
    Wire.endTransmission();
    delay(5);
}


