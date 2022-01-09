# ban_network
Making a server/client network using boost asio!
The lobby is a little concept of meeting zone.
`namespace ban` is Boost Asio Network.

## common
- lobby_session.h/cc: It works differently depending on who is the `Owner` of itself.
  - Owner: `SERVER`, `CLIENT`
- logger.hpp: Just logging on cmd console. It uses `time_util.hpp`.
- time_util.hpp: Getting a string current time.
  - TimeType: `DATE`, `TIME`, `DETAIL`
- tsdeque.hpp: [OLC's thread safe deque](https://github.com/OneLoneCoder/olcPixelGameEngine/tree/master/Videos/Networking/Parts1%262). This is wraps the std::deque.

## client
- client_option.h: Some options used in the client.
  - `SERVER_ADDRESS`, `PORT`, `TIMEOUT`
- lobby_client.h/cc: The Client side. 
  - `Heartbeating`
- client_session.h/cc: This is deprecated. Do not use this.

## server
- server_option.h: Some options used in the server.
  - `SERVER_PORT`, `LOBBY_COUNT`, `NUM_PER_LOBBY` (The last of two variables not used in `ver0.1`)
- lobby_server.h/cc: The Server side.