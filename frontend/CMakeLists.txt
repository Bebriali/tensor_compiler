cmake_minimum_required(VERSION 3.15)
project(TensorCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Поиск системных зависимостей ---
# В Linux эти пакеты устанавливаются через sudo apt install
find_package(Protobuf REQUIRED)
find_package(GTest REQUIRED)

# ONNX часто ставится как системная библиотека без конфигурационных файлов CMake,
# поэтому ищем её через поиск путей к заголовочным файлам и библиотекам.
find_path(ONNX_INCLUDE_DIR NAMES onnx/onnx_pb.h)
find_library(LIB_ONNX NAMES onnx)
find_library(LIB_ONNX_PROTO NAMES onnx_proto)

if(NOT LIB_ONNX OR NOT LIB_ONNX_PROTO)
    message(FATAL_ERROR "ONNX libraries not found! Install libonnx-dev")
endif()

# --- 2. Создание библиотеки ядра ---
add_library(compiler_core STATIC
    "src/tree/tree.cpp"
    "src/parse_onnx/parser.cpp"
    "src/dump/dump.cpp"
)

target_include_directories(compiler_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tree"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dump"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/parse_onnx"
    ${ONNX_INCLUDE_DIR}
)

target_link_libraries(compiler_core PUBLIC
    ${LIB_ONNX}
    ${LIB_ONNX_PROTO}
    protobuf::libprotobuf
)

# --- 3. Исполняемые файлы ---
# Основная программа
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE compiler_core)

# Тесты
add_executable(unit_tests tests/unit/test_node.cpp tests/unit/test_tree.cpp)
target_link_libraries(unit_tests PRIVATE compiler_core GTest::gtest GTest::gtest_main)

add_executable(e2e_tests
    tests/e2e/test_onnx_parsing.cpp
    tests/e2e/test_generate_and_parse.cpp
)
target_link_libraries(e2e_tests PRIVATE compiler_core GTest::gtest GTest::gtest_main)

# Копируем модели для тестов в папку сборки
file(COPY tests/e2e/test_models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()
add_test(NAME Unit COMMAND unit_tests)
add_test(NAME E2E  COMMAND e2e_tests)
