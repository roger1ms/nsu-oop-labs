cmake_minimum_required(VERSION 3.14)

project(TanksGame CXX)

set(CMAKE_CXX_STANDARD 17)

set(exe_name ${PROJECT_NAME})

if(WIN32 OR MSVC)
    include(FetchContent)
    FetchContent_Declare(
        PDcurses
        GIT_REPOSITORY https://github.com/dsavenko/PDCurses.git
        GIT_TAG        dfbac0fe5ef90a40a67d6b13cc7d9c69bf40a5f5
    )
    FetchContent_MakeAvailable(PDcurses)
endif()

add_executable(${exe_name} 
    main.cpp
    Game.cpp
    GameContext.cpp
    GameObjects.cpp
)

target_include_directories(${exe_name} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32 OR MSVC)
    target_link_libraries(${exe_name} PDcurses)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(NCURSES REQUIRED ncurses)
    target_link_libraries(${exe_name} ${NCURSES_LIBRARIES})
endif()