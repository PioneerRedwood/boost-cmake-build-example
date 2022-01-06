/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-06
 * 
 * @copyright Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 * https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio/tutorial/tutdaytime4/src.html
 */

#include <iostream>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main() {
    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint = 
        *resolver.resolve(udp::v4(), "127.0.0.1", "daytime").begin();
    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::array<char, 1> send_buf = {{0}};
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    std::array<char, 128> recv_buf;
    udp::endpoint send_endpoint;
    std::size_t len = 
        socket.receive_from(boost::asio::buffer(recv_buf), send_endpoint);
    
    std::cout.write(recv_buf.data(), len);
    
    return 0;
}