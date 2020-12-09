#include "Log.h"
#include <iomanip>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

void init_log(const std::string &file, const std::string &state) {
  if (state == "release") {
    logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );
  }

  if (!file.empty()) {
    logging::add_file_log
        (
            keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation =
                sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format =
                (
                    expr::stream
                        << std::hex << std::setw(8) << std::setfill('0') << expr::attr<unsigned int>("LineID")
                        << "\t"
                        << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f")
                        << "\t:"
                        << " <" << logging::trivial::severity << "> "
                        << "\t"
                        << expr::smessage
                )
        );
  }
}