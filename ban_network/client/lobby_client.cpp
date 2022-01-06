#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"
#include "common/logger.hpp"

#include "client_option.h"
#include "lobby_client.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
LobbyClient::LobbyClient(io::io_context& context)
  : context_(context), timer_(context) {}

LobbyClient::~LobbyClient() {
  if(thread_.joinable()) {
    thread_.join();
  }

  context_.stop();
  log::Logging("[DEBUG] LobbyClient stopped ..");
}

void LobbyClient::Start() {
  conn_ = std::make_unique<LobbySession>(context_, read_deque_);
  conn_->Start(tcp::endpoint(io::ip::make_address(SERVER_ADDRESS), PORT));

  thread_ = std::thread([this]() {context_.run(); });
}

// void LobbyClient::

void LobbyClient::Heartbeat() {
  timer_.expires_from_now(io::chrono::milliseconds(TIMEOUT));
  timer_.async_wait(
    [this](const boost::system::error_code& error)->void {
      if(error) {
        log::Logging("[ERROR] LobbyClient::Heartbeat %s", error.message().c_str());
        return;
      } else {
        // make Heartbeat message
        Message<LobbyMsg> msg;
        msg.header_.id_ = LobbyMsg::HEARTBEAT;

        conn_->Send(msg); 
      }
    });
}

} // ban