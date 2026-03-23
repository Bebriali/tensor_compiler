enable_testing()
find_package(GTest REQUIRED)

add_executable(unit_tests
    unit/test_tokenizer.cpp
    unit/test_tree_builder.cpp
    ../../src/tokenizer.cpp
    ../../src/tree_builder.cpp
)

target_link_libraries(unit_tests GTest::gtest_main)
add_test(NAME AllUnitTests COMMAND unit_tests)
