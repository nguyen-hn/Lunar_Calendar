#include <iostream>
#include <chrono>
#include <sstream>
#include <ctime>
#include <argparse/argparse.hpp>
#include "lunar/calendar.hpp"
#include "lunar/solar_terms.hpp"

namespace {
int current_year() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_local{};
    localtime_r(&t, &tm_local);
    return tm_local.tm_year + 1900;
}

void parse_date_str(const std::string& date_str) {
    int day, month, year;
    char sep1, sep2;
    std::stringstream ss(date_str);
    if (ss >> day >> sep1 >> month >> sep2 >> year && sep1 == '/' && sep2 == '/') {
    
    } else {
        std::cerr << "Invalid date format\n";
    }
}

}

int main(int argc, char* argv[]) {
    const int default_year = current_year();

    argparse::ArgumentParser program("lunar-cli", "0.1");
    program.add_description("Vietnamese Lunar Calendar CLI");

    auto& mode_group = program.add_mutually_exclusive_group();

    program.add_argument("-t", "--today")
        .help("print today's Vietnamese lunar date")
        .default_value(false).implicit_value(true);
    
    program.add_argument("-s", "--solar")
        .metavar("dd-mm-yyyy")
        .help("convert Gregorian (solar) date to Vietnamese's lunar date");

    program.add_argument("-l", "--lunar").metavar("dd-mm-yyyy")
        .help("convert Vietnamese lunar date to Gregorian date");
    
    program.add_argument("-H", "--holidays")
        .help("list holidays/festivals for a given year")
        .default_value(false).implicit_value(true);

    program.add_argument("--leap")
        .help("treat the lunar month in --lunar as a leap month")
        .default_value(false)
        .implicit_value(true);
    
    program.add_argument("-S" ,"--solarterm").metavar("YEAR")
        .help("print solar terms for a given year (defaults to current year)")
        .scan<'i', int>()
        .default_value(default_year)
        .nargs(argparse::nargs_pattern::optional);
    
    program.add_argument("--tz")
        .metavar("OFFSET")
        .help("timezone offset in hours (default: 7.0 for UTC+7)")
        .scan<'g', double>()
        .default_value(7.0);

    try {
        program.parse_args(argc, argv);
        const double tz = program.get<double>("--tz");
        const bool is_leap = program.get<bool>("--leap");

        if (program.is_used("--solar")) {
            const std::string solar_str = program.get<std::string>("--solar");
        } else if (program.is_used("--lunar")) {
            const std::string lunar_str = program.get<std::string>("--lunar");
        } else if (program.is_used("--holidays")) {
            const int year = program.get<int>("--year");
        } else if (program.is_used("--solarterm")) {
            const int year = program.get<int>("--solarterm");
            auto t = lunar::solar_term(year, tz);
            for(auto d: t) {
                std::cout << d.name << " " << d.date.day << " - " << d.date.month << " - " << d.date.year << std::endl;
            }
        } else {
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            char buffer[80];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
            std::cout << "Current Date: " << buffer << std::endl;
            auto l = lunar::to_lunar(lunar::SolarDate{1900 + localTime->tm_year, 1 + localTime->tm_mon, localTime->tm_mday}, tz);
            std::cout << l.day << "/" << l.month << "/" << l.year << std::endl;
        }

    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    return 0;
}
