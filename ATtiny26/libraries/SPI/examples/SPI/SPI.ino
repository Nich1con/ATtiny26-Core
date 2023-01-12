#include <SPI.h>

/*
	Пример работы SPI на базе модуля USI
  ATtiny26:     SPI Device:
  GND      ---- GND
  DI  [8]  ---- MISO
  DO  [9]  ---- MOSI
  SCK [10] ---- SCK
*/

void setup() {
  SPI.begin();                              // Инициализация USI в режиме Master SPI
  SPI.setDataMode(SPI_MODE0);               // Режим передачи данных MODE0 / MODE1
  uint8_t tx_data = 0b10100101;             // Байт данных на передачу
  uint8_t rx_data = SPI.transfer(tx_data);  // Передаем байт и принимаем взамен
}

void loop() {}
