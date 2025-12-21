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
  _invertOutputs = false;
  _outputsEnabled = true;
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
  _invertOutputs = false;
  _outputsEnabled = true;
}

// Инициализация
void SN74HC573::begin() {
  // Настройка пинов данных как OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(_dataPins[i], OUTPUT);
    digitalWrite(_dataPins[i], LOW);
  }
  
  // Настройка управляющих пинов
  pinMode(_lePin, OUTPUT);
  digitalWrite(_lePin, LOW); // По умолчанию защелка закрыта
  
  if (_oePin != 255) { // Если пин OE указан
    pinMode(_oePin, OUTPUT);
    digitalWrite(_oePin, LOW); // Выходы включены
    _outputsEnabled = true;
  }
  
  // Инициализация нулевым значением
  write(0x00);
}

// Внутренний метод записи в пины
void SN74HC573::_writeToPins(uint8_t value) {
  uint8_t outputValue = _invertOutputs ? ~value : value;
  
  for (int i = 0; i < 8; i++) {
    digitalWrite(_dataPins[i], (outputValue >> i) & 0x01);
  }
}

// Обновление защелки
void SN74HC573::_updateLatch() {
  // Положительный импульс на LE для защелкивания данных
  digitalWrite(_lePin, HIGH);
  delayMicroseconds(1); // Минимальная длительность импульса
  digitalWrite(_lePin, LOW);
}

// Запись байта
void SN74HC573::write(uint8_t value) {
  _currentValue = value;
  
  if (_mode == MODE_TRANSPARENT) {
    // В прозрачном режиме сразу выводим на пины
    _writeToPins(value);
  } else if (_mode == MODE_NORMAL) {
    // В обычном режиме обновляем защелку
    _writeToPins(value);
    _updateLatch();
  }
  // В режиме защелки нужно отдельно вызывать latch()
}

// Защелкивание данных (для MODE_LATCHED)
void SN74HC573::latch() {
  if (_mode == MODE_LATCHED) {
    _writeToPins(_currentValue);
    _updateLatch();
  }
}

// Запись отдельного бита
void SN74HC573::write(uint8_t pin, bool state) {
  if (pin > 7) return;
  
  if (state) {
    _currentValue |= (1 << pin);
  } else {
    _currentValue &= ~(1 << pin);
  }
  
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) {
      _updateLatch();
    }
  }
}

// Установка бита в 1
void SN74HC573::set(uint8_t pin) {
  write(pin, HIGH);
}

// Сброс бита в 0
void SN74HC573::clear(uint8_t pin) {
  write(pin, LOW);
}

// Переключение бита
void SN74HC573::toggle(uint8_t pin) {
  if (pin > 7) return;
  
  _currentValue ^= (1 << pin);
  
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) {
      _updateLatch();
    }
  }
}

// Чтение бита из буфера
bool SN74HC573::read(uint8_t pin) {
  if (pin > 7) return false;
  return (_currentValue >> pin) & 0x01;
}

// Запись всего порта
void SN74HC573::writePort(uint8_t portValue) {
  write(portValue);
}

// Чтение всего порта
uint8_t SN74HC573::readPort() {
  return _currentValue;
}

// Включение выходов
void SN74HC573::enable() {
  if (_oePin != 255) {
    digitalWrite(_oePin, LOW);
    _outputsEnabled = true;
  }
}

// Отключение выходов (высокоимпедансное состояние)
void SN74HC573::disable() {
  if (_oePin != 255) {
    digitalWrite(_oePin, HIGH);
    _outputsEnabled = false;
  }
}

// Установка режима работы
void SN74HC573::setMode(uint8_t mode) {
  _mode = mode;
  if (mode == MODE_TRANSPARENT) {
    // В прозрачном режиме держим LE высоким
    digitalWrite(_lePin, HIGH);
  } else {
    digitalWrite(_lePin, LOW);
  }
}

// Установка инверсии выходов
void SN74HC573::setInversion(bool invert) {
  _invertOutputs = invert;
  // Перезаписываем текущее значение с учетом инверсии
  _writeToPins(_currentValue);
  if (_mode == MODE_NORMAL) {
    _updateLatch();
  }
}

// Генерация импульса
void SN74HC573::pulse(uint8_t value, uint16_t duration) {
  uint8_t oldValue = _currentValue;
  write(value);
  delay(duration);
  write(oldValue);
}

// Сдвиг влево
void SN74HC573::shiftLeft(bool fillZero) {
  _currentValue <<= 1;
  if (!fillZero) {
    _currentValue |= 0x01; // Заполнение единицей
  }
  
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) {
      _updateLatch();
    }
  }
}

// Сдвиг вправо
void SN74HC573::shiftRight(bool fillZero) {
  _currentValue >>= 1;
  if (!fillZero) {
    _currentValue |= 0x80; // Заполнение единицей
  }
  
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) {
      _updateLatch();
    }
  }
}

// Циклический сдвиг влево
void SN74HC573::rotateLeft() {
  uint8_t bit7 = (_currentValue & 0x80) ? 1 : 0;
  _currentValue <<= 1;
  _currentValue |= bit7;
  
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) {
      _updateLatch();
    }
  }
}

// Циклический сдвиг вправо
void SN74HC573::rotateRight() {
  uint8_t bit0 = (_currentValue & 0x01) ? 1 : 0;
  _currentValue >>= 1;
  _currentValue |= (bit0 << 7);
  
  if (_mode != MODE_LATCHED) {
    _writeToPins(_currentValue);
    if (_mode == MODE_NORMAL) {
      _updateLatch();
    }
  }
}
