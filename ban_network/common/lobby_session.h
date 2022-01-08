#pragma once

#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
enum class Owner : uint16_t {
  SERVER = 0,
  CLIENT = 1,
};

class LobbySession;

template<typename Remote, typename MsgType>
struct OwnedMessage {
    std::shared_ptr<Remote> remote_ = nullptr;
    Message<MsgType> msg_;

    friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<Remote, MsgType>& msg) {
        os << msg.msg_;
        return os;
    }
};

enum class LobbyMsg : uint32_t;

class LobbySession : public std::enable_shared_from_this<LobbySession> {
public:
  using Msg = Message<LobbyMsg>;
  using RemoteMsg = OwnedMessage<LobbySession, LobbyMsg>;
public:
  LobbySession(Owner, io::io_context&, tcp::socket, TSDeque<RemoteMsg>& );
  ~LobbySession() {};
  
  void ConnectToClient(uint32_t);
  void ConnectToServer(const tcp::resolver::results_type&);

  void Stop();
  void Restart();

  bool IsConnected() const;
  void Disconnect();
  void Send(const Msg&);

  tcp::socket& GetSocket() { return socket_; }

  uint32_t GetId() { return id_; }

private:
  void WriteHeader();
  void WriteBody();
  
  void ReadHeader();
  void ReadBody();

  void Store();
protected:
  io::io_context& context_;
  tcp::socket socket_;

  TSDeque<RemoteMsg>& read_deque_;
  TSDeque<Msg> write_deque_;
  Msg temp_msg_;

  Owner owner_ = Owner::SERVER;

  bool is_connected_ = false;
  // Session info
  uint32_t id_ = 0;
};
}