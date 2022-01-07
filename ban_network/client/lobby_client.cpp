#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"
#include "common/logger.hpp"

#include "client_option.h"
#include "lobby_client.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {

LobbyClient::ClientSession::ClientSession(io::io_context& context,
                            std::weak_ptr<LobbyClient> owner,
                            TSDeque<OwnedMessage<LobbyMsg>>& read_deque)
  : LobbySession(context, tcp::socket(nullptr), read_deque), owner_(owner) {}

void LobbyClient::ClientSession::Start(tcp::endpoint endpoint) {
  socket_.async_connect(endpoint,
    [this](const boost::system::error_code& error)->void {
      if(error) {
        log::Logging("[DEBUG] ClientSession::Start %d", error.value());
        is_connected_ = false;
        Stop();
        return;
      } else {
        log::Logging("[DEBUG] ClientSession::Start");
        is_connected_ = true;
        // owner weakptr invalid check
        //if(owner_) {
        //  owner_->Heartbeat();
        //}
      }
    });
}

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
  conn_ = std::make_shared<ClientSession>(context_, std::make_shared<LobbyClient>(this), read_deque_);

  conn_->Start(tcp::endpoint(io::ip::make_address(SERVER_ADDRESS), PORT));
  
  Heartbeat();
  
  thread_ = std::thread([this]() {context_.run(); });
}

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

      Heartbeat();
    });
}

} // ban