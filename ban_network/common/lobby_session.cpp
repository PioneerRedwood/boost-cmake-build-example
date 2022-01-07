#include <boost/asio.hpp>

#include "logger.hpp"
#include "message.hpp"
#include "tsdeque.hpp"

#include "lobby_session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
// Session
LobbySession::LobbySession(io::io_context& context, 
                          tcp::socket socket,
                          TSDeque<OwnedMessage<LobbyMsg>>& read_deque)
:context_(context), socket_(std::move(socket)), read_deque_(read_deque) {}

void LobbySession::Start(uint32_t id) {
  is_connected_ = true;
  id_ = id;

  ReadHeader();
}

bool LobbySession::Connected() const {
  return is_connected_ && socket_.is_open();
}

void LobbySession::Stop() {
  is_connected_ = false;
  if(socket_.is_open()) {
    socket_.close();
  }
}

void LobbySession::Restart() {
  // TODO: Reset the connection or Destroy ..
  // reset the shared_ptr of it
}

void LobbySession::Send(const Message<LobbyMsg>& msg) {
  io::post(context_, [this, msg]()->void {
    bool is_writing_msg = !write_deque_.empty();
    write_deque_.push_back(msg);

    if(!is_writing_msg) {
      WriteHeader();
    }
  });
}

const uint32_t LobbySession::GetId() {
  return id_;
}

void LobbySession::WriteHeader() {
  io::async_write(socket_, io::buffer(&write_deque_.front().header_, sizeof(uint32_t)), 
    [this](boost::system::error_code error, std::size_t bytes)->void {
      if(error) {
        log::Logging("[ERROR] LobbySession::WriteHeader(): %d", error.value());
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
  io::async_write(socket_, io::buffer(write_deque_.front().body_.data(), write_deque_.front().body_.size()),
    [this](boost::system::error_code error, std::size_t bytes)->void {
      if(error) {
        log::Logging("[ERROR] LobbySession::WriteBody(): %d", error.value());
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
  io::async_read(socket_, io::buffer(&temp_msg_.header_, sizeof(MessageHeader<LobbyMsg>)), 
    [this](auto error, std::size_t bytes) {
      if(error) {
        log::Logging("[ERROR] LobbySession::ReadHeader(): %d", error.value());
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
  io::async_read(socket_, io::buffer(temp_msg_.body_.data(), temp_msg_.body_.size()),
    [this](auto error, std::size_t bytes) {
      if(error) {
        log::Logging("[ERROR] LobbySession::ReadBody(): %d", error.value());
        return;
      } else {
        Store();
      }
    });
}

void LobbySession::Store() {
  read_deque_.push_back({this->shared_from_this(), temp_msg_});

  ReadHeader();
}

} // ban