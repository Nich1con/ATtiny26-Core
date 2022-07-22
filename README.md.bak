# ATtiny26 core for Arduino IDE
 **Версия 1.0.0 от 22.07.2022**  
 Быстрое и легкое ядро, позволяющее программировать недорогие микроконтроллеры ATtiny26 в среде Arduino IDE.
 Включает в себя все основные wiring-функции, пресеты для AVR-программаторов, гибкие настройки и несколько полезных библиотек.
 
## Установка
### Автоматическая
- Открой Arduino IDE
- Зайди в **Файл > Настройки**
- Вставь этот адрес в **Дополнительные ссылки для менеджера плат**:
    ```
    https://nich1con.github.io//ATtiny26-Core.json
    ``` 
- Открой **Инструменты > Плата > Менеджер плат...**
- Подожди загрузку списка
- Листай в самый низ, пока не увидишь **ATtiny26-Core**
- Жми **Установка**
- Закрой окно
- Выбери плату в **Инструменты > Плата > ATtiny26-Core > ATtiny26**
- Готово!

### Ручная
- Папку ATtiny26-Core положить по пути C:\Users\Username\Documents\Arduino\hardware 
- Если папка hardware отсутствуют - необходимо создать ее.

## Раписновка ATtiny26 и доступные wiring-функции
### Используя данное ядро придерживайтесь этой распиновки:
![CORE_PINOUT](https://github.com/Nich1con/ATtiny26-Core/blob/main/ATtiny26-pinout.png)
### Цифровые wiring-функции:
- **pinMode(pin, mode)**  	 	
- **digitalRead(pin)**
- **digitalWrite(pin, state)**
- **analogWrite(pin, duty)**
- **shiftOut(dataPin, clkPin, order, data)**
- **shiftIn(dataPin, clkPin, order)**

**pin** передавать в виде числа **0...14** или в формате **PIN_PA0**, **PIN_PB3** и т.д.
---
### Аналоговые wiring-функции:
- **analogRead(pin)**
- **analogReference(reference)**

**pin** передавать только в формате **A0...A9**
---
### wiring-функции времени:
- **millis()**
- **micros()**
- **delay(ms)**
- **delayMicroseconds(us)**
---
### wiring-функции внешнего прерывания:
- **attachInterrupt(0, func, mode)**
- **detachInterrupt(0)**
---
### Так же доступны :
- **random(), map()** и другие математические функции и макросы Arduino

## Встроенные библиотеки (см. примеры в Arduino IDE)
- **EEPROM**
- **SoftwareUart**

## Доступные настройки ядра
**Clock** - выбор частоты и источника тактирования **(требует перезаписи загрузчика)**:
- **Internal 16MHz(PLL)** (Тактирование встроенным генератором с умножением частоты до 16 МГц)
- **Internal 8MHz** (Тактирование встроенным генератором с частотой 8 МГц)
- **Internal 4MHz** 
- **Internal 2MHz** 
- **Internal 1MHz** (На данной частоте не работает библиотека SoftwareUart)
- **External crystal 16MHz** (Тактирование внешним кварцем 16 МГц)
- **External crystal 8MHz** 
- **External crystal 1MHz**  (На данной частоте не работает библиотека SoftwareUart)
---
**EEPROM** - сохранять EEPROM после перепрошивки (очистки) контроллера  **(требует перезаписи загрузчика)**:
- **Retain** - сохранять
- **Erase** - очищать
---
**millis()/micros()** - компиляция функций millis() и micros() **(задействует Timer0)*:
- **Enable** - использовать
- **Disable** - не использовать (экономит flash, millis()/micros() не доступны)
---
**analogWrite()** - компиляция функции ШИМ analogWrite **(задействует Timer1)*:
- **Enable** - использовать стандартный ШИМ с частотой около ~1кГц
- **Fast mode** - высокочастотный ШИМ с частотой около ~31кГц
- **Disable** - не использовать (экономит flash, analogWrite() не доступен)
---
**analogWrite()** - компиляция функции analogRead **(задействует АЦП)*:
- **Enable** - использовать
- **Fast mode** - повышенная скорость преобразования АЦП
- **Disable** - не использовать (экономит flash, analogRead() не доступен)
---
**Compiler**
- **Embedded** - использовать встроенный компилятор Arduino IDE
- **GCC v8.3.0** - использовать более свежую версию компилятора **(могут возникать проблемы)*