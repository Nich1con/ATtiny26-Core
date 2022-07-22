#include <SoftwareUart.h>

/*
	Пример программной эмуляции UART в режиме отправки через пин D0
  Программный UART поддерживает частоту CPU 2,4,8,16 MHz
  Скорость передачи данных фиксированная - 9600 бод
*/

#define TX_PIN  0
SoftwareUart <TX_PIN> dbg;

void setup() {

}

void loop() {
  dbg.printStr("lol keeek : ");	// Выводим текст
  dbg.printInt(-14631);			    // Выводим число int
  dbg.write('\n');				      // Выводим символ (перевод строки)
  delay(10);                    // Ждем
}
