/**
 * @file udp_server.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-06
 * 
 * @copyright Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 * https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio/tutorial/tutdaytime6/src.html
 */

#include <ctime>
#include <iostream>
#include <array>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using udp = asio::ip::udp;

namespace ban {
namespace {

std::string make_date_string() {
    time_t now = time(0);
    return ctime(&now);
}

class UdpServer {
public:

    UdpServer(asio::io_context& context) 
    : context_(context), 
    socket_(context, udp::endpoint(udp::v4(), 13)),
    strand_(context),
    timer_(context) 
    {}

    void Start() {
        Receive();
    }
private:
    void Tick() {
        timer_.expires_from_now(asio::chrono::milliseconds(delay_));
        timer_.async_wait(
            [this](const auto& error) {
                if(error) {
                    return;
                } else {
                    std::cout << "udp_server Tick()\n";
                    Tick();
                }
            }
        );
    }

    void Receive() {
        socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&UdpServer::HandleReceive, this,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    }

    void HandleReceive(const boost::system::error_code& error, std::size_t byte) {
        if(error) {
            return;
        } else {
            std::shared_ptr<std::string> message(
                new std::string(make_date_string()));
            socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                boost::bind(&UdpServer::HandleSend, this,
                    asio::placeholders::error,
                    asio::placeholders::bytes_transferred));
            
            Receive();
        }
    }

    void HandleSend(const boost::system::error_code& error, std::size_t byte) {
        if(error) {
            return;
        } else {
            // ..
        }
    }

private:
    asio::io_context& context_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;

    asio::io_context::strand strand_;

    asio::steady_timer timer_;
    const uint32_t delay_ = 1;

    std::array<char, 1> recv_buffer_;

};  // UdpServer    
}
}   // ban