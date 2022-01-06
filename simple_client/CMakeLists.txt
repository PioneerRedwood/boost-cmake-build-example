# set the target name
set(SIMPLE_CLIENT "simple_client")

# sources
set(CLIENT_SRCS
    main.cpp
)
source_group(client FILES ${CLIENT_SRCS})

# add_executable
add_executable(${SIMPLE_CLIENT}
    ${CLIENT_SRCS}
    )


# include directories
# we need the boost headers
target_include_directories(${SIMPLE_CLIENT}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

# dependencies

# message(DEBUG "OUTPUT DIR: ${CMAKE_BINARY_DIR}/lib")

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# message(STATUS "OUTPUT DIR: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

# set_target_properties(${SIMPLE_CLIENT}
#     PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
# )
