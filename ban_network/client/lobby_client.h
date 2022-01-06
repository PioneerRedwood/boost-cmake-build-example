#pragma once
#include <boost/asio.hpp>

#include "common/lobby_session.h"
#include "common/logger.hpp"
#include "common/message.hpp"
#include "common/tsdeque.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
class LobbyClient {
public:
  LobbyClient(io::io_context&);
  ~LobbyClient();

  void Start();

private:
  void Heartbeat();

private:
  io::io_context& context_;
  std::unique_ptr<LobbySession> conn_;
  TSDeque<OwnedMessage<LobbyMsg>> read_deque_;
  io::steady_timer timer_;

  std::thread thread_;
};
}