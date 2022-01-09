/**
 * @file client_session.cpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * This is deprecated, do not use this.
 */
#include <boost/asio.hpp>

#include "common/logger.hpp"
#include "common/message.hpp"
#include "common/tsdeque.hpp"
#include "common/lobby_session.h"

#include "client_session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
ClientSession::ClientSession(io::io_context& context, TSDeque<RemoteMsg>& read_deque)
    : context_(context), read_deque_(read_deque), socket_(context) {}

ClientSession::~ClientSession() {
    Stop();
}

void ClientSession::Start(tcp::endpoint endpoint) {
  socket_.async_connect(endpoint,
      [this](const boost::system::error_code& error)->void {
        if(error) {
          log::Logging("[DEBUG] ClientSession::Start %d", error.value());
          is_connected_ = false;
          Stop();
          return;
        } else {
          log::Logging("[DEBUG] ClientSession started ..");
          is_connected_ = true;
          
          //ReadHeader();
        }
      });
}

void ClientSession::Start(const tcp::resolver::results_type& endpoints) {
  io::async_connect(socket_, endpoints,
    [this](boost::system::error_code error, tcp::endpoint endpoint) {
      if (error) {
        log::Logging("[DEBUG] ClientSession::Start %d", error.value());
        is_connected_ = false;
        Stop();
        return;
      } else {
        log::Logging("[DEBUG] ClientSession started ..");
        is_connected_ = true;

        //ReadHeader();
      }
    });

}

void ClientSession::Stop() {
  is_connected_ = false;
  if(socket_.is_open()) {
    socket_.close();
  }
}

void ClientSession::Send(const std::shared_ptr<Msg> msg) {
  io::post(context_, [this, msg]()->void {
    bool is_writing_msg = !write_deque_.empty();
    write_deque_.push_back(msg);

    if(!is_writing_msg) {
      WriteHeader();
    }
  });
}

void ClientSession::WriteHeader() {
  io::async_write(socket_, 
    io::buffer(&write_deque_.front()->header_, sizeof(uint32_t)), 
    [this](boost::system::error_code error, std::size_t bytes)->void {
      if(error) {
        log::Logging("[ERROR] ClientSession::WriteHeader(): %s", error.message().c_str());
        return ;
      } else {
        if(write_deque_.front()->body_.size() > 0) {
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

void ClientSession::WriteBody() {
  io::async_write(socket_, 
    io::buffer(write_deque_.front()->body_.data(), write_deque_.front()->body_.size()),
    [this](boost::system::error_code error, std::size_t bytes)->void {
      if(error) {
        log::Logging("[ERROR] ClientSession::WriteBody(): %s", error.message().c_str());
        return;
      } else {
        write_deque_.pop_front();

        if(!write_deque_.empty()) {
          WriteHeader();
        }
      }
    });
}

void ClientSession::ReadHeader() {
  io::async_read(socket_,
    io::buffer(&temp_msg_.header_, sizeof(MessageHeader<LobbyMsg>)), 
    [this](auto error, std::size_t bytes) {
      if(error) {
        log::Logging("[ERROR] ClientSession::ReadHeader(): %s", error.message().c_str());
        return;
      } else {
        Store();
        //if(temp_msg_.header_.size_ > 0) {
        //  temp_msg_.body_.resize(temp_msg_.header_.size_);
        //  
        //  ReadBody();
        //} else {
        //  Store();
        //}
      }
    });
}

void ClientSession::ReadBody() {
  io::async_read(socket_, 
    io::buffer(temp_msg_.body_.data(), temp_msg_.body_.size()),
    [this](auto error, std::size_t bytes) {
      if(error) {
        log::Logging("[ERROR] ClientSession::ReadBody(): %s", error.message().c_str());
        return;
      } else {
        Store();
      }
    });
}

void ClientSession::Store() {
  read_deque_.push_back({this->shared_from_this(), temp_msg_});

  ReadHeader();
}

}
