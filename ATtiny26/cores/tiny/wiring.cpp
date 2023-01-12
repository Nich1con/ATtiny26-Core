/*
   ATtiny26-Core это Arduino-ядро для работы с недорогим микроконтроллером ATtiny26/ATtiny26L.
   Это бесплатное ядро, подразумевает свободное использование и модификацию.
   Просьба ссылаться на первоисточник - github.com/Nich1con/ATtiny26-Core
   
   ATtiny26-Core is an Arduino core for working with an inexpensive ATtiny26/ATtiny26L microcontroller.
   This is a free core, implies free use and modification.
   Please refer to the original source - github.com/Nich1con/ATtiny26-Core
   
   ATtiny26 pinout:		 	 
	--------------------------------------------------------------------------------
	[CORE]	  - FUNCTION	      -  GPIO	_______    GPIO - FUNCTION  - [CORE]
	[D8]      - MOSI/DI/SDA/~OC1A -  PB0 ->|o      |<- PA0  - ADC0      - [D0, A0] 		  
	[D9]      - MISO/DO/OC1A      -  PB1 ->|       |<- PA1  - ADC1      - [D1, A1]  
	[D10]     - SCK/SCL/~OC1B     -  PB2 ->|       |<- PA2  - ADC2      - [D2, A2] 	
	[D11]     - OC1B              -  PB3 ->|       |<- PA3  - AREF      - [D3, AREF] 
	[VCC]     - POWER	      -  VCC ->|       |<- GND  - POWER     - [GND] 
	[GND] 	  - POWER 	      -  GND ->|       |<- AVCC - POWER     - [VCC] 
	[D12, A7] - ADC7/XTAL1        -  PB4 ->|       |<- PA4  - ADC3      - [D4, A3]  
	[D13, A8] - ADC8/XTAL2        -  PB5 ->|       |<- PA5  - ADC4      - [D5, A4] 
	[D14, A9] - ADC9/INT0/T0      -  PB6 ->|       |<- PA6  - ADC5/AIN0 - [D6, A5]
	[RST]     - ADC10/~RESET      -  PB7 ->|_______|<- PA7  - ADC6/AIN1 - [D7, A6]
    -------------------------------------------------------------------------------- 
	
	В файле wiring.cpp располагаются все Arduino функции, реализованные в этом ядре.	
	The wiring.cpp file contains all the Arduino functions implemented in this core.
*/ 

#include <Arduino.h>

volatile uint32_t _millis_ovf_cnt = 0; 
uint8_t analog_reference = DEFAULT;
extern void (*ext_isr)();
void (*ext_isr)();

/* Функции времени - millis(), micros(), delay(), delayMicroseconds() */
#ifdef _T26CORE_MILLIS
ISR(TIMER0_OVF0_vect) {		
  TCNT0 = TCNT_RELOAD_VAL;  
  _millis_ovf_cnt++;		
}

uint32_t millis(void){     
   return _millis_ovf_cnt;  
}

uint32_t micros(void){      // micros() = ((millis() * 1000) + COUNT * US PER TICK)
   return (_millis_ovf_cnt * 1000UL) + ((TCNT0 - TCNT_RELOAD_VAL) << MICROS_SHIFT);
}
#endif

void delay(uint16_t ms) {	
#ifdef _T26CORE_MILLIS
  uint32_t start = millis();
  while (millis() - start <= ms) {
    yield();
  }
#else
  while (--ms) {
    _delay_ms(1);
  }
#endif
}

void delayMicroseconds(uint16_t us) {
  while (--us) {
    _delay_us(1);
  }
}

/* Цифровые функции работы с GPIO */
void pinMode(uint8_t pin, uint8_t mode) {
  switch (mode) {
    case INPUT:
      if (pin > 7) {
        bitClear(PORTB, pin - 8);
        bitClear(DDRB, pin - 8);
      } else {
        bitClear(PORTA, pin);
        bitClear(DDRA, pin);
      } return;
    case OUTPUT:
      if (pin > 7) {
        bitSet(DDRB, pin - 8);
      } else {
        bitSet(DDRA, pin);
      } return;
    case INPUT_PULLUP:
      if (pin > 7) {
        bitSet(PORTB, pin - 8);
        bitClear(DDRB, pin - 8);
      } else {
        bitSet(PORTA, pin);
        bitClear(DDRA, pin);
      } return;
  }
}

void digitalWrite(uint8_t pin, uint8_t state){
#if (defined(_T26CORE_PWM) || defined (_T26CORE_PWMFAST))
	analogWriteDisable(pin);
#endif
	if(pin > 7) bitWrite(PORTB, pin - 8, state);
	else bitWrite(PORTA, pin, state);
}

uint8_t digitalRead(uint8_t pin){
	if(pin > 7) return bitRead(PINB, pin - 8);
	return bitRead(PINA, pin);	
}

/* Функции генерации ШИМ  */
#if (defined(_T26CORE_PWM) || defined (_T26CORE_PWMFAST))
void analogWriteDisable(uint8_t pin) {
  switch (pin) {
    case 9: bitClear(TCCR1A, COM1A1);
    case 11: bitClear(TCCR1A, COM1B1);
  }
}

void analogWrite(uint8_t pin, uint8_t val) {
  switch (val) {
    case 0x00:
      digitalWrite(pin, LOW);
      return;
    case 0xFF:
      digitalWrite(pin, HIGH);
      return;
    default:
      switch (pin) {
        case 9:
          bitSet(TCCR1A, COM1A1);
          OCR1A = val;
          return;
        case 11:
          bitSet(TCCR1A, COM1B1);
          OCR1B = val;
          return;
      }
  }
}
#endif
	
/* Функции для работы с АЦП */
#if (defined(_T26CORE_ADC) || defined (_T26CORE_ADCFAST))
void analogPrescaler(uint8_t presc){
	ADCSR = (ADCSR & 0xF8) | presc;
}

void analogReference(uint8_t mode){
	analog_reference = mode;
}

int16_t analogRead(uint8_t pin){
	ADMUX = analog_reference << 6 | pin;
	ADCSR |= 1 << ADSC;
	while(ADCSR & (1 << ADSC));
	return ADC;
}
#endif

/* Функции shiftOut и shiftIn - программный SPI */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
  for (uint8_t i = 0; i < 8; i++)  {
    switch (bitOrder) {
      case LSBFIRST:
        digitalWrite(dataPin, (val & 0b00000001));
        val >>= 1; break;
      case MSBFIRST:
        digitalWrite(dataPin, (val & 0b10000000));
        val <<= 1; break;
    }
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
  uint8_t data = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(clockPin, HIGH);
    switch (bitOrder) {
      case LSBFIRST:
        data |= (digitalRead(dataPin) ? 1 : 0) << 7;
        data >>= 1; break;
      case MSBFIRST:
        data |= (digitalRead(dataPin) ? 1 : 0);
        data <<= 1; break;
    }
    digitalWrite(clockPin, LOW);
  }
  return data;
}

/* Функции внешних прерываний*/
void attachInterrupt(uint8_t interruptNum, void (*handler)(void), uint8_t mode){
	ext_isr = *handler;
	MCUCR = (MCUCR & 0b11111100) | mode;
	bitSet(GIMSK, INT0);
}

void detachInterrupt(uint8_t interruptNum){
	bitClear(GIMSK, INT0);
}

ISR(INT0_vect) {				
  (*ext_isr)();
}

/* Функции WMath */
void randomSeed(uint16_t seed) {
  if (seed != 0)
    srandom(seed);
}

int32_t random(int32_t howbig) {
  if (!howbig) return 0;
  return random() % howbig;
}

int32_t random(int32_t howsmall, int32_t howbig) {
  if (howsmall >= howbig) return howsmall;
  int32_t diff = howbig - howsmall;
  return random(diff) + howsmall;
}

uint16_t makeWord(uint16_t w) {
  return w;
}

uint16_t makeWord(uint8_t h, uint8_t l) {
  return (h << 8) | l;
}

long map(long x, long in_min, long in_max, long out_min, long out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
