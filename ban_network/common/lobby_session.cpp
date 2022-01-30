/**
 * @file lobby_session.cpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <boost/asio.hpp>

#include "logger.hpp"
#include "message.hpp"
#include "tsdeque.hpp"

#include "lobby_session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
// Session
LobbySession::LobbySession(Owner owner,
                          io::io_context& context, 
                          tcp::socket socket,
                          TSDeque<RemoteMsg>& read_deque)
: context_(context), socket_(std::move(socket)), read_deque_(read_deque) {
  owner_ = owner;
}

void LobbySession::ConnectToClient(uint32_t id = 0) {
  if(owner_ == Owner::SERVER) {
    if(socket_.is_open()) {
      is_connected_ = true;
      id_ = id;
      temp_msg_ = {};

      ReadHeader();      
    }
  }
}

void LobbySession::ConnectToServer(const tcp::resolver::results_type& endpoints) {
  if(owner_ == Owner::CLIENT) {
    io::async_connect(socket_, endpoints,
    [this](boost::system::error_code error, tcp::endpoint endpoint) {
      if(error) {
        log::Logging("[ERROR] LobbySession::ConnectToServer(): %d: %s", 
          error.value(), error.message().c_str());
        return;
      } else {
        is_connected_ = true;

        ReadHeader();
      }
    });
  }
}

bool LobbySession::IsConnected() const {
  return is_connected_ && socket_.is_open();
}

void LobbySession::Disconnect() {
  if(IsConnected()) {
    io::post(context_, [this]() {
      socket_.close();
    });
  }
}

void LobbySession::Stop() {
  is_connected_ = false;
  Disconnect();
}

void LobbySession::Send(const Msg& msg) {
  io::post(context_, [this, msg]()->void {
    bool is_writing_msg = !write_deque_.empty();
    write_deque_.push_back(msg);

    if(!is_writing_msg) {
      WriteHeader();
    }
  });
}

void LobbySession::Restart() {
  // TODO: Reset the connection or Destroy ..
  // reset the shared_ptr of it
}

void LobbySession::WriteHeader() {
  io::async_write(socket_, 
    io::buffer(&write_deque_.front().header_, sizeof(MessageHeader<LobbyMsg>)),
    [this](boost::system::error_code error, std::size_t bytes)->void {
      if(error) {
        log::Logging("[ERROR] LobbySession::WriteHeader(): %d: %s", 
          error.value(), error.message().c_str());
        socket_.close();
        return ;
      } else {
        if(write_deque_.front().body_.size() > 0) {

          WriteBody();

        } else {
          write_deque_.pop_front();

          if(!write_deque_.empty()) {
            WriteHeader();
          }
        }
      }
    });
}

void LobbySession::WriteBody() {
  io::async_write(socket_, 
    io::buffer(write_deque_.front().body_.data(), write_deque_.front().body_.size()),
    [this](boost::system::error_code error, std::size_t bytes)->void {
      if(error) {
        log::Logging("[ERROR] LobbySession::WriteBody() %d: %s", 
          error.value(), error.message().c_str());
        socket_.close();
        return;
      } else {
        write_deque_.pop_front();

        if(!write_deque_.empty()) {
          WriteHeader();
        }
      }
    });
}

void LobbySession::ReadHeader() {
  io::async_read(socket_,
    io::buffer(&temp_msg_.header_, sizeof(MessageHeader<LobbyMsg>)), 
    [this](boost::system::error_code error, std::size_t bytes) {
      if(error) {
        log::Logging("[ERROR] LobbySession::ReadHeader() %d: %s", 
          error.value(), error.message().c_str());
        socket_.close();
        return;
      } else {
        if(temp_msg_.header_.size_ > 0) {
          
          temp_msg_.body_.resize(temp_msg_.header_.size_);
          
          ReadBody();
        } else {
          Store();
        }
      }
    });
}

void LobbySession::ReadBody() {
  io::async_read(socket_, 
    io::buffer(temp_msg_.body_.data(), temp_msg_.body_.size()),
    [this](boost::system::error_code error, std::size_t bytes) {
      if(error) {
        log::Logging("[ERROR] LobbySession::ReadBody() %d: %s", 
          error.value(), error.message().c_str());
        socket_.close();
        return;
      } else {
        Store();
      }
    });
}

void LobbySession::Store() {
  //log::Logging("[DEBUG] LobbySession::Store() {Msg {id: %d} {size: %d}", temp_msg_.header_.id_, temp_msg_.header_.size_);
  if (owner_ == Owner::SERVER) {
    read_deque_.push_back({ this->shared_from_this(), temp_msg_ });
  } else {
    read_deque_.push_back({ nullptr, temp_msg_ });
  }


  ReadHeader();
}

} // ban