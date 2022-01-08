#pragma once
#include <boost/asio.hpp>

#include "common/lobby_session.h"
#include "common/logger.hpp"
#include "common/message.hpp"
#include "common/tsdeque.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {

class ClientSession;

class LobbyClient {
public:
  using Msg = Message<LobbyMsg>;
  using RemoteMsg = OwnedMessage<LobbySession, LobbyMsg>;
public:
  LobbyClient(io::io_context&);
  ~LobbyClient();

  void Start();
  void Heartbeat();

private:
  io::io_context& context_;
  io::steady_timer timer_;
  std::unique_ptr<LobbySession> conn_;
  
  TSDeque<RemoteMsg> read_deque_;
  
  std::thread thread_;
  std::thread biz_thread_;
};
}