#pragma once

#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {

class LobbySession;

template<typename T>
struct OwnedMessage {
    std::shared_ptr<LobbySession> remote_ = nullptr;
    Message<T> msg_;

    friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg) {
        os << msg.msg_;
        return os;
    }
};

enum class LobbyMsg : uint32_t;

class LobbySession : public std::enable_shared_from_this<LobbySession> {
public:
  LobbySession(io::io_context&, tcp::socket, TSDeque<OwnedMessage<LobbyMsg>>& );

  void Start(uint32_t);
  void Stop();
  void Restart();

  bool Connected() const;
  void Send(const Message<LobbyMsg>&);

  tcp::socket& GetSocket() {return socket_;}

  const uint32_t GetId();

private:
  void WriteHeader();
  void WriteBody();
  
  void ReadHeader();
  void ReadBody();

  void Store();
protected:
  io::io_context& context_;
  tcp::socket socket_;

  TSDeque<OwnedMessage<LobbyMsg>>& read_deque_;
  TSDeque<Message<LobbyMsg>> write_deque_;
  Message<LobbyMsg> temp_msg_;

  bool is_connected_ = false;
  // Session info
  uint32_t id_ = 0;
};
}