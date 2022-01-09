/**
 * @file lobby_server.cpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "common/logger.hpp"
#include "common/message.hpp"
#include "common/lobby_session.h"

#include "server_option.h"
#include "lobby_server.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
LobbyServer::LobbyServer(io::io_context& context)
  : context_(context), acceptor_(context, tcp::endpoint(tcp::v4(), SERVER_PORT)), 
    timer_(context) {}

LobbyServer::~LobbyServer() {
  context_.stop();
  log::Logging("[DEBUG] LobbyServer deinitialized ..");

  if(thread_.joinable()) {
    thread_.join();
  }
}

void LobbyServer::Start() {
  log::Logging("[DEBUG] LobbyServer started..");
  
  UpdateState();
  Accept();

  thread_ = std::thread([this]() {context_.run();});
}

void LobbyServer::Stop() {
  log::Logging("[DEBUG] LobbyServer stopped");
}

void LobbyServer::Unicast(std::shared_ptr<LobbySession> client, const Msg& msg) {
  if(client && client->IsConnected()) {
    client->Send(msg);
  } else {
    OnDisconnect(client);
    // destroy
    client.reset();
  }
}

void LobbyServer::Broadcast(const Msg& msg, std::shared_ptr<LobbySession> exclude = nullptr) {
  // bool is_invaild = false;
  // std::vector<uint32_t> temp;
  // Broadcast does not work yet.
}

void LobbyServer::Tick(std::size_t max = -1, bool is_wait = false) {
  if(is_wait) {
    read_deque_.wait();
  }

  std::size_t count = 0;
  while((count < max) && !read_deque_.empty()) {
    auto temp = read_deque_.pop_front();

    OnMessage(temp.remote_, temp.msg_);

    count++;
  }
}

void LobbyServer::Accept() {
  acceptor_.async_accept(
    [this](boost::system::error_code error, tcp::socket socket) {
    if(error) {
      log::Logging("[ERROR] LobbyServer::Accept .. %d", error.value());
    } else {
      // get into the server!
      std::stringstream ss;
      ss << socket.remote_endpoint();

      log::Logging("[DEBUG] new connection! %s", ss.str().c_str());

      std::shared_ptr<LobbySession> conn = 
        std::make_shared<LobbySession>(Owner::SERVER, context_, std::move(socket), read_deque_);

      if(OnConnect(conn, curr_id_)) {
        client_deque_.push_back(std::move(conn));
        client_deque_.back()->ConnectToClient(curr_id_++);

        log::Logging("[DEBUG] [%d] connection approved", curr_id_);
      } else {
        log::Logging("[DEBUG] connection denied");
      }
    }

    Accept();
  });
}

bool LobbyServer::OnConnect(std::shared_ptr<LobbySession> client, uint32_t id) {
  //Msg msg;
  //msg.header_.id_ = LobbyMsg::HEARTBEAT;

  //client->Send(msg);
  return true;
}

void LobbyServer::OnDisconnect(std::shared_ptr<LobbySession> client) {
  log::Logging("[DEBUG] removing client [%d]", client->GetId());
}

void LobbyServer::OnMessage(std::shared_ptr<LobbySession> client, Msg& msg) {
  switch(msg.header_.id_) {
  case LobbyMsg::HEARTBEAT: {
    log::Logging("[DEBUG] [%d] Heartbeating", client->GetId());
    
    Unicast(client, msg); // just echo!

    break;
  }
  default: {
    break;
  }
  } // switch(data.header_.id_) -- LobbyMsg
}

void LobbyServer::UpdateState() {
  timer_.expires_from_now(io::chrono::milliseconds(2000));
  timer_.async_wait(
    [this](const boost::system::error_code error)->void {
      if (error) {
        log::Logging("[ERROR] LobbyServer::UpdateState %d: %s",
          error.value(), error.message().c_str());
        return;
      }
      else {
        log::Logging("[DEBUG] LobbyServer::UpdateState");
        UpdateState();
      }
    });
}

}