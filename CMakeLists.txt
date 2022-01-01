cmake_minimum_required(VERSION 3.19)

# set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(BAN VERSION 1.0)

# hide cache variable
# set(BUILD_TESTING OFF) 

# CXX 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

# set_property(GLOBAL PROPERTY OS_FOLDERS ON)

set(BAN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
# message(STATUS "BAN_ROOT: ${BAN_ROOT}")

# include 
# target_include_directories()

# CMake 모듈
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${BAN_ROOT}/cmake")
# find_package(BOOST
include("boost_custom_variables")
include("boost_custom_macros")

if(EXISTS "${BAN_ROOT}/BAN")
  add_subdirectory(ban)
endif()