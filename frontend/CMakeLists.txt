cmake_minimum_required(VERSION 3.15)
project(TensorCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Явно задаем папку, которую подтвердил ваш 'ls'
set(VCPKG_LIB_DIR "D:/vcpkg_source/vcpkg/installed/x64-mingw-dynamic/lib")

# 2. Указываем полные пути к найденным файлам
set(LIB_ONNX "${VCPKG_LIB_DIR}/libonnx.a")
set(LIB_ONNX_PROTO "${VCPKG_LIB_DIR}/libonnx_proto.a")

# 3. Находим пакеты для инклудов (заголовков)
find_package(ONNX CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(Gtest CONFIG REQUIRED)

# 1. Create a library for your core compiler logic
add_library(compiler_core STATIC
    "src/tree/tree.cpp"
    "src/parse_onnx/parser.cpp"
    "src/dump/dump.cpp"
)

# 2. Use 'PUBLIC' so anyone linking to this library automatically gets these includes
target_include_directories(compiler_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tree"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dump"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/parse_onnx"
    "D:/vcpkg_source/vcpkg/installed/x64-mingw-dynamic/include"
)

# 3. Link external libs TO the core library (PUBLIC so tests get them too)
target_link_libraries(compiler_core PUBLIC
    "${LIB_ONNX}"
    "${LIB_ONNX_PROTO}"
    protobuf::libprotobuf
)

# 4. Now your main executable is MUCH simpler:
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE compiler_core)

# Юнит тесты
add_executable(unit_tests
    tests/unit/test_node.cpp
    tests/unit/test_tree.cpp
)
target_link_libraries(unit_tests PRIVATE
    compiler_core
    GTest::gtest
    GTest::gtest_main
    # "${LIB_ONNX}"
    # "${LIB_ONNX_PROTO}"
    # protobuf::libprotobuf
)

# end-to-end тесты
add_executable(e2e_tests
    tests/e2e/test_onnx_parsing.cpp
)
target_link_libraries(e2e_tests PRIVATE
    compiler_core
    GTest::gtest
    GTest::gtest_main
    # "${LIB_ONNX}"
    # "${LIB_ONNX_PROTO}"
    # protobuf::libprotobuf
)

file(COPY tests/e2e/test_models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()
add_test(NAME Unit COMMAND unit_tests)
add_test(NAME E2E  COMMAND  e2e_tests)

# Для динамической сборки в MinGW
if(WIN32)
    # Позволяет линкеру быть более гибким с зависимостями DLL
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--enable-auto-import")
endif()


# для добавления динамич библиотеки
if(WIN32)
    # Определяем базовую папку vcpkg
    set(VCPKG_INSTALLED_DIR "D:/vcpkg_source/vcpkg/installed/x64-mingw-dynamic")

    # Используем $<CONFIG> для автоматического выбора папки bin или debug/bin
    # Но так как MinGW иногда капризничает, укажем пути явно через условие
    set(CURRENT_VCPKG_BIN "${VCPKG_INSTALLED_DIR}/debug/bin")

    # Мы знаем точно из ваших прошлых сообщений, что есть файл libprotobufd.dll
    # Проверьте в проводнике: если в debug/bin лежит onnx.dll без 'd', оставляем так.
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CURRENT_VCPKG_BIN}/libprotobufd.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying files from ${CURRENT_VCPKG_BIN}"
    )
endif()


find_program(GRAPHVIZ_DOT_EXECUTABLE dot)

if(GRAPHVIZ_DOT_EXECUTABLE)
    message(STATUS "Graphviz dot found: ${GRAPHVIZ_DOT_EXECUTABLE}")

    set(DOT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dump/model3.dot")
    set(PNG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dump/model3.png")

    add_custom_command(
        OUTPUT ${PNG_FILE}
        COMMAND ${GRAPHVIZ_DOT_EXECUTABLE} -Tpng ${DOT_FILE} -o ${PNG_FILE}
        DEPENDS ${DOT_FILE}  # Команда сработает, только если .dot файл изменился
        COMMENT "Converting ONNX graph to PNG image..."
    )

    add_custom_target(graph DEPENDS ${PNG_FILE})

else()
    message(WARNING "Graphviz 'dot' executable not found. Target 'graph' will not be available.")
endif()
