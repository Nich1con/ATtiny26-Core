#pragma once

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04

#include <Arduino.h>

class SPIClass {
  public:
    void begin(void) {
      USICR = 1 << USIWM0 | 1 << USICS1 | 1 << USICLK;
      bitClear(DDRB, PB0);
      bitSet(DDRB, PB1);
      bitSet(DDRB, PB2);
    }

    void setDataMode(uint8_t mode) {
      switch (mode) {
        case SPI_MODE1: bitSet(USICR, USICS0);
          break;
        default:  bitClear(USICR, USICS0);
          break;
      }
    }

    uint8_t transfer(uint8_t data) {
      USIDR = data;
      USISR = 1 << USIOIF;
      while (!(USISR & (1 << USIOIF)))
        USICR |= 1 << USITC;
      return USIDR;
    }

    void end() {
      bitClear(USICR, USIWM0);
      bitClear(USICR, USIWM1);
    }
  private:
};

static SPIClass SPI;

