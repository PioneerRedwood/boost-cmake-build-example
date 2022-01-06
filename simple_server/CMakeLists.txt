
# set the target name
set(SIMPLE_SERVER "simple_server")

# sources
set(SERVER_SRCS
    udp_server.hpp
    main.cpp
)
source_group(server FILES ${SERVER_SRCS})

# add_executable
add_executable(${SIMPLE_SERVER}
    ${SERVER_SRCS}
    )

# include directories
# we need the boost headers
target_include_directories(${SIMPLE_SERVER}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

# dependencies

message(DEBUG "OUTPUT DIR: ${CMAKE_BINARY_DIR}/lib")

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# message(STATUS "OUTPUT DIR: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

# set_target_properties(${SIMPLE_SERVER}
#     PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
# )
