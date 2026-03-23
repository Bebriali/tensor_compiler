# Tensor Compiler Frontend

C++ фронтенд для трансляции ONNX-моделей в вычислительный граф  
с последующей визуализацией через GraphViz.

---

## 🛠 Подготовка системы

Для сборки в Linux используйте системный менеджер пакетов:

```bash
sudo apt update
sudo apt install -y build-essential cmake graphviz \
                    libprotobuf-dev protobuf-compiler \
                    libonnx-dev libgtest-dev
```

---

## 🚀 Сборка проекта

Сборка выполняется стандартными командами CMake:

```bash
# 1. Генерация файлов сборки
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Компиляция (используя все ядра процессора)
cmake --build build
```

---

## 🧪 Запуск и тестирование

После сборки все бинарники находятся в директории `./build/`.

### ▶ Основная программа

```bash
./build/TensorCompiler path/to/model.onnx
```

### 🧪 Тесты

| Тип теста | Команда | Описание |
|----------|--------|----------|
| Unit     | `./build/unit_tests` | Проверка логики отдельных компонентов |
| E2E      | `./build/e2e_tests`  | Интеграционные тесты ONNX |

Запуск всех тестов сразу:

```bash
cd build && ctest
```

---

## 📊 Визуализация графа

Если установлен GraphViz, можно сгенерировать изображение графа:

```bash
dot -Tpng dump/graph.dot -o graph.png
```

---

## 📂 Структура проекта

```
src/        — исходный код ядра и парсеров  
tests/      — модульные и e2e тесты  
models/     — примеры ONNX моделей  
dump/       — результаты генерации графов  
```

---

## ⚙️ Особенности Linux-версии

- Нет vcpkg — используются системные библиотеки  
- Нет .exe — бинарники без расширений  
- Нет PowerShell — используется Bash/Zsh  
- Быстрая сборка через `-j$(nproc)`  
- Зависимости устанавливаются через apt  

---

## 💡 Требования

- CMake ≥ 3.15  
- GCC / Clang с поддержкой C++17  
- Установленные библиотеки:
  - Protobuf  
  - ONNX  
  - GoogleTest  
  - GraphViz (опционально)  

---

## 🚧 Пример полного цикла

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
