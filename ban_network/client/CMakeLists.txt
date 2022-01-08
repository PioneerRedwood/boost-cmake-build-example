# set the target name
set(BANCLIENT "ban_client")

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

# sources core srcs
set(BANCLIENT_CORE_SRCS
    client_option.h
    # client_session.h
    # client_session.cpp
    lobby_client.h
    lobby_client.cpp
    
    main.cpp
    )
source_group(client FILES ${BANCLIENT_CORE_SRCS})

set(BANCLIENT_SRCS
    ${BANCLIENT_CORE_SRCS}
    ${BAN_COMMON_SRCS}
)

# Executable target
add_executable(${BANCLIENT} ${BANCLIENT_SRCS})

# include Boost libraraies
target_include_directories(${BANCLIENT}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${CMAKE_SOURCE_DIR}/ban_network
    )

# set_target_properties(${BANCLIENT}
#     PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ban_network/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ban_network/lib"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ban_network/bin"
#     )