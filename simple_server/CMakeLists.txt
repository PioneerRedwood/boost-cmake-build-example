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
target_include_directories(${SIMPLE_SERVER}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

