#ifndef SN74HC573_h
#define SN74HC573_h

#include "Arduino.h"

class SN74HC573 {
  public:
    // Конструкторы
    SN74HC573(uint8_t dataPins[], uint8_t lePin, uint8_t oePin);
    SN74HC573(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
              uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
              uint8_t lePin, uint8_t oePin);
    
    // Основные методы
    void begin();
    void write(uint8_t value);
    void write(uint8_t pin, bool state);
    void latch();
    void enable();
    void disable();
    
    // Работа с отдельными битами
    void set(uint8_t pin);
    void clear(uint8_t pin);
    void toggle(uint8_t pin);
    bool read(uint8_t pin);
    
    // Режимы работы
    enum Mode {
      MODE_NORMAL,
      MODE_TRANSPARENT,
      MODE_LATCHED
    };
    void setMode(uint8_t mode);
    
  private:
    uint8_t _dataPins[8];
    uint8_t _lePin;
    uint8_t _oePin;
    uint8_t _currentValue;
    uint8_t _mode;
    void _writeToPins(uint8_t value);
    void _updateLatch();
};

#endif
