/*
  SN74HC573 Shift Operations Example
  Демонстрация сдвиговых и циклических операций
*/

#include <SN74HC573.h>

// Создаем объект защелки
SN74HC573 latch(2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  Serial.println("SN74HC573 Shift Operations Example");
  
  latch.begin();
  latch.setMode(SN74HC573::MODE_NORMAL);
  
  // Начальное значение
  latch.write(0b00001111);
}

void printBinary(uint8_t value) {
  Serial.print("0b");
  for (int i = 7; i >= 0; i--) {
    Serial.print((value >> i) & 1 ? "1" : "0");
  }
  Serial.print(" (");
  Serial.print(value);
  Serial.print(")");
  Serial.println();
}

void loop() {
  Serial.println("\n=== Сдвиг влево (заполнение 0) ===");
  latch.write(0b00000001);
  
  for (int i = 0; i < 8; i++) {
    printBinary(latch.readPort());
    latch.shiftLeft(true);  // true = заполнять нулями
    delay(300);
  }
  
  delay(1000);
  
  Serial.println("\n=== Сдвиг влево (заполнение 1) ===");
  latch.write(0b10000000);
  
  for (int i = 0; i < 8; i++) {
    printBinary(latch.readPort());
    latch.shiftLeft(false);  // false = заполнять единицами
    delay(300);
  }
  
  delay(1000);
  
  Serial.println("\n=== Сдвиг вправо (заполнение 0) ===");
  latch.write(0b10000000);
  
  for (int i = 0; i < 8; i++) {
    printBinary(latch.readPort());
    latch.shiftRight(true);  // true = заполнять нулями
    delay(300);
  }
  
  delay(1000);
  
  Serial.println("\n=== Сдвиг вправо (заполнение 1) ===");
  latch.write(0b00000001);
  
  for (int i = 0; i < 8; i++) {
    printBinary(latch.readPort());
    latch.shiftRight(false);  // false = заполнять единицами
    delay(300);
  }
  
  delay(1000);
  
  Serial.println("\n=== Циклический сдвиг влево ===");
  latch.write(0b00001111);
  
  for (int i = 0; i < 16; i++) {
    printBinary(latch.readPort());
    latch.rotateLeft();
    delay(300);
  }
  
  delay(1000);
  
  Serial.println("\n=== Циклический сдвиг вправо ===");
  latch.write(0b11110000);
  
  for (int i = 0; i < 16; i++) {
    printBinary(latch.readPort());
    latch.rotateRight();
    delay(300);
  }
  
  delay(1000);
  
  Serial.println("\n=== Сложный паттерн: бегущие огни ===");
  latch.write(0b00010100);
  
  for (int i = 0; i < 32; i++) {
    printBinary(latch.readPort());
    
    if (i % 4 == 0) {
      latch.rotateLeft();
    } else {
      latch.shiftLeft((i % 2 == 0));
    }
    
    delay(200);
  }
  
  delay(2000);
}
