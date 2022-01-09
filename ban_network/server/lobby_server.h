/**
 * @file lobby_server.h
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <deque>
#include <thread>

#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"
#include "common/lobby_session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {

class LobbyServer {
public:
  using Msg = Message<LobbyMsg>;
  using RemoteMsg = OwnedMessage<LobbySession, LobbyMsg>;
public:
  LobbyServer(io::io_context& );
  ~LobbyServer();

  void Start();
  void Stop();

  void Unicast(std::shared_ptr<LobbySession>, const Msg& );
  void Broadcast(const Msg&, std::shared_ptr<LobbySession>);
  
  void Tick(std::size_t , bool );

private:
  void Accept();
  void UpdateState();

  bool OnConnect(std::shared_ptr<LobbySession> , uint32_t );
  void OnDisconnect(std::shared_ptr<LobbySession> );
  void OnMessage(std::shared_ptr<LobbySession> , Msg& );

private:
  io::io_context& context_;
  tcp::acceptor acceptor_;
  io::steady_timer timer_;

  uint32_t curr_id_ = 0;
  uint32_t max_client_ = UINT32_MAX >> 8;
  std::deque<std::shared_ptr<LobbySession>> client_deque_;

  TSDeque<RemoteMsg> read_deque_;

  std::thread thread_;
}; 

} // ban