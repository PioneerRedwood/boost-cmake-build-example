set(BAN_COMMON_TARGET "ban_common")

set(BAN_COMMON_SRCS
  lobby_session.cpp
  lobby_session.h
  logger.hpp
  message.hpp
  time_util.hpp
  tsdeque.hpp
  )

add_library(${BAN_COMMON_TARGET}
  ${BAN_COMMON_SRCS}
  )

set(BAN_COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
