#include <ctime>
#include <exception>
#include <type_traits>
#include <boost/asio.hpp>

#include "common/lobby_session.h"
#include "common/message.hpp"
#include "common/tsdeque.hpp"
#include "common/logger.hpp"

#include "client_option.h"
#include "lobby_client.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

std::string MakeDateString() {
  time_t now = time(0);
  return ctime(&now);
}

namespace ban {
LobbyClient::LobbyClient(io::io_context& context)
  : context_(context), timer_(context) {}

LobbyClient::~LobbyClient() {
  if(thread_.joinable()) {
    thread_.join();
  }

  if (biz_thread_.joinable()) {
    biz_thread_.join();
  }

  context_.stop();
  log::Logging("[DEBUG] LobbyClient stopped ..");
}

void LobbyClient::Start() {
  log::Logging("[DEBUG] LobbyClient started ..");

  conn_ = std::make_unique<LobbySession>(Owner::CLIENT, context_, tcp::socket(context_), read_deque_);
  
  tcp::resolver resolver(context_);
  tcp::resolver::results_type endpoints = resolver.resolve(SERVER_ADDRESS, std::to_string(PORT));

  conn_->ConnectToServer(endpoints);

  thread_ = std::thread([this]() { context_.run(); });
  /*biz_thread_ = std::thread([this]() {
  * 
    while (true) {
      if (false) {
        read_deque_.wait();
      }

      std::size_t count = 0;
      while (count < -1 && !read_deque_.empty()) {
        auto temp = read_deque_.pop_front();

        log::Logging("[DEBUG] Client Tick.. {Remote:%d, ID:%d}", temp.remote_, temp.msg_.header_.id_);
        count++;
      }

      if (!conn_->IsConnected()) {
        break;
      }
    }
    });*/

  Heartbeat();
}

void LobbyClient::Heartbeat() {
  timer_.expires_from_now(io::chrono::milliseconds(TIMEOUT));
  timer_.async_wait(
    [this](const boost::system::error_code error)->void {
      if(error) {
        log::Logging("[ERROR] LobbyClient::Heartbeat %d: %s",
          error.value(), error.message().c_str());
        return;
      } else {
        if (conn_->IsConnected()) {
          Msg msg;
          msg.header_.id_ = LobbyMsg::HEARTBEAT;
          
          //msg << "Hello server!";
          conn_->Send(msg);
        } else {
          log::Logging("[DEBUG] LobbyClient::Heartbeat but not connected with server");
        }

        /*{
          read_deque_.wait();
          if (!read_deque_.empty()) {
            auto temp = read_deque_.pop_front();

            log::Logging("[DEBUG] Client Tick.. {Remote:%d, ID:%d}", temp.remote_, temp.msg_.header_.id_);
          }
        }*/
      }

      Heartbeat();
    });
}

} // ban