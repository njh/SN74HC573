/*
  SN74HC573 7-Segment Display Example
  Управление 7-сегментным индикатором
  
  Подключение сегментов (общий катод):
  SN74HC573  7-Segment
  ---------  ---------
  Q0         A
  Q1         B
  Q2         C
  Q3         D
  Q4         E
  Q5         F
  Q6         G
  Q7         DP (десятичная точка)
*/

#include <SN74HC573.h>

// Создаем объект защелки
SN74HC573 segment(2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

// Коды цифр для 7-сегментного индикатора (общий катод)
// Порядок сегментов: A, B, C, D, E, F, G, DP
const uint8_t digits[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // b
  0b00111001, // C
  0b01011110, // d
  0b01111001, // E
  0b01110001  // F
};

// Специальные символы
const uint8_t symbols[] = {
  0b01000000, // Минус/тире
  0b00001000, // Подчеркивание
  0b01100011, // Градус °
  0b00111001, // C
  0b01111001, // E
  0b01110001, // F
  0b00000000  // Пробел
};

void setup() {
  Serial.begin(9600);
  Serial.println("7-Segment Display Example");
  
  segment.begin();
  segment.setMode(SN74HC573::MODE_NORMAL);
}

void displayNumber(int number) {
  // Ограничиваем диапазон 0-99 для простоты
  if (number < 0) number = 0;
  if (number > 99) number = 99;
  
  int tens = number / 10;
  int ones = number % 10;
  
  Serial.print("Отображаем: ");
  Serial.println(number);
  
  // Можно добавить управление несколькими индикаторами
  // через мультиплексирование
}

void displayHex(uint8_t value) {
  if (value < 16) {
    segment.write(digits[value]);
  } else {
    segment.write(symbols[6]); // Пробел для значений >15
  }
}

void loop() {
  Serial.println("=== Отображение цифр 0-9 ===");
  
  // Отображаем цифры 0-9
  for (int i = 0; i < 10; i++) {
    segment.write(digits[i]);
    Serial.print("Цифра: ");
    Serial.println(i);
    delay(500);
  }
  
  delay(1000);
  
  Serial.println("=== Отображение HEX A-F ===");
  
  // Отображаем HEX символы A-F
  for (int i = 10; i < 16; i++) {
    segment.write(digits[i]);
    Serial.print("HEX: ");
    Serial.println((char)('A' + i - 10));
    delay(500);
  }
  
  delay(1000);
  
  Serial.println("=== Бегущая точка ===");
  
  // Анимация бегущей точки
  for (int i = 0; i < 8; i++) {
    segment.write(1 << i);  // Включаем только один сегмент
    delay(200);
  }
  
  // Включаем все сегменты
  segment.write(0xFF);
  delay(1000);
  
  // Выключаем все
  segment.write(0x00);
  delay(1000);
  
  Serial.println("=== Символы ===");
  
  // Отображаем специальные символы
  segment.write(symbols[0]);  // Минус
  delay(1000);
  
  segment.write(symbols[2]);  // Градус
  delay(1000);
  
  segment.write(symbols[3]);  // C
  delay(1000);
  
  segment.write(symbols[4]);  // E
  delay(1000);
}
