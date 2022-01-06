#pragma once

#include <thread>

#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"
#include "common/lobby_session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
namespace {
class LobbyServer {
public:
  LobbyServer(io::io_context& );
  ~LobbyServer();

  void Start();
  void Stop();

  void Unicast(const Message<LobbyMsg>& , std::shared_ptr<LobbySession> );
  void Broadcast(const Message<LobbyMsg>& , std::shared_ptr<LobbySession> exclude = nullptr);
  
  void Tick(std::size_t max = -1, bool is_wait = false);

private:
  void Accept();

  bool OnConnect(std::shared_ptr<LobbySession> , uint32_t );
  void OnDisconnect(std::shared_ptr<LobbySession> );
  void OnMessage(std::shared_ptr<LobbySession> , Message<LobbyMsg>& );

private:
  io::io_context& context_;
  tcp::acceptor acceptor_;

  uint32_t curr_id_ = 0;
  uint32_t max_client_ = UINT32_MAX >> 8;

  TSDeque<OwnedMessage<LobbyMsg>> read_deque_;

  std::thread thread_;
};



} 

} // ban