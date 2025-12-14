cmake_minimum_required(VERSION 3.15)
project(CSVParser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

# Скачиваем Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

# Основное приложение
add_executable(csv_parser_app
    main.cpp
)

target_include_directories(csv_parser_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Тесты
add_executable(tests
    tests/test_tuple_print.cpp
    tests/test_csv_parser.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линкуем с Google Test
target_link_libraries(tests PRIVATE gtest gtest_main)

# Регистрируем тест
add_test(NAME AllTests COMMAND tests)