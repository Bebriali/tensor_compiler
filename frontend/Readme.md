# Tensor Compiler Frontend

C++ фронтенд для трансляции ONNX-моделей в вычислительный граф
с последующей визуализацией через GraphViz.

---

## Подготовка системы

Для сборки в Linux используйте системный менеджер пакетов:

```bash
sudo apt update
sudo apt install -y build-essential cmake graphviz \
                    libprotobuf-dev protobuf-compiler \
                    libonnx-dev libgtest-dev
```

---

## Сборка проекта

Сборка выполняется стандартными командами CMake:

```bash
# 1. Генерация файлов сборки
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Компиляция
cmake --build build
```

---

## Запуск и тестирование

### Основная программа

Запуск парсера для конкретной ONNX модели:

```powershell
./build/TensorCompiler.exe <*.onnx>
```

### Тесты

| Тип теста | Команда | Описание |
|----------|--------|----------|
| Unit     | `./build/unit_tests` | Проверка логики отдельных компонентов |
| E2E      | `./build/e2e_tests`  | Интеграционные тесты ONNX |

Запуск всех тестов сразу:

```bash
cd build && ctest
```

---

## Визуализация графа

Если установлен GraphViz, можно сгенерировать изображение графа:

```powershell
dot -Tpng dump/<*.dot> -o dump/<*.png>
```

Результат: изображение будет сохранено в директории `frontend/dump/model3.png`.

---

## Структура проекта

```
src/        — исходный код ядра и парсеров
tests/      — модульные и e2e тесты
models/     — примеры ONNX моделей
dump/       — результаты генерации графов
```

---

## Требования

- CMake ≥ 3.15
- GCC / Clang с поддержкой C++17
- Установленные библиотеки:
  - Protobuf
  - ONNX
  - GoogleTest
  - GraphViz

---

## Пример полного цикла

```bash
# Установка зависимостей
sudo apt update
sudo apt install -y build-essential cmake graphviz \
                    libprotobuf-dev protobuf-compiler \
                    libonnx-dev libgtest-dev

# Сборка
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Запуск тестов
cd build && ctest

# Запуск программы
./TensorCompiler ../models/example.onnx

# Генерация графа
dot -Tpng dump/graph.dot -o graph.png
```

---

## Примечание

Если ONNX не найден автоматически, установите:

```bash
sudo apt install libonnx-dev
```
