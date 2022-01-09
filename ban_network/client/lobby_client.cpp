/**
 * @file lobby_client.cpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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

  context_.stop();
  log::Logging("[DEBUG] LobbyClient stopped ..");
}

void LobbyClient::Start() {
  log::Logging("[DEBUG] LobbyClient started ..");

  conn_ = std::make_unique<LobbySession>(
    Owner::CLIENT, context_, tcp::socket(context_), read_deque_);
  
  tcp::resolver resolver(context_);
  tcp::resolver::results_type endpoints = resolver.resolve(SERVER_ADDRESS, std::to_string(PORT));

  conn_->ConnectToServer(endpoints);

  thread_ = std::thread([this]() { context_.run(); });

  Heartbeat();
}

void LobbyClient::Send(const Msg& msg) {
  if (IsConnected()) {
    conn_->Send(msg);
  }
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
        if (conn_ && conn_->IsConnected()) {
          Msg msg;
          msg.header_.id_ = LobbyMsg::HEARTBEAT;

          Send(msg);
        } else {
          log::Logging("[DEBUG] LobbyClient::Heartbeat but not connected with server");
        }
      }
      
      Heartbeat();
    });
}

void LobbyClient::Tick() {
  while (!read_deque_.empty()) {
    Msg temp = read_deque_.pop_front().msg_;

    switch (temp.header_.id_) {
    case LobbyMsg::HEARTBEAT: {
      log::Logging("[DEBUG] Heartbeating echo");

      break;
    }
    default: {
      break;
    }
    } // switch(data.header_.id_) -- LobbyMsg
  }
}

} // ban