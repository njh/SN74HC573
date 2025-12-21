/*
  SN74HC573 Basic Example
  Connections:
  D0-D7 -> Arduino pins 2-9
  LE    -> pin 10
  OE    -> pin 11 (connect to GND if not used)
*/
#include <SN74HC573.h>

// Используем правильное имя класса - SN74HC573
uint8_t dataPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
SN74HC573 latch(dataPins, 10, 11);

void setup() {
  latch.begin();
  Serial.begin(9600);
  Serial.println("SN74HC573 Basic Example Started");
}

void loop() {
  // Binary counter
  for (int i = 0; i < 256; i++) {
    latch.write(i);
    Serial.print("Output: ");
    Serial.println(i, BIN);
    delay(100);
  }
}
