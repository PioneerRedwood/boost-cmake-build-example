#include <iostream>
#include <boost/asio.hpp>
#include "lobby_server.h"

#ifndef SERVER
#define SERVER
#undef CLIENT
#endif

int main() {
  boost::asio::io_context context;
  ban::LobbyServer server(context);
  server.Start();

  while(true) {
    server.Tick(-1, true);
  }

  return 0;
}