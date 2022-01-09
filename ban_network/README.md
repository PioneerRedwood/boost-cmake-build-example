# ban_network
Making a server/client network using boost asio!

- Updated 2022-01-09
- The lobby is a little concept of meeting zone.
- `namespace ban` is Boost Asio Network.
- ver 0.1

## common
- lobby_session.h/cc: It works differently depending on who is the `Owner` of itself.
  - Owner: `SERVER`, `CLIENT`
- logger.hpp: Just logging on cmd console. It uses `time_util.hpp`.
- time_util.hpp: Getting a string of the current time.
  - TimeType: `DATE`, `TIME`, `DETAIL`
- tsdeque.hpp: [OLC's thread safe deque](https://github.com/OneLoneCoder/olcPixelGameEngine/tree/master/Videos/Networking/Parts1%262). This wraps the std::deque.

## client
- client_option.h: The options used in the client.
  - `SERVER_ADDRESS`, `PORT`, `TIMEOUT`
  - It can be replaced by a preprocessor in `CMakeLists.txt`
- lobby_client.h/cc: The Client side. 
  - Send `Heartbeating` message to the server every `TIMEOUT`(ms).
- client_session.h/cc: This is deprecated. Do not use this.

## server
- server_option.h: The options used in the server.
  - `SERVER_PORT`, `LOBBY_COUNT`, `NUM_PER_LOBBY` (The last of two variables not used in `ver0.1`)
  - It can be replaced by a preprocessor in `CMakeLists.txt`
- lobby_server.h/cc: The Server side.