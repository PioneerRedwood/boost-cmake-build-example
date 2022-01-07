#pragma once
#include <boost/asio.hpp>

#include "common/lobby_session.h"
#include "common/logger.hpp"
#include "common/message.hpp"
#include "common/tsdeque.hpp"

namespace io = boost::asio;
using tcp = io::ip::tcp;

namespace ban {

class LobbyClient {
public:
	class ClientSession : public LobbySession {
	public:
		ClientSession(io::io_context&, std::weak_ptr<LobbyClient>, TSDeque<OwnedMessage<LobbyMsg>>&);
		void Start(tcp::endpoint endpoint);

	private:
		std::weak_ptr<LobbyClient> owner_;
	};

public:
  LobbyClient(io::io_context&);
  ~LobbyClient();

  void Start();
  void Heartbeat();

private:
  io::io_context& context_;
  std::shared_ptr<ClientSession> conn_;
  
  TSDeque<OwnedMessage<LobbyMsg>> read_deque_;
  io::steady_timer timer_;

  std::thread thread_;
};
}