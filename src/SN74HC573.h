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
    
    // Работа с портами
    void writePort(uint8_t portValue);
    uint8_t readPort();
    
    // Режимы работы
    enum Mode {
      MODE_NORMAL,
      MODE_TRANSPARENT,
      MODE_LATCHED
    };
    
    void setMode(uint8_t mode);
    void setInversion(bool invert);
    
    // Расширенные функции
    void pulse(uint8_t value, uint16_t duration);
    void shiftLeft(bool fillZero = true);
    void shiftRight(bool fillZero = true);
    void rotateLeft();
    void rotateRight();
    
    // Битовые маски
    enum {
      BIT0 = 0x01,
      BIT1 = 0x02,
      BIT2 = 0x04,
      BIT3 = 0x08,
      BIT4 = 0x10,
      BIT5 = 0x20,
      BIT6 = 0x40,
      BIT7 = 0x80,
      ALL_BITS = 0xFF
    };
    
  private:
    uint8_t _dataPins[8];
    uint8_t _lePin;
    uint8_t _oePin;
    uint8_t _currentValue;
    uint8_t _mode;
    bool _invertOutputs;
    bool _outputsEnabled;
    
    void _writeToPins(uint8_t value);
    void _updateLatch();
};

#endif
