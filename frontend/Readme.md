# Tensor Compiler Frontend

C++ фронтенд для трансляции ONNX-моделей в вычислительный граф
с последующей визуализацией через GraphViz.

---

## 🛠 Подготовка системы (Linux / WSL)

Для сборки установите системные пакеты и библиотеки:

```bash
sudo apt update
sudo apt install -y build-essential cmake graphviz \
                    libprotobuf-dev protobuf-compiler \
                    libonnx-dev libgtest-dev
```

**Важно для WSL/Ubuntu:**
Если после установки возникает ошибка `onnx/onnx.pb.h: No such file`, выполните:

```bash
cd /usr/include/onnx
sudo protoc --cpp_out=. onnx.proto
```

---

## 🚀 Сборка проекта

Сборка выполняется стандартными командами CMake:

```bash
# 1. Генерация файлов сборки
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Компиляция (используя все доступные ядра процессора)
cmake --build build
```

---

## 🧪 Запуск и тестирование

После сборки все бинарники находятся в директории `./build/`.

### ▶ Основная программа

```bash
./build/TensorCompiler path/to/model.onnx
```

### 🧪 Тестирование

| Тип теста | Команда | Описание |
|----------|--------|----------|
| Unit     | `./build/unit_tests` | Проверка логики отдельных компонентов |
| E2E      | `./build/e2e_tests`  | Интеграционные тесты ONNX моделей |

Запуск всех тестов:

```bash
cd build && ctest --output-on-failure
```

---

## 📊 Визуализация графа

Проект генерирует `.dot` файл. Для получения изображения:

```bash
dot -Tpng dump/graph.dot -o graph.png
```

---

## 📂 Структура проекта

```
src/     — исходный код (Core, Parser, Dump)
tests/   — модульные и интеграционные тесты
models/  — примеры .onnx моделей
dump/    — сгенерированные графы (.dot)
```

---

## ⚙️ Особенности Linux-версии

- Native Dependencies — используются системные библиотеки вместо vcpkg
- Performance — сборка с `-j$(nproc)`
- Standard Paths — бинарники без `.exe`
- Environment — совместимость с Bash/Zsh и WSL

---

## 🚧 Быстрый старт (Full Cycle)

```bash
# 1. Сборка
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 2. Запуск тестов
cd build && ctest && cd ..

# 3. Трансляция модели и генерация графа
./build/TensorCompiler models/example.onnx
dot -Tpng dump/graph.dot -o result_graph.png
```
