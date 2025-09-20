cmake_minimum_required(VERSION 3.27.7)
project(Knuffelkrieg)

find_package(raylib 5.5 QUIET)


# This code downloads raylib into a directory called _deps and adds it as a subdirectory, compiling it with the program when running the build command
include(FetchContent)
if(NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP True #This option is not required but suppresses a warning
    )
    FetchContent_MakeAvailable(raylib)
endif()


set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SOURCES src/main.cpp src/MovementStrategies.cpp
        src/EmittingStrategies.cpp
        src/EmittingStrategies.h
        src/LevelState.cpp
        src/LevelState.h
        src/Graphics.cpp
        src/Graphics.h)

set(HEADERS
        src/EmittingStrategies.cpp
        src/EmittingStrategies.h
        src/LevelState.cpp
        src/LevelState.h
        src/Graphics.cpp
        src/Graphics.h)

add_executable(Knuffelkrieg ${SOURCES} ${HEADERS})

target_link_libraries(Knuffelkrieg raylib)

set_property(TARGET Knuffelkrieg PROPERTY CXX_STANDARD 20)

target_include_directories(Knuffelkrieg PUBLIC "${raylib_SOURCE_DIR}/src")


add_custom_command(
    TARGET Knuffelkrieg POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res" "$<TARGET_FILE_DIR:Knuffelkrieg>/res"
)

