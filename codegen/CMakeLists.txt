# dump path
add_definitions(-DPROJECT_DUMP_DIR="${CMAKE_SOURCE_DIR}/log/dump")

cmake_minimum_required(VERSION 3.10)
project(Codegen CXX)

# Стандарт C++17 обязателен для std::variant и std::filesystem
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Пути к заголовочным файлам
# На твоем скриншоте это codegen/parser/lexer/include
include_directories(frontend/lexer/include
                    frontend/common
)

# 2. Список исходных файлов
set(SOURCES
    main.cpp
    frontend/lexer/src/tokenizer.cpp
    # parser/lexer/src/tree_builder.cpp
)

# 3. Создание исполняемого файла
add_executable(codegen ${SOURCES})

# Если ты добавишь тесты, CMake должен о них узнать
#add_subdirectory(tests)