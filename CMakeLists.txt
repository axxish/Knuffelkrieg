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
        src/ResourceLoader.cpp
        src/ResourceLoader.h
  )

set(HEADERS
        src/EmittingStrategies.cpp
        src/EmittingStrategies.h
        src/LevelState.cpp
        src/LevelState.h
        src/ResourceLoader.cpp
        src/ResourceLoader.h
        src/colourDefs.h
 )

add_executable(Knuffelkrieg ${SOURCES} ${HEADERS})

target_link_libraries(Knuffelkrieg raylib)

set_property(TARGET Knuffelkrieg PROPERTY CXX_STANDARD 20)

target_include_directories(Knuffelkrieg PUBLIC "${raylib_SOURCE_DIR}/src")


set(RESOURCE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
set(RESOURCE_DEST_DIR "$<TARGET_FILE_DIR:Knuffelkrieg>/res")
set(RESOURCE_STAMP_FILE "${CMAKE_CURRENT_BINARY_DIR}/resource_stamp")

add_custom_command(
        OUTPUT ${RESOURCE_STAMP_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${RESOURCE_SOURCE_DIR}"
        "${RESOURCE_DEST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${RESOURCE_STAMP_FILE}"
        DEPENDS ${RESOURCE_SOURCE_DIR}
        COMMENT "Syncing resources to build directory..."
)


add_custom_target(SyncResources ALL
        DEPENDS ${RESOURCE_STAMP_FILE}
)


add_dependencies(Knuffelkrieg SyncResources)