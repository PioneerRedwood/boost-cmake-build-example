#pragma once

#include "common/time_util.hpp"

namespace ban {
namespace log {
static void Logging(const char* data, ...) {
    using posix = boost::posix_time;
    
    char buffer[512] = { 0, };
    va_list argueList;
    va_start(argueList, data);
    vsprintf_s(buffer, data, argueList);
    va_end(argueList);

    std::string time_str;
    ban::time::GetTime(time_str, ban::TimeType::DETAIL);

    std::cout << time_str << " " << buffer << "\n";
}

} // log
} // ban