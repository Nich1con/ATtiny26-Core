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
	[D8]      -	MOSI/DI/SDA/~OC1A -  PB0 ->|o	   |<- PA0  - ADC0      - [D0, A0] 		  
	[D9]      - MISO/DO/OC1A      -  PB1 ->|       |<- PA1  - ADC1      - [D1, A1]  
	[D10]     - SCK/SCL/~OC1B     -  PB2 ->|       |<- PA2  - ADC2      - [D2, A2] 	
	[D11]     - OC1B              -  PB3 ->|       |<- PA3  - AREF      - [D3, AREF] 
	[VCC]     -	POWER			  -  VCC ->|       |<- GND  - POWER     - [GND] 
	[GND] 	  -	POWER 			  -  GND ->|       |<- AVCC - POWER     - [VCC] 
	[D12, A7] - ADC7/XTAL1        -  PB4 ->|       |<- PA4  - ADC3      - [D4, A3]  
	[D13, A8] - ADC8/XTAL2        -  PB5 ->|       |<- PA5  - ADC4      - [D5, A4] 
	[D14, A9] - ADC9/INT0/T0      -  PB6 ->|       |<- PA6  - ADC5/AIN0 - [D6, A5]
	[RST]     - ADC10/~RESET      -  PB7 ->|_______|<- PA7  - ADC6/AIN1 - [D7, A6]
    -------------------------------------------------------------------------------- 
	
	Этот файл содержит все необходимые Arduino-определения и функции.	
	This file contains all the necessary Arduino definitions and functions.
*/


#ifndef Arduino_h
#define Arduino_h

/* Подключение библиотек */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

/* Дополнительные определения ядра, работают ТОЛЬКО в этом ядре */
/* Для измерения смещения АЦП в диф. режиме усилением x20 */
#define ADC_OFFSET_X20   	0b10110
/* Для измерения в диф. режиме с усилением x1 */
#define ADC_A0_A1     		0b01100
#define ADC_A2_A1     		0b01111
#define ADC_A2_A3     		0b10000
#define ADC_A4_A3     		0b10011
#define ADC_A4_A5     		0b10101
#define ADC_A6_A5     		0b11000
#define ADC_A8_A9     		0b11010
#define ADC_A10_A9    		0b11101
/* Для измерения в диф. режиме с усилением x20 */
#define ADC_A0_A1_X20   	0b01011
#define ADC_A2_A1_X20   	0b01110
#define ADC_A4_A3_X20   	0b10010
#define ADC_A4_A5_X20   	0b10100
#define ADC_A6_A5_X20   	0b10111
#define ADC_A8_A9_X20   	0b11001
#define ADC_A10_A9_X20  	0b11100
/* Для измерения смещения нуля и напряжения VCC */
#define ADC_INTERNAL1V18  	0b11110
#define ADC_GND           	0b11111
/* Для изменения скорости работы АЦП */
#define ADC_PRESCALER_128 	0b111
#define ADC_PRESCALER_64  	0b110
#define ADC_PRESCALER_32  	0b101
#define ADC_PRESCALER_16  	0b100
#define ADC_PRESCALER_8   	0b011
#define ADC_PRESCALER_4   	0b010
#define ADC_PRESCALER_2   	0b001

/* Выбор частото-зависимых констант, в зависимости от выбранной частоты CPU */
/* Подбираем оптимальные параметы для АЦП, millis()/micros()/delay(), ШИМ */
#if (F_CPU == 16000000L)
#define ADC_PRSCL       ADC_PRESCALER_128 
#define TIM0_INIT_PRSCL 0b011 // 64	-> (16MHz / 64)	= 250 kHz	
#define TCNT_RELOAD_VAL 5	  // 5 ->  (250 kHz / (255 - 5)) = OVF 1kHz, 4us / tick  
#define MICROS_SHIFT	2 	  // 4us / tick -> (TCNT << 2) ~ (TCNT * 4)

#elif (F_CPU == 8000000L)
#define ADC_PRSCL       ADC_PRESCALER_64
#define TIM0_INIT_PRSCL 0b011 // 64	-> (8MHz / 64)	= 125 kHz	
#define TCNT_RELOAD_VAL 130	  // 130 -> (125 kHz / (255 - 130)) = OVF 1kHz, 8us / tick
#define MICROS_SHIFT	3 	  // 8us / tick -> (TCNT << 3) ~ (TCNT * 8)

#elif (F_CPU == 4000000L)
#define ADC_PRSCL       ADC_PRESCALER_32
#define TIM0_INIT_PRSCL 0b011 // 64	-> (4MHz / 64)	= 62,5 kHz	
#define TCNT_RELOAD_VAL 193	  // 193 -> (62,5 kHz / (255 - 193)) = OVF 1kHz, 16us / tick
#define MICROS_SHIFT	4 	  // 16us / tick -> (TCNT << 4) ~ (TCNT * 16)

#elif (F_CPU == 2000000L)
#define ADC_PRSCL       ADC_PRESCALER_16
#define TIM0_INIT_PRSCL 0b010 // 8	-> (2MHz / 8)	= 250 kHz	
#define TCNT_RELOAD_VAL 5	  // 5 ->  (250 kHz / (255 - 5)) = OVF 1kHz, 4us / tick
#define MICROS_SHIFT	2 	  // 4us / tick -> (TCNT << 2) ~ (TCNT * 4)

#elif (F_CPU == 1000000L)
#define ADC_PRSCL       ADC_PRESCALER_8
#define TIM0_INIT_PRSCL 0b010 // 8	-> (1MHz / 8)	= 125 kHz	
#define TCNT_RELOAD_VAL 130	  // 130 ->  (125 kHz / (255 - 130)) = OVF 1kHz, 8us / tick
#define MICROS_SHIFT	3 	  // 8us / tick -> (TCNT << 3) ~ (TCNT * 8)

#else
#error "Unsupported F_CPU"	 
#endif


#if (defined (_T26CORE_PWMFAST))
#define TIM1_INIT_PRSCL 0b0100
#elif (defined (_T26CORE_PWM))
#define TIM1_INIT_PRSCL 0b1001
#endif

#if (defined (_T26CORE_ADCFAST))
#define ADC_INIT_PRSCL (ADC_PRSCL - 2)
#elif (defined (_T26CORE_ADC))
#define ADC_INIT_PRSCL (ADC_PRSCL)
#endif 

/* Проверка на поддержку оптимизации кода компилятором (LTO) */
#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 40902
#error Update your Arduino IDE or choose a more recent compiler
#endif

/* Специальные определения устройства (Arduino - совместимые) */
#define NUM_DIGITAL_PINS           	14
#define NUM_ANALOG_INPUTS          	9
#define EXTERNAL_NUM_INTERRUPTS    	1
#define digitalPinToInterrupt(p)   	(((p) == 14) ? 0 : NOT_AN_INTERRUPT)
#define analogInputToDigitalPin(p) 	(p < 3 ? p : (p < 7 ? p + 1 : (p <= 9 ? p + 5 : -1)))
#define analogPinToChannel(p)      	(p)
#define digitalPinHasPWM(p)        	(((p) == 9) || ((p) == 11))
#define digitalPinToPort(p)      	(p < 8 ? 1 : 2)
#define digitalPinToBitMask(p)   	(p < 8 ? (1 << p) : (1 << (p - 7)))
#define digitalPinToTimer(p)     	(((p) == 9) ? TIMER1A : (((p) == 11) ? TIMER1B : (NOT_ON_TIMER)))
#define analogInPinToBit(p)      	(analogInputToDigitalPin(p) < 8 ? analogInputToDigitalPin(p) : analogInputToDigitalPin(p) - 7)
#define portOutputRegister(p)    	(p == 1 ? &PORTA : &PORTB)
#define portInputRegister(p)     	(p == 1 ? &PINA : &PINB)
#define portModeRegister(p)      	(p == 1 ? &DDRA : &DDRB)

/* Определения аналоговых пинов A0-A9 */
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define A8 8
#define A9 9

/* Определения аналоговых пинов A0-A9 */
#define PIN_PA0 0
#define PIN_PA1 1
#define PIN_PA2 2
#define PIN_PA3 3
#define PIN_PA4 4
#define PIN_PA5 5
#define PIN_PA6 6
#define PIN_PA7 7
#define PIN_PB0 8
#define PIN_PB1 9
#define PIN_PB2 10
#define PIN_PB3 11
#define PIN_PB4 12
#define PIN_PB5 13
#define PIN_PB6 14

/* Определения пинов SPI */
#define MOSI 8
#define MISO 9
#define SCK  11


#ifdef __cplusplus
extern "C" {
#endif

/* Функция yield, вызывается в каждом delay() */
static void __empty() {}
void yield(void) __attribute__ ((weak, alias("__empty")));

/* Специальные типы данных */
typedef uint16_t word;
typedef uint8_t byte;
typedef bool boolean;

/* Системные функции */
__attribute__((section(".init9")))
int main();
void setup(void);
void loop(void);

/* Функции для работы с GPIO */
void analogWriteMode(uint8_t mode);
void analogPrescaler(uint8_t presc);
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t state);
void analogWrite(uint8_t pin, uint8_t val);
void analogWriteDisable(uint8_t pin);
void analogReference(uint8_t mode);
uint8_t digitalRead(uint8_t pin);
int16_t analogRead(uint8_t pin);

/* Функции времени */
#ifdef _T26CORE_MILLIS
uint32_t millis(void);
uint32_t micros(void);
#endif
void delay(uint16_t ms);
void delayMicroseconds(uint16_t us);

/* Дополнительные функции */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void attachInterrupt(uint8_t interruptNum, void (*handler)(void), uint8_t mode);
void detachInterrupt(uint8_t interruptNum);

/* Стандартные arduino-определения */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#ifndef abs
#define abs(x)     __builtin_abs(x)
#endif

#define DEFAULT      0
#define EXTERNAL     1
#define INTERNAL     2
#define INTERNAL2V56 2

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define FALSE 0
#define TRUE 1
#define PI 3.1416
#define HALF_PI 1.57
#define TWO_PI 6.283
#define DEG_TO_RAD 0.0174533
#define RAD_TO_DEG 57.2958
#define SERIAL 0
#define DISPLAY 1
#define LSBFIRST 0
#define MSBFIRST 1
#define CHANGE 1
#define FALLING 2
#define RISING 3
#define DEFAULT 0
#define NOT_AN_INTERRUPT -1
#define NOT_ON_TIMER 0
#define NOT_A_PIN 0
#define TIMER0A 1
#define TIMER0B 2

#define sq(x)        ({ typeof (x) _x = (x); _x * _x; })
#define min(a,b)     ({ typeof (a) _a = (a); typeof (b) _b = (b); _a < _b ? _a : _b; })
#define max(a,b)     ({ typeof (a) _a = (a); typeof (b) _b = (b); _a > _b ? _a : _b; })
#define round(x)     ({ typeof (x) _x = (x); _x >= 0 ? (long)(_x + 0.5) : (long)(_x - 0.5); })
#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define constrain(x,low,high)    ({ \
    typeof (x) _x = (x);              \
    typeof (low) _l = (low);          \
    typeof (high) _h = (high);        \
    _x < _l ? _l : _x > _h ? _h :_x; })
#define interrupts() sei()
#define noInterrupts() cli()
#define clockCyclesPerMicrosecond()  ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define microsecondsToClockCycles(a) ( ((a) * (F_CPU / 1000L)) / 1000L )
#define lowByte(w)  ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitRead(value, bit)  (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)   ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))
#ifdef __cplusplus
}
#endif
#endif

#ifdef __cplusplus
  /* Дополнительные функции */
  uint16_t makeWord(uint16_t w);
  uint16_t makeWord(uint8_t h, uint8_t l);
  int32_t random(int32_t howbig);
  int32_t random(int32_t howsmall, int32_t howbig);
  void randomSeed(uint16_t seed);
  long map(long, long, long, long, long);
  
#endif
