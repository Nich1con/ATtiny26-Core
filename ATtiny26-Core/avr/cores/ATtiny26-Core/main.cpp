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
	
	В файле main.cpp располагается главный цикл программы.	
	The main.cpp file contains the main program loop.
*/ 
  
#include "Arduino.h"

int main() {
  wdt_disable();												// Остановка watchdog
  
#ifdef _T26CORE_MILLIS											// Если используется millis/micros
  TCCR0 = TIM0_INIT_PRSCL;										// Установить рассчитанный делитель Timer0
  TCNT0 = TCNT_RELOAD_VAL;										// Установить предзагружаемое значение Timer0
  TIMSK |= 1 << TOIE0;											// Запустить прерывание Timer0 overflow
#endif

#if (defined(_T26CORE_PWM) || defined (_T26CORE_PWMFAST))		// Если используется ШИМ или быстрый ШИМ
  PLLCSR |= 1 << PLLE;											// Принудительный запуск PLL
  while(!PLLCSR & (1 << PLOCK));  								// Ожидание PLL	
  PLLCSR |= 1 << PCKE;											// Подаем 64 МГц на Timer1
  TCCR1A = 1 << PWM1A | 1 << PWM1B;								// Запускаем ШИМ на каналах A & B
  TCCR1B = TIM1_INIT_PRSCL;										// Загружаем рассчитанный делитель Timer1
  OCR1A = OCR1B = 0x00;											// Сброс заполнения ШИМ
  OCR1C = 0xFF;													// Устанавливаем разрешение ШИМ 8 бит (0-255)
#endif

#if (defined(_T26CORE_ADC) || defined (_T26CORE_ADCFAST))		// Если используется АЦП
  ADCSR = 1 << ADEN | ADC_INIT_PRSCL;							// Запускаем АЦП и загружаем рассчитанный делитель
#endif

  sei();														// Разрешение прерываний
  setup();														// Функция setup()	
  while (1) {													// Бесконечный цикл
    loop();														// Функция loop()	
  } return 0;
}
