cmake_minimum_required(VERSION 3.2)

project(Minecraft)

add_subdirectory(engine/)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Minecraft ${SOURCES})
target_link_libraries(Minecraft Engine)
include_directories(Minecraft "engine/include/")

if( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(Minecraft PRIVATE -Wall -Wextra)
endif()

if( MSVC )
    target_compile_options(Minecraft PRIVATE /W4 /ZI)
    set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT "Minecraft")
    set_property(TARGET Minecraft PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message("Setting VS Working Dir to: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()