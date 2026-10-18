cmake_minimum_required(VERSION 3.13)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("Breakout")


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "$${CMAKE_CURENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE USER_HEADER CONFIGURE_DEPENDS "$${CMAKE_CURENT_SOURCE_DIR}/include/*.h")



if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libbs/SDL/CMakeLists.txt")
    find_package(Git REQUIRED)
    message(STATUS "Initializing git submodules...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
add_subdirectory("libbs/SDL")




add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURENT_SOURCE_DIR}/include/")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
