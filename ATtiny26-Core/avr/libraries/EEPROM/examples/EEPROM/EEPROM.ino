#include <EEPROM.h>

/*
	Пример работы с EEPROM
*/

void setup() {
  uint32_t a = 0, b = 10;   
  a = EEPROM.read(0);       // Чтение байта из ячейки 0
  EEPROM.write(0, 127);     // Запись байта из ячейки 0
  EEPROM.update(0, 255);    // Запись байта если содержимое отличается
  EEPROM.get(0, a);         // Чтение любого типа данных
  EEPROM.put(0, b);         // Запись любого типа данных
}

void loop() {

}
