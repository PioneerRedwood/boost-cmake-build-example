#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>

namespace ban {
enum class TimeType {
    DATE = 0,
    TIME = 1,
    DETAIL = 2,
}

namespace time {
static void GetTime(std::string& result, ban::TimeType type) {
    using posix = boost::posix_time;
    posix::time_facet* pTime = nullptr;

    switch(type) {
    case TimeType::DATE: {
        pTime = new posix::time_facet("%Y-%m-%d");
        break;
    }
    case TimeType::TIME: {
        pTime = new posix::time_facet("%Y-%m-%d %H:%M:%S");
        break;
    }
    case TimeType::DETAIL: {
        pTime = new posix::time_facet("%Y-%m-%d %H:%M:%S.%f");
        break;
    }
    default: {
        break;
    }
    } // switch(TimeType)

    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), pTime));
    ss << posix::microsec_clock::local_time();

    result = date.str();
}
} // time
} // ban