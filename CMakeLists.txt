cmake_minimum_required(VERSION 3.1.0)

project(siganim)
set(SIGANIM_VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

cmake_policy(SET CMP0076 NEW)

add_subdirectory(src)
