
# SOURCES
set(BAN_TIMER_SRCS
    include/custom_timer.hpp
    include/http_sync_client.hpp
)
source_group(ban_timer\\\\timer FILES ${BAN_TIMER_SRCS})

set(BAN_TIMER_MAIN
    main.cpp
)
source_group(ban_timer FILES ${BAN_TIMER_MAIN})

set(BAN_SRCS
    ${BAN_TIMER_SRCS}
    ${BAN_TIMER_MAIN}
)

# target이 뭘까?
# target_include_directories()
# target_compile_options()
# target_link_directories()
# target_link_libraries()
# target_link_options()
# target_precompile_headers()
# target_sources()

set(BAN_TARGET "ban_timer")

# # Path to the include directory
# set(BAN_INCLUDE_PATH "${BOOST_ROOT}/include")

add_executable(${BAN_TARGET} 
    ${BAN_SRCS}
)

# Include directories
target_include_directories(
    ${BAN_TARGET} PRIVATE ${BAN_TIMER_SRCS}
)

# Qt + Cef 해봐야함
# boost는 필요한 파일만 가져오면 될듯
