# set the target name
set(BANSERVER "ban_server")

# preprocessor
add_compile_definitions(
    _WIN32_WINNT=0x0501
    )

# common srcs
set(BAN_COMMON_SRCS
    ../common/message.hpp
    ../common/time_util.hpp
    ../common/logger.hpp
    ../common/tsdeque.hpp
    ../common/lobby_session.cpp
    ../common/lobby_session.h
    )
source_group(common FILES ${BAN_COMMON_SRCS})

# server core srcs
set(BANSERVER_CORE_SRCS
    lobby_server.h
    lobby_server.cpp
    server_option.h
    main.cpp
    )
source_group(server FILES ${BANSERVER_CORE_SRCS})

set(BANSERVER_SRCS
    ${BANSERVER_CORE_SRCS}
    ${BAN_COMMON_SRCS}
    )

# add_executable
add_executable(${BANSERVER} ${BANSERVER_SRCS})

# add include directories
target_include_directories(${BANSERVER}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${CMAKE_SOURCE_DIR}/ban_network
    )

# target_link_directories(${BANSERVER}
#     PRIVATE ${CMAKE_SOURCE_DIR}/ban_network
# )

# set_target_properties(${BANSERVER}
#     PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build/lib"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build/bin"
#     )
