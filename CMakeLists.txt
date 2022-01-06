cmake_minimum_required(VERSION 3.19)

# set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(BOOST_CMAKE_EXAMPLES VERSION 1.0)

# hide cache variable
# set(BUILD_TESTING OFF) 

# CXX 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

find_package(Boost 1.77.0 REQUIRED)

if(${Boost_FOUND} AND ${Boost_VERSION_STRING} EQUAL "1.77.0")
  message(DEBUG "Boost found, version: " ${Boost_VERSION_STRING})
  # include boost header files
  include_directories(${Boost_INCLUDE_DIRS})
else()
  # download and expension boost for here
endif()

# if(EXISTS "${BAN_ROOT}/BAN")
#   add_subdirectory(ban)
# endif()

# target all
# add_subdirectory(simple_server)
# add_subdirectory(simple_client)

add_subdirectory(ban_network)
