cmake_minimum_required(VERSION 3.14)	 

project(BigIntProject)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB BigInt.cpp)

add_library(BigInt STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main BigInt)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(BigInt_tests BigInt_tests.cpp ${SOURCE_LIB})
target_link_libraries(BigInt_tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(BigInt_tests)