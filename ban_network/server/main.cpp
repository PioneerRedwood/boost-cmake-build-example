/**
 * @file main.cpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <boost/asio.hpp>
#include "lobby_server.h"

int main() {
  boost::asio::io_context context;
  ban::LobbyServer server(context);
  server.Start();

  while(true) {
    server.Tick(-1, true);
  }

  return 0;
}