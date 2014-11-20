cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
if(CMAKE_MAJOR_VERSION EQUAL 3)
    cmake_policy(SET CMP0042 NEW) # suppresses warning on OS X
endif()

project(dataflow)

if(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(basicmath)
add_subdirectory(interpreter)
add_subdirectory(gui)

source_group("Headers" "\\.h$")
source_group("Sources" "\\.c(pp)?$")
