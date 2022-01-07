/*
* https://www.boost.org/doc/libs/1_78_0/doc/html/date_time/posix_time.html#ptime_header
*/
#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace ban {
enum class TimeType {
    DATE = 0,
    TIME = 1,
    DETAIL = 2,
};

namespace time {
static void GetTime(std::string& result, ban::TimeType type) {
    boost::posix_time::time_facet* pTime = nullptr;

    switch(type) {
    case TimeType::DATE: {
        pTime = new boost::posix_time::time_facet("%Y-%m-%d");
        break;
    }
    case TimeType::TIME: {
        pTime = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
        break;
    }
    case TimeType::DETAIL: {
        pTime = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S.%f");
        break;
    }
    default: {
        break;
    }
    } // switch(TimeType)

    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), pTime));
    ss << boost::posix_time::microsec_clock::local_time();

    result = ss.str();
}
} // time
} // ban