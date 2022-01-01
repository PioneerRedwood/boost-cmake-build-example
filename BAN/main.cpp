#include "include/custom_timer.hpp"
#include "include/http_sync_client.hpp"
#include "boost/asio.hpp"

int main()
{
    boost::asio::io_context context;
    custom_timer::LittleTimer timer(context, static_cast<uint32_t>(1));

    timer.Start();

    http_test::HttpSyncClient::Request();

    context.run();

    return 0;
}