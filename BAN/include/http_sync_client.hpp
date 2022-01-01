/**
 * @file http_sync_client.hpp
 * @author Chris Redwood (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace http_test {
class HttpSyncClient
{
public:
	static int Request() {
		try
		{
			// 실행은 확인했으나 응답 없음
			// www.google.com // public ip
			auto const host = "8.8.8.8";
			auto const port = "80";
			auto const target = "/\n";
			auto const version = 10;

			net::io_context context;
			tcp::resolver resolver(context);
			beast::tcp_stream stream(context);

			auto const results = resolver.resolve(host, port);
			stream.connect(results);

			http::request<http::string_body> req{ http::verb::get, target, version };
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			http::write(stream, req);

			beast::flat_buffer buffer;

			http::response<http::dynamic_body> res;
			http::read(stream, buffer, res);

			std::cout << res << "\n";

			beast::error_code ec;
			stream.socket().shutdown(tcp::socket::shutdown_both, ec);

			if (ec && ec != beast::errc::not_connected)
			{
				throw beast::system_error{ ec };
			}

		}
		catch (std::exception const& e)
		{
			std::cerr << "Error: " << e.what() << "\n";
			return -1;
		}

		return 0;
	}
};
} // http_test