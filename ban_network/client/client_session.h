#pragma once
#include <boost/asio.hpp>

#include "common/message.hpp"
#include "common/tsdeque.hpp"

#include "common/lobby_session.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {
class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
  using Msg = Message<LobbyMsg>;
  using RemoteMsg = OwnedMessage<ClientSession, LobbyMsg>;
  ClientSession(io::io_context& context,
                TSDeque<RemoteMsg>& read_deque);
  
  ~ClientSession();

  void Start(tcp::endpoint endpoint);
  void Start(const tcp::resolver::results_type& endpoints);
  void Stop();
  void Send(const std::shared_ptr<Msg>);

  bool Connected() { return is_connected_ && socket_.is_open(); }
  tcp::socket& GetSocket() { return socket_; }
private:
  void WriteHeader();
  void WriteBody();
  
  void ReadHeader();
  void ReadBody();

  void Store();

private:
  io::io_context& context_;
  tcp::socket socket_;

  TSDeque<RemoteMsg>& read_deque_;
  TSDeque<std::shared_ptr<Msg>> write_deque_;
  Msg temp_msg_;

  bool is_connected_ = false;
};

}
