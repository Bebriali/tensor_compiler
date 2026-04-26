find_package(GTest REQUIRED)
include(GoogleTest)

# Создаем тестовый бинарник
add_executable(unit_tests 
    unit/test_tokenizer.cpp 
    unit/test_dump.cpp
    unit/test_tree_builder.cpp
    ../frontend/lexer/src/tokenizer.cpp
    ../frontend/parser/src/tree_builder.cpp
)

# Подключаем инклуды лексера и GTest
target_include_directories(unit_tests PRIVATE ../parser/lexer/include)
target_link_libraries(unit_tests GTest::GTest GTest::Main)

# Регистрируем тесты в системе
gtest_discover_tests(unit_tests)