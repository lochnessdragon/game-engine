cmake_minimum_required(VERSION 3.2)

project(Engine)

function(CheckDep depName)
  if( NOT DEFINED ${depName}_DIR )
    # error out
    message(SEND_ERROR "${depName} couldn't be found, set the ${depName}_DIR variable to the location of the ${depName} src tree.")
  endif()
endfunction()

function(AddDep depName)
  CheckDep(${depName})
  if( DEFINED ${depName}_DIR)
    add_subdirectory(${${depName}_DIR} lib/${depName})
  endif()
endfunction()

AddDep(GLFW)
AddDep(GLM)
CheckDep(STB)
# add_subdirectory(lib/glfw/)
#
# add_subdirectory(lib/glm/)
#
add_subdirectory(lib/glad/)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(Engine STATIC ${SOURCES})
target_link_libraries(Engine glfw Glad glm)
target_include_directories(Engine PUBLIC "include/" "${GLFW_DIR}/include" "lib/glad/include" "${GLM_DIR}" ${STB_DIR})

if( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(Engine PRIVATE -Wall -Wextra)
    target_link_libraries(Engine GL)
endif()

if( MSVC )
    target_compile_options(Engine PRIVATE /W4 /ZI)
endif()
