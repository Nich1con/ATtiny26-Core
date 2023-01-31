#pragma once

#include <Arduino.h>
#include <util/delay.h>

/* Коррекция величины задержки бод */
#if (F_CPU  == 16000000UL)
#define SWU_DELAY 102.50f
#elif (F_CPU  == 8000000UL)
#define SWU_DELAY 105.00f
#elif (F_CPU  == 4000000UL)
#define SWU_DELAY 104.50f
#elif (F_CPU  == 2000000UL)
#define SWU_DELAY 98.50f
#elif (F_CPU  == 1000000UL)
#error "SoftwareUart is not support 1MHz CPU clock"
#endif

template <uint8_t PIN> class SoftwareUart {
  public:
    SoftwareUart() {
      pinMode(PIN, OUTPUT);
      digitalWrite(PIN, HIGH);
    }

    void write(char c) {
      digitalWrite(PIN, LOW);
      _delay_us(SWU_DELAY);
      for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(PIN, c & 0b1);
        _delay_us(SWU_DELAY), c >>= 1;
      }
      digitalWrite(PIN, HIGH);
      _delay_us(SWU_DELAY);
    }

    void printStr(char *str) {
      char* p = &str[0];
      while (*p != '\0') {
        write(*p);
        ++p;
      }
    }

    void printInt(int data) {
      uint8_t chars[10], len;
      if (data < 0) {
        write('-');
        data = -data;
      }
      for (uint8_t i = 0; i < 10; i++) {
        chars[i] = data % 10;
        data /= 10;
        if (data == 0) {
          len = i;
          break;
        }
      }
      for (int8_t i = len; i >= 0; i--) {
        write(chars[i] + '0');
      }
    }
};
