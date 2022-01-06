
#include <boost/asio.hpp>

#include "message.hpp"
#include "tsdeque.hpp"

#if CLIENT
#include "client/lobby_client.h"
#endif

namespace io = boost::asio;
using tcp = io::ip::tcp;

template<typename T>
struct Message;

template<typename T>
struct OwnedMessage;

namespace ban {
class LobbySession : public std::enable_shared_from_this<LobbySession> {
public:
#if CLIENT
  LobbySession(io::io_context&, TSDeque<OwnedMessage<LobbyMsg>>&, uint32_t timeout);
  void Start(tcp::endpoint endpoint);
#else
  LobbySession(io::io_context&, tcp::socket, TSDeque<OwnedMessage<LobbyMsg>>& );
#endif
  
  void Start(uint32_t);
  void Start(std::time_t, uint32_t);
  
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
private:
  io::io_context& context_;
  tcp::socket socket_;

  TSDeque<OwnedMessage<LobbyMsg>>& read_deque_;
  TSDeque<Message<LobbyMsg>> write_deque_;
  Message<LobbyMsg> temp_msg_{};

  bool is_connected_ = false;
  // Session info
  uint32_t id_ = 0;

#if CLIENT
  LobbyClient* owner_;
  uint32_t timeout_;
#endif
};
}