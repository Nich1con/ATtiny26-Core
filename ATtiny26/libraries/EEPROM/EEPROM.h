#pragma once

#include <Arduino.h>
#include <avr/eeprom.h>

class EEClass {
  public:
    uint16_t length(void) {
      return 128;
    }

    uint8_t read(uint16_t address) {
	  return eeprom_read_byte((uint8_t*)address);
    }

    void write(uint16_t address, uint8_t data) {
      eeprom_write_byte ((uint8_t*)address, data);
    }

    void update(uint16_t address, uint8_t data) {
      eeprom_update_byte ((uint8_t*)address, data);
    }

    template <typename T> T &get(uint16_t address, const T &data) {
      uint8_t *_ptr = (uint8_t*) &data;
      for (uint16_t i = 0; i < sizeof(T); i++) *_ptr++ = read(address++); 
    }

    template <typename T> const T &put(uint16_t address, const T &data) {
      const uint8_t *_ptr = (const uint8_t*) &data;
      for (uint16_t i = 0; i < sizeof(T); i++) update(address++, *_ptr++);
    }
};

static EEClass EEPROM;