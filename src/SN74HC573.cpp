#include "SN74HC573.h"

// Конструктор с массивом пинов
SN74HC573::SN74HC573(uint8_t dataPins[], uint8_t lePin, uint8_t oePin) {
  for (int i = 0; i < 8; i++) {
    _dataPins[i] = dataPins[i];
  }
  _lePin = lePin;
  _oePin = oePin;
  _currentValue = 0;
  _mode = MODE_LATCHED;
}

// Конструктор с отдельными пинами
SN74HC573::SN74HC573(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t lePin, uint8_t oePin) {
  _dataPins[0] = d0;
  _dataPins[1] = d1;
  _dataPins[2] = d2;
  _dataPins[3] = d3;
  _dataPins[4] = d4;
  _dataPins[5] = d5;
  _dataPins[6] = d6;
  _dataPins[7] = d7;
  _lePin = lePin;
  _oePin = oePin;
  _currentValue = 0;
  _mode = MODE_LATCHED;
}

void SN74HC573::begin() {
  for (int i = 0; i < 8; i++) {
    pinMode(_dataPins[i], OUTPUT);
    digitalWrite(_dataPins[i], LOW);
  }
  pinMode(_lePin, OUTPUT);
  digitalWrite(_lePin, LOW);
  
  if (_oePin != 255) {
    pinMode(_oePin, OUTPUT);
    digitalWrite(_oePin, LOW);
  }
  write(0x00);
}

void SN74HC573::_writeToPins(uint8_t value) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(_dataPins[i], (value >> i) & 0x01);
  }
}

void SN74HC573::_updateLatch() {
  digitalWrite(_lePin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_lePin, LOW);
}

void SN74HC573::write(uint8_t value) {
  _currentValue = value;
  if (_mode == MODE_TRANSPARENT) {
    _writeToPins(value);
  } else if (_mode == MODE_NORMAL) {
    _writeToPins(value);
    _updateLatch();
  }
}

void SN74HC573::latch() {
  if (_mode == MODE_LATCHED) {
    _writeToPins(_currentValue);
    _updateLatch();
  }
}

void SN74HC573::write(uint8_t pin, bool state) {
  if (pin > 7) return;
  if (state) {
    _currentValue |= (1 << pin);
  } else {
    _currentValue &= ~(1 << pin);
  }
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) _updateLatch();
  }
}

// Реализации остальных методов (set, clear, toggle, read, enable, disable, setMode)
// ... Их код можно взять из нашего предыдущего обсуждения ...
// Для краткости здесь приведена только базовая структура.
