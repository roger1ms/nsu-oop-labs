cmake_minimum_required(VERSION 3.15)
project(PrisonersDilemma VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Основное приложение
add_executable(prisoners_dilemma
    main.cpp
    GameRunner.cpp
    GameRunner.h
    Game.cpp
    Game.h
    Strategy.cpp
    Strategy.h
    StrategyFactory.cpp
    StrategyFactory.h
)
target_include_directories(prisoners_dilemma PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Тесты
add_executable(tests
    tests/test_strategies.cpp
    tests/test_game.cpp
    tests/test_gamerun.cpp
    
    Game.cpp
    GameRunner.cpp
    Strategy.cpp
    StrategyFactory.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests GTest::gtest_main)

# Простая альтернатива gtest_discover_tests
add_test(NAME AllTests COMMAND tests)