cmake_minimum_required(VERSION 3.15)
project(TensorCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 1. Кроссплатформенная настройка путей ---

# Пытаемся взять VCPKG_ROOT из системы.
# Если не задано, CMake просто оставит переменную пустой (но лучше задать её в ОС)
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(WARNING "VCPKG_ROOT environment variable is not set! This might cause build issues.")
endif()

set(VCPKG_ROOT_DIR $ENV{VCPKG_ROOT})

# Если триплет не задан (на Linux он обычно не нужен в командной строке),
# определяем его по умолчанию для поиска путей.
if(NOT VCPKG_TARGET_TRIPLET)
    if(WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")
    else()
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    endif()
endif()

# Пути к инклудам и либам теперь универсальны
set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET}")

# --- 2. Находим пакеты ---
# На Linux пакеты из vcpkg подхватываются идеально через find_package
find_package(ONNX CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# --- 3. Создание библиотеки ядра ---
add_library(compiler_core STATIC
    "src/tree/tree.cpp"
    "src/parse_onnx/parser.cpp"
    "src/dump/dump.cpp"
)

target_include_directories(compiler_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tree"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dump"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/parse_onnx"
    # Больше никакого диска D! Используем переменную:
    "${VCPKG_INSTALLED_DIR}/include"
)

# Используем цели (targets) вместо прямых путей к .a файлам.
# Это гораздо надежнее для кроссплатформенности.
# ... (начало файла без изменений)

# 1. Явно просим CMake найти файлы библиотек в папке vcpkg
find_library(LIB_ONNX NAMES onnx PATHS "${VCPKG_INSTALLED_DIR}/lib" NO_DEFAULT_PATH)
find_library(LIB_ONNX_PROTO NAMES onnx_proto PATHS "${VCPKG_INSTALLED_DIR}/lib" NO_DEFAULT_PATH)

# 2. Проверка (для отладки)
if(NOT LIB_ONNX OR NOT LIB_ONNX_PROTO)
    message(FATAL_ERROR "Could not find ONNX libraries in ${VCPKG_INSTALLED_DIR}/lib")
endif()

# ... (создание library compiler_core)

target_link_libraries(compiler_core PUBLIC
    "${LIB_ONNX}"
    "${LIB_ONNX_PROTO}"
    protobuf::libprotobuf
)

# --- 4. Исполняемые файлы ---
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE compiler_core)

# Тесты
add_executable(unit_tests tests/unit/test_node.cpp tests/unit/test_tree.cpp)
target_link_libraries(unit_tests PRIVATE compiler_core GTest::gtest GTest::gtest_main)

add_executable(e2e_tests tests/e2e/test_onnx_parsing.cpp)
target_link_libraries(e2e_tests PRIVATE compiler_core GTest::gtest GTest::gtest_main)

file(COPY tests/e2e/test_models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()
add_test(NAME Unit COMMAND unit_tests)
add_test(NAME E2E  COMMAND e2e_tests)

# --- 5. Специфичные настройки для Windows (DLL) ---
if(WIN32)
    # Линковка для MinGW
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--enable-auto-import")

    # Копирование DLL (на Linux это не нужно, там всё работает через RPATH или LD_LIBRARY_PATH)
    # Пытаемся найти, где лежит нужная DLL
    find_file(PROTOBUF_DLL
        NAMES "libprotobufd.dll" "libprotobuf.dll"
        PATHS "${VCPKG_INSTALLED_DIR}/debug/bin" "${VCPKG_INSTALLED_DIR}/bin"
        NO_DEFAULT_PATH
    )

    if(PROTOBUF_DLL)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROTOBUF_DLL}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMENT "Copying Protobuf DLL for Windows"
        )
    endif()
endif()
