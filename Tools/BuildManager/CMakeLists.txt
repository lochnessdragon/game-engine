cmake_minimum_required(VERSION 3.14)

project(BuildManager)

file(GLOB_RECURSE BUILD_MANAGER_SOURCES "Source/*.cpp" "Source/*.h")

add_executable(bldr ${BUILD_MANAGER_SOURCES})

target_include_directories(bldr PUBLIC "Source/")