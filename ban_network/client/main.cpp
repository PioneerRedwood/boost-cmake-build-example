#include <boost/asio.hpp>
#include "lobby_client.h"

int main() {
  boost::asio::io_context context;
  ban::LobbyClient client(context);
  
  client.Start();

  context.run();

  return 0;
}