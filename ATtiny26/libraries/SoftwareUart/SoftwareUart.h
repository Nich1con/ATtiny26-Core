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
      char intBuffer[7];  //for integers, that is seven characters (five digits, a possible minus sign, and a terminating 0)
      itoa(data, intBuffer, 10);
      printStr(intBuffer);
    }
};
