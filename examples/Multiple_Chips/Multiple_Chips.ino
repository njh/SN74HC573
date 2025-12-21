/*
  SN74HC573 Multiple Chips Example
  Управление несколькими микросхемами
  
  Пример для 3 микросхем SN74HC573, подключенных к разным пинам LE
  Общие пины данных, разные пины защелки
*/

#include <SN74HC573.h>

// Общие пины данных для всех микросхем
uint8_t dataPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

// Создаем объекты для трех микросхем
SN74HC573 chip1(dataPins, 10, 255);  // LE=10, OE не используется
SN74HC573 chip2(dataPins, 11, 255);  // LE=11
SN74HC573 chip3(dataPins, 12, 255);  // LE=12

void setup() {
  Serial.begin(9600);
  Serial.println("Multiple SN74HC573 Chips Example");
  
  // Инициализируем все микросхемы
  chip1.begin();
  chip2.begin();
  chip3.begin();
  
  // Устанавливаем режим защелки
  chip1.setMode(SN74HC573::MODE_LATCHED);
  chip2.setMode(SN74HC573::MODE_LATCHED);
  chip3.setMode(SN74HC573::MODE_LATCHED);
}

void writeToAll(uint8_t value) {
  // Записываем одинаковое значение во все микросхемы
  chip1.write(value);
  chip2.write(value);
  chip3.write(value);
  
  // Защелкиваем данные одновременно
  chip1.latch();
  chip2.latch();
  chip3.latch();
}

void writeSequentially(uint8_t value1, uint8_t value2, uint8_t value3) {
  // Записываем разные значения в каждую микросхему
  chip1.write(value1);
  chip1.latch();
  delay(100);
  
  chip2.write(value2);
  chip2.latch();
  delay(100);
  
  chip3.write(value3);
  chip3.latch();
  delay(100);
}

void knightRiderEffect() {
  // Эффект "Knight Rider" на трех микросхемах
  uint8_t patterns[] = {
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010
  };
  
  for (int i = 0; i < sizeof(patterns); i++) {
    chip1.write(patterns[i]);
    chip2.write(patterns[(i + 5) % sizeof(patterns)]);
    chip3.write(patterns[(i + 10) % sizeof(patterns)]);
    
    chip1.latch();
    chip2.latch();
    chip3.latch();
    
    delay(100);
  }
}

void loop() {
  Serial.println("=== Все микросхемы вместе ===");
  
  // Все микросхемы отображают одинаковую бегущую точку
  for (int i = 0; i < 8; i++) {
    writeToAll(1 << i);
    delay(200);
  }
  
  delay(1000);
  
  Serial.println("=== Последовательная активация ===");
  
  // Активируем микросхемы по очереди
  for (int i = 0; i < 8; i++) {
    writeSequentially(1 << i, 1 << ((i + 2) % 8), 1 << ((i + 4) % 8));
  }
  
  delay(1000);
  
  Serial.println("=== Эффект Knight Rider ===");
  
  knightRiderEffect();
  
  delay(1000);
  
  Serial.println("=== Двоичный счетчик на всех чипах ===");
  
  // Синхронный двоичный счетчик
  for (int i = 0; i < 256; i++) {
    writeToAll(i);
    delay(50);
  }
  
  delay(2000);
}
