#include "common/logger.hpp"
#include "common/message.hpp"
#include "common/lobby_session.h"

#include "server_option.h"
#include "lobby_server.h"


namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
LobbyServer::LobbyServer(io::io_context& context)
  : context_(context), acceptor_(context, tcp::endpoint(tcp::v4(), SERVER_PORT)) {}

LobbyServer::~LobbyServer() {
  context_.stop();
  log::Logging("[DEBUG] LobbyServer deinitialized ..");

  if(thread_.joinable()) {
    thread_.join();
  }
}

void LobbyServer::Start() {
  log::Logging("[DEBUG] LobbyServer started..");
  
  Accept();

  thread_ = std::thread([this]() {context_.run();});
}

void LobbyServer::Stop() {
  log::Logging("[DEBUG] LobbyServer stopped");
}

void LobbyServer::Unicast(const Message<LobbyMsg>& msg, std::shared_ptr<LobbySession> client) {
  if(client && client->Connected()) {
    client->Send(msg);
  } else {
    OnDisconnect(client);
    client.reset(); // destroy
  }
}

void LobbyServer::Broadcast(const Message<LobbyMsg>& msg, std::shared_ptr<LobbySession> exclude = nullptr) {
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
  acceptor_.async_accept([this](boost::system::error_code& error, tcp::socket socket) {
    if(error) {
      log::Logging("[ERROR] LobbyServer::Accept .. %d", error.value());
    } else {
      // get into the server!
      std::stringstream ss;
      ss << socket.remote_endpoint();

      log::Logging("[DEBUG] new connection! %s", ss.str().c_str());

      std::shared_ptr<LobbySession> conn = 
        std::make_shared<LobbySession>(context_, std::move(socket), read_deque_);

      if(OnConnect(conn, curr_id_)) {
        
        conn->Start(curr_id_);
        // conn->Start(started_time, id); overloaded function

        log::Logging("[DEBUG] [%d] connection approved", curr_id_);
        curr_id_++;
      } else {
        log::Logging("[DEBUG] connection denied");
      }
    }

    Accept();
  });
}

bool LobbyServer::OnConnect(std::shared_ptr<LobbySession> client, uint32_t id) {
  Message<LobbyMsg> msg;
  msg.header_.id_ = LobbyMsg::HEARTBEAT;

  msg << id;
  std::cout << id << "\n";
  client->Send(msg);
  return true;
}

void LobbyServer::OnDisconnect(std::shared_ptr<LobbySession> client) {
  log::Logging("[DEBUG] removing client [%d]", client->GetId());
}

void LobbyServer::OnMessage(std::shared_ptr<LobbySession> client, Message<LobbyMsg>& msg) {
  switch(msg.header_.id_) {
  case LobbyMsg::HEARTBEAT: {
    log::Logging("[DEBUG] [%d] Heartbeating", client->GetId());
    
    Unicast(msg, client); // just echo!

    break;
  }
  default: {
    break;
  }
  } // switch(data.header_.id_) -- LobbyMsg
}

}