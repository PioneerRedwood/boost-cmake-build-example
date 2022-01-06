#include <boost/asio.hpp>
#include "udp_server.hpp"

int main() {
    boost::asio::io_context context;
    ban::UdpServer server(context);
    server.Start();
    context.run();

    return 0;
}