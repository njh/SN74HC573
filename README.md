# Библиотека SN74HC573

![Версия](https://img.shields.io/badge/версия-1.0.0-blue)
![Arduino](https://img.shields.io/badge/Arduino-Библиотека-success)
![Лицензия](https://img.shields.io/badge/лицензия-MIT-green)

Arduino библиотека для управления 8-битными защелками SN74HC573AN/74HC573A. Предоставляет удобный интерфейс для расширения выходов Arduino, управления светодиодными матрицами, 7-сегментными индикаторами и другими устройствами.

## 📦 Возможности

- **Управление отдельными битами** - Установка, сброс, переключение конкретных выводов
- **Сдвиговые операции** - Сдвиг влево/вправо, циклический сдвиг с настраиваемым заполнением
- **Несколько режимов работы** - Прозрачный, защелкивающийся и нормальный режимы
- **Управление выходами** - Включение/отключение выходов (высокоимпедансное состояние)
- **Оптимизация** - Быстрые операции для платформ AVR
- **Поддержка LED матриц** - Встроенные методы для управления матрицами
- **7-сегментные индикаторы** - Вспомогательные функции для дисплеев

## 🚀 Установка

### Через Менеджер библиотек Arduino (рекомендуется)
1. Откройте **Arduino IDE**
2. Перейдите **Скетч → Подключить библиотеку → Управлять библиотеками...**
3. Найдите **"SN74HC573"**
4. Нажмите **"Установить"**

### Ручная установка
```bash
# Клонируйте репозиторий
git clone https://github.com/KITTEN2008/74HC573A.git

# Или скачайте ZIP с GitHub Releases
# Затем в Arduino IDE: Скетч → Подключить библиотеку → Добавить .ZIP библиотеку
```

### Использование PlatformIO
Добавьте в ваш `platformio.ini`:
```ini
lib_deps = 
    https://github.com/KITTEN2008/74HC573A.git
```

## ⚡ Быстрый старт

```cpp
#include <SN74HC573.h>

// Использование массива пинов
uint8_t dataPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
SN74HC573 latch(dataPins, 10, 11);  // LE=10, OE=11

void setup() {
  latch.begin();
  latch.setMode(SN74HC573::MODE_NORMAL);
}

void loop() {
  // Запись двоичного паттерна
  latch.write(0b10101010);
  delay(500);
  
  // Управление отдельными битами
  latch.set(0);      // Установить бит 0 в HIGH
  latch.clear(7);    // Установить бит 7 в LOW
  latch.toggle(3);   // Переключить бит 3
  
  // Сдвиговые операции
  latch.shiftLeft();
  delay(200);
}
```

## 🔌 Подключение выводов

### Базовое подключение
```
Arduino    SN74HC573     Описание
--------   ---------     -----------
2-9        D0-D7        Пины данных (любые цифровые пины)
10         LE (пин 11)  Разрешение защелки (активный HIGH)
11         OE (пин 1)   Разрешение выхода (активный LOW, опционально)
GND        GND (пин 20) Земля
5V         VCC (пин 10) Питание (2-6В, работает с 5В)
```

### Для работы со светодиодами/дисплеями
```
SN74HC573    Внешняя цепь
---------    ---------------
Q0-Q7        → Светодиоды с резисторами 220Ω
              ИЛИ сегменты 7-сегментного индикатора
              ИЛИ базы транзисторов для больших нагрузок
OE           → ШИМ пин для регулировки яркости (опционально)
```

## 📚 Справка по API

### Конструкторы
```cpp
// Конструктор с массивом
SN74HC573 latch(uint8_t dataPins[8], uint8_t lePin, uint8_t oePin);

// Конструктор с отдельными пинами
SN74HC573 latch(uint8_t d0, d1, d2, d3, d4, d5, d6, d7, lePin, oePin);
```

### Основные методы
| Метод | Описание | Пример |
|--------|-------------|---------|
| `begin()` | Инициализация пинов | `latch.begin()` |
| `write(value)` | Запись 8-битного значения | `latch.write(0xFF)` |
| `write(pin, state)` | Запись отдельного бита | `latch.write(3, HIGH)` |
| `latch()` | Защелкивание данных на выходах | `latch.latch()` |
| `set(pin)` | Установка бита в HIGH | `latch.set(0)` |
| `clear(pin)` | Установка бита в LOW | `latch.clear(7)` |
| `toggle(pin)` | Переключение состояния бита | `latch.toggle(4)` |
| `read(pin)` | Чтение бита из буфера | `bool state = latch.read(2)` |

### Расширенные операции
```cpp
// Сдвиговые операции
latch.shiftLeft();           // Сдвиг влево, заполнение 0
latch.shiftRight(true);      // Сдвиг вправо, заполнение 1
latch.rotateLeft();          // Циклический сдвиг влево
latch.rotateRight();         // Циклический сдвиг вправо

// Генерация импульсов
latch.pulse(0xFF, 100);      // Генерация импульса 100мс

// Управление выходами
latch.enable();              // Включить выходы (OE = LOW)
latch.disable();             // Отключить выходы (высокий импеданс)

// Выбор режима работы
latch.setMode(SN74HC573::MODE_NORMAL);
latch.setMode(SN74HC573::MODE_TRANSPARENT);
latch.setMode(SN74HC573::MODE_LATCHED);
```

## 🛠️ Команды для разработки

### Тестирование и проверка
```bash
# Установить Arduino CLI (если не установлен)
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Инициализировать Arduino CLI
arduino-cli config init

# Обновить индексы ядер и библиотек
arduino-cli core update-index
arduino-cli lib update-index

# Проверить структуру библиотеки
arduino-cli lib lint --library .

# Проверить компиляцию для Arduino Uno
arduino-cli compile --fqbn arduino:avr:uno examples/Basic

# Проверить компиляцию для ESP32
arduino-cli compile --fqbn esp32:esp32:esp32 examples/Basic

# Установить библиотеку локально для тестирования
arduino-cli lib install --git-url https://github.com/KITTEN2008/74HC573A.git

# Показать установленные библиотеки
arduino-cli lib list

# Обновить библиотеку до последней версии
arduino-cli lib upgrade SN74HC573
```

### Git команды для разработки библиотеки
```bash
# Клонировать репозиторий
git clone https://github.com/KITTEN2008/74HC573A.git
cd 74HC573A

# Создать новую ветку для функции
git checkout -b feature/new-method

# После внесения изменений
git add .
git commit -m "Добавлен новый метод генерации импульсов"

# Отправить на GitHub
git push origin feature/new-method

# Создать тег новой версии
git tag v1.1.0
git push origin v1.1.0
```

### Команды для публикации библиотеки
```bash
# Перед публикацией: выполнить полную проверку
arduino-cli cache clean
arduino-cli lib update-index
arduino-cli lib lint --library . --verbose

# Проверить синтаксис library.properties
cat library.properties

# Ожидаемый вывод:
# name=SN74HC573
# version=1.0.0
# architectures=*
# ...

# Проверить компиляцию примеров
for example in examples/*/; do
  echo "Тестируем $example"
  arduino-cli compile --fqbn arduino:avr:uno "$example"
done

# Создать пакет для распространения (для ручной раздачи)
zip -r SN74HC573.zip . -x "*.git*" "*.vscode*" "*.DS_Store"
```

## 📁 Структура проекта
```
74HC573A/
├── src/                    # Исходные файлы
│   ├── SN74HC573.h        # Основной заголовочный файл
│   └── SN74HC573.cpp      # Реализация
├── examples/              # Примеры скетчей
│   ├── Basic/            # Базовое использование
│   ├── LEDMatrix/        # Управление матрицей 8x8
│   ├── SevenSegment/     # Драйвер дисплея
│   └── ShiftRegister/    # Каскадные защелки
├── extras/               # Дополнительные материалы
│   ├── schematics/       # Схемы подключения
│   └── docs/             # Документация
├── library.properties    # Метаданные библиотеки
├── keywords.txt         # Подсветка синтаксиса в Arduino IDE
├── LICENSE              # Лицензия MIT
└── README.md           # Этот файл
```

## 🎯 Включенные примеры

### 1. **Basic** - Простой двоичный счетчик
```cpp
// Двоичный счетчик 0-255
for (int i = 0; i < 256; i++) {
  latch.write(i);
  delay(100);
}
```

### 2. **LED Matrix** - Управление матрицей 8x8
```cpp
// Управление строками и столбцами для мультиплексирования
SN74HC573 rowLatch(rowPins, rowLE, rowOE);
SN74HC573 colLatch(colPins, colLE, colOE);
// ... код сканирования матрицы
```

### 3. **7-Segment Display** - Многоразрядный индикатор
```cpp
// Отображение чисел на 7-сегментном индикаторе
const uint8_t digits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D};
latch.write(digits[number]);
```

### 4. **Shift Register Chain** - Каскадные защелки
```cpp
// Управление несколькими микросхемами
SN74HC573 latch1(pins1, le1, oe1);
SN74HC573 latch2(pins2, le2, oe2);
// ... синхронное управление
```

## 🚨 Диагностика проблем

### Библиотека не устанавливается
```bash
# Очистить кэш Arduino
arduino-cli cache clean

# Полностью переустановить библиотеку
arduino-cli lib uninstall SN74HC573
arduino-cli lib install SN74HC573
```

### Ошибки компиляции
```bash
# Проверить зависимости
arduino-cli lib dependencies SN74HC573

# Показать информацию о библиотеке
arduino-cli lib detail SN74HC573
```

### Проверка установленной версии
```bash
# В Arduino CLI
arduino-cli lib list | grep SN74HC573

# В скетче Arduino
#ifdef SN74HC573_h
  #pragma message "Библиотека SN74HC573 найдена"
#endif
```

## 🔄 Обновление библиотеки

Для обновления библиотеки пользователям:
```bash
# Обновить все библиотеки
arduino-cli lib upgrade

# Или конкретную библиотеку
arduino-cli lib upgrade SN74HC573
```

Для разработчика (выпуск новой версии):
1. Изменить версию в `library.properties`
2. Создать тег на GitHub: `git tag v1.1.0`
3. Создать новый релиз с тегом

## 📄 Лицензия

Эта библиотека распространяется под лицензией MIT. Подробнее см. файл [LICENSE](LICENSE).

## 🤝 Вклад в разработку

1. Форкните репозиторий
2. Создайте ветку для новой функции
3. Внесите изменения
4. Протестируйте с `arduino-cli lib lint`
5. Создайте Pull Request

## 📞 Поддержка и обратная связь

- **Issues**: [Сообщить о проблеме](https://github.com/KITTEN2008/74HC573A/issues)
- **Обсуждение**: [Arduino Forum](https://forum.arduino.cc/)
- **Автор**: KITTEN2008

---

⭐ **Если библиотека полезна, поставьте звезду на GitHub!** ⭐
