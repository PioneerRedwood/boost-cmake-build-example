/**
 * @file main.cpp
 * @version 0.1
 * @date 2022-01-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <thread>
#include <boost/asio.hpp>

#include "lobby_client.h"
#include "common/logger.hpp"

int main() {
  boost::asio::io_context context;
  ban::LobbyClient client(context);
  
  client.Start();
  while (true) {
    client.Tick();
  }

  return 0;
}