/*
  SN74HC573 LED Matrix Example
  Управление светодиодной матрицей 8x8
  
  Для этого примера нужны 2 микросхемы SN74HC573:
  - Первая для строк (аноды)
  - Вторая для столбцов (катоды)
*/

#include <SN74HC573.h>

// Защелка для строк (аноды)
uint8_t rowPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
SN74HC573 rowLatch(rowPins, 10, 255);  // OE не используется (255)

// Защелка для столбцов (катоды)
uint8_t colPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
SN74HC573 colLatch(colPins, 11, 255);  // OE не используется

// Паттерны для отображения
const uint8_t smiley[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

const uint8_t heart[8] = {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00000000
};

const uint8_t arrow[8] = {
  0b00011000,
  0b00111100,
  0b01111110,
  0b11111111,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000
};

void setup() {
  // Инициализация защелок
  rowLatch.begin();
  colLatch.begin();
  
  // Установка режима защелкивания
  rowLatch.setMode(SN74HC573::MODE_LATCHED);
  colLatch.setMode(SN74HC573::MODE_LATCHED);
  
  // Все столбцы выключены (для общего катода)
  colLatch.write(0xFF);
  colLatch.latch();
}

void displayPattern(const uint8_t pattern[8], int duration) {
  unsigned long startTime = millis();
  
  while (millis() - startTime < duration) {
    // Динамическая индикация: быстро перебираем строки
    for (uint8_t row = 0; row < 8; row++) {
      // Включаем текущую строку (для общего анода - инвертируем)
      colLatch.write(~(1 << row));  // Инвертируем для общего катода
      // Устанавливаем данные для строки
      rowLatch.write(pattern[row]);
      
      // Защелкиваем данные
      colLatch.latch();
      rowLatch.latch();
      
      // Короткая задержка для яркости
      delayMicroseconds(800);
      
      // Выключаем все перед следующим циклом
      colLatch.write(0xFF);
      colLatch.latch();
    }
  }
}

void loop() {
  // Отображение разных паттернов
  displayPattern(smiley, 3000);
  displayPattern(heart, 3000);
  displayPattern(arrow, 3000);
  
  // Бегущая точка
  for (int i = 0; i < 8; i++) {
    uint8_t runningDot[8] = {0};
    runningDot[i] = 0xFF;
    displayPattern(runningDot, 200);
  }
}
