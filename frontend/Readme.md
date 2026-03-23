# Tensor Compiler Frontend

C++ фронтенд для трансляции ONNX-моделей в вычислительный граф с возможностью визуализации через GraphViz.

## 🚀 Сборка проекта

Проект использует **vcpkg** для управления зависимостями. Для корректной настройки и связи библиотек используйте PowerShell.

### 1. Генерация файлов сборки

Запустите команду из корневой директории проекта (убедитесь, что пути к `vcpkg` соответствуют вашему локальному окружению):

#### в Windows
прописать путь к vcpkg:
```powershell
setx VCPKG_ROOT ""path_to_vcpkg"\vcpkg"
```
запустить с помощью MinGW:
```powershell
cmake -B build `
     -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg_source/vcpkg/scripts/buildsystems/vcpkg.cmake" `
     -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic `
     -DCMAKE_BUILD_TYPE=Debug `
     -G "MinGW Makefiles"
```

#### В Linux
```powershell
export VCPKG_ROOT=/путь/к/вашему/vcpkg
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### 2. Компиляция

```powershell
cmake --build build
```

---

## 🧪 Запуск и тестирование

После успешной сборки все исполняемые файлы будут находиться в директории `./build/`.

### Основная программа

Запуск парсера для конкретной ONNX модели:

```powershell
./build/TensorCompiler.exe <*.onnx>
```

### Тестирование

В проекте реализовано два уровня тестов. Вы можете запустить их напрямую:

| Тест | Команда запуска | Описание |
|------|-----------------|----------|
| Unit | `./build/unit_tests.exe` | Проверка логики отдельных узлов и структуры дерева. |
| E2E  | `./build/e2e_tests.exe`  | End-to-end проверка парсинга реальных ONNX моделей. |

---

## 📊 Визуализация

Проект поддерживает автоматическую генерацию графических схем графа. Если в вашей системе установлен GraphViz (утилита `dot` доступна в `PATH`), вы можете выполнить команду:

### Генерация PNG:

```powershell
dot -Tpng dump/<*.dot> -o dump/<*.png>
```

Результат: изображение будет сохранено в директории `frontend/dump/model3.png`.

---

## 📂 Структура проекта

```
src/tree/        — базовые структуры данных вычислительного графа
src/parse_onnx/  — логика трансляции из формата Protobuf (ONNX)
src/dump/        — экспорт графа в формат .dot для GraphViz
tests/           — модульные и интеграционные тесты
models/          — примеры моделей для тестирования
```

---

## ⚠️ Примечания для Windows (MinGW)

**Динамические библиотеки:**
Так как используется триплет `x64-mingw-dynamic`, для запуска `.exe` могут потребоваться DLL (например, `libprotobufd.dll`). Скрипт сборки в `CMakeLists.txt` настроен на их автоматическое копирование в папку `build`.

**vcpkg:**
Если ваш `vcpkg` установлен в другое место, обязательно замените путь в параметре `-DCMAKE_TOOLCHAIN_FILE`.
