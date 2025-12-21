/*
  SN74HC573 PWM Brightness Control Example
  Управление яркостью светодиодов через ШИМ на выводе OE
  
  Важно: вывод OE (Output Enable) должен быть подключен к ШИМ-пину Arduino
*/

#include <SN74HC573.h>

// Пин OE должен быть ШИМ-пином (на Uno: 3, 5, 6, 9, 10, 11)
#define OE_PWM_PIN 11

// Создаем объект защелки
// OE_PWM_PIN будет использоваться для управления яркостью
SN74HC573 latch(2, 3, 4, 5, 6, 7, 8, 9, 10, OE_PWM_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("PWM Brightness Control Example");
  
  // Инициализация защелки
  latch.begin();
  latch.setMode(SN74HC573::MODE_NORMAL);
  
  // Устанавливаем начальный паттерн
  latch.write(0b10101010);
  
  Serial.println("Управляйте яркостью:");
  Serial.println("- Нажмите 'u' для увеличения яркости");
  Serial.println("- Нажмите 'd' для уменьшения яркости");
  Serial.println("- Нажмите цифру 0-9 для установки уровня");
}

void setBrightness(uint8_t level) {
  // Инвертируем, т.к. OE активен в LOW
  // 0 = максимальная яркость (OE постоянно LOW)
  // 255 = выключено (OE постоянно HIGH)
  analogWrite(OE_PWM_PIN, 255 - level);
  
  Serial.print("Яркость установлена: ");
  Serial.print(map(level, 0, 255, 0, 100));
  Serial.println("%");
}

void breathEffect() {
  Serial.println("Эффект дыхания...");
  
  for (int i = 0; i <= 255; i += 5) {
    setBrightness(i);
    delay(30);
  }
  
  for (int i = 255; i >= 0; i -= 5) {
    setBrightness(i);
    delay(30);
  }
}

void loop() {
  // Проверка ввода с последовательного порта
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    static uint8_t brightness = 128;  // Средняя яркость по умолчанию
    
    switch (command) {
      case 'u':  // Увеличить яркость
        if (brightness > 20) brightness -= 20;
        else brightness = 0;
        setBrightness(brightness);
        break;
        
      case 'd':  // Уменьшить яркость
        if (brightness < 235) brightness += 20;
        else brightness = 255;
        setBrightness(brightness);
        break;
        
      case 'b':  // Эффект дыхания
        breathEffect();
        break;
        
      case '0' ... '9':  // Установить уровень 0-9
        brightness = map(command - '0', 0, 9, 0, 255);
        setBrightness(brightness);
        break;
        
      case 'f':  // Полная яркость
        setBrightness(0);
        brightness = 0;
        break;
        
      case 'o':  // Выключить (но данные остаются в защелке)
        setBrightness(255);
        brightness = 255;
        break;
    }
  }
  
  // Автоматическая демонстрация эффектов
  static unsigned long lastDemo = 0;
  static int demoState = 0;
  
  if (millis() - lastDemo > 5000) {
    lastDemo = millis();
    
    switch (demoState) {
      case 0:
        Serial.println("Демо: Плавное включение");
        for (int i = 255; i >= 0; i--) {
          setBrightness(i);
          delay(10);
        }
        break;
        
      case 1:
        Serial.println("Демо: Мигание");
        for (int i = 0; i < 5; i++) {
          setBrightness(0);   // Включено
          delay(200);
          setBrightness(255); // Выключено
          delay(200);
        }
        setBrightness(0);     // Оставить включенным
        break;
        
      case 2:
        breathEffect();
        break;
    }
    
    demoState = (demoState + 1) % 3;
  }
  
  // Меняем паттерн каждые 2 секунды
  static unsigned long lastPatternChange = 0;
  static uint8_t pattern = 0b10101010;
  
  if (millis() - lastPatternChange > 2000) {
    lastPatternChange = millis();
    pattern = ~pattern;  // Инвертируем паттерн
    latch.write(pattern);
    
    Serial.print("Паттерн изменен: 0b");
    for (int i = 7; i >= 0; i--) {
      Serial.print((pattern >> i) & 1 ? "1" : "0");
    }
    Serial.println();
  }
}
