#include <iostream>
#include "boost/asio.hpp"

/**
 * @brief boost::asio::deadline_timer 기반 작은 타이머 래퍼
 * - CMake 빌드 시스템 예시로 만듬
 */
namespace custom_timer {
class LittleTimer
{
public:
    LittleTimer(boost::asio::io_context& context, uint32_t delay)
        : context_(context), timer_(context), delay_(delay)
    {}

    void Start()
    {
        Tick();
    }

private:
    void Tick() {
        // when timer execute 10 times, exit
        if (count_++ > 10) {
            return;
        }

        timer_.expires_from_now(boost::posix_time::seconds(delay_));
        timer_.async_wait(
            [&](auto& ec) {
                if (ec) {
                    return;
                }

                std::cout << "hello, new year !\n";

                // call it again
                Tick();
            }
        );
    }
private:
    boost::asio::io_context& context_;
    boost::asio::deadline_timer timer_;
    uint32_t delay_;
    uint32_t count_ = 0;
};

} // custom timer