# SN74HC573 Library

Arduino library for SN74HC573AN 8-bit transparent latch.

## Basic Usage

```cpp
#include <SN74HC573.h> // Имя файла исправлено!

// Define pins: D0-D7, LE, OE
uint8_t pins[] = {2, 3, 4, 5, 6, 7, 8, 9};
SN74HC573 latch(pins, 10, 11); // Имя класса исправлено!

void setup() {
  latch.begin();
}

void loop() {
  latch.write(0b10101010); // Write binary pattern
  delay(500);
}
