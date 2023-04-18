#include "server.h"

static void init_log() {
    static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");

    boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
    boost::log::add_file_log (
            boost::log::keywords::file_name = "server_%3N.log",
            boost::log::keywords::rotation_size = 1 * 1024 * 1024,
            boost::log::keywords::max_size = 20 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
            boost::log::keywords::format = COMMON_FMT,
            boost::log::keywords::auto_flush = true
    );

    boost::log::add_common_attributes();
}
int main(int argc, char* argv[]) {
    init_log();
    try
    {
        if(argc == 2) {
            Server server;
            server.run(std::stoi(argv[1]));
        } else if (argc > 2) {
            BOOST_LOG_TRIVIAL(fatal) << "Invalid starting parameters!";
            throw std::invalid_argument("ERROR: <INCORRECT PARAMETERS NUMBER>");
        } else {
            Server server;
            BOOST_LOG_TRIVIAL(warning) << "Server running on default port";
            server.run();
        }
    } catch (std::exception& error) {
        std::cerr <<error.what() << "\n";
        return 1;
    }
    return 0;
}