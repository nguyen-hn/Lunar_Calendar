#include <iostream>
#include <chrono>
#include <ctime>
#include <type_traits>
#include <charconv>
#include <argparse/argparse.hpp>
#include "lunar/calendar.hpp"
#include "lunar/solar_terms.hpp"
#include "lunar/format.hpp"

namespace {
lunar::SolarDate today() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_local{};
    localtime_r(&t, &tm_local);
    return lunar::SolarDate{tm_local.tm_mday, tm_local.tm_mon + 1, tm_local.tm_year + 1900};
}


}

int main(int argc, char* argv[]) {
    auto current_day = today();
    const int default_year = current_day.year;

    argparse::ArgumentParser program("lunar-cli", "0.1");
    program.add_description("Vietnamese Lunar Calendar CLI");

    auto& mode_group = program.add_mutually_exclusive_group();

    mode_group.add_argument("-t", "--today")
        .help("print today's Vietnamese lunar date")
        .default_value(false).implicit_value(true);

    mode_group.add_argument("-s", "--solar")
        .metavar("dd-mm-yyyy")
        .help("convert Gregorian (solar) date to Vietnamese's lunar date");

    mode_group.add_argument("-l", "--lunar").metavar("dd-mm[L]-yyyy")
        .help("convert Vietnamese lunar date to Gregorian date");

    mode_group.add_argument("-H", "--holidays")
        .help("list holidays/festivals for a given year")
        .default_value(false).implicit_value(true);

    mode_group.add_argument("-S" ,"--solarterm").metavar("YEAR")
        .help("print solar terms for a given year (defaults to current year)")
        .scan<'i', int>()
        .default_value(default_year)
        .nargs(argparse::nargs_pattern::optional);

    program.add_argument("-y", "--year").metavar("YEAR")
        .help("target year (used with --holidays)")
        .scan<'i', int>()
        .default_value(default_year);

    program.add_argument("--tz")
        .metavar("OFFSET")
        .help("timezone offset in hours (default: 7.0 for UTC+7)")
        .scan<'g', double>()
        .default_value(7.0);

    try {
        program.parse_args(argc, argv);
        const double tz = program.get<double>("--tz");

        if (program.is_used("--solar")) {
            const std::string solar_str = program.get<std::string>("--solar");
            auto solar_date = lunar::parse_solar(solar_str);
            if ( solar_date.has_value() ) {
                auto lunar_date = lunar::to_lunar(*solar_date, tz);
                if (lunar_date.has_value()) {
                    std::cout << *lunar_date << std::endl;
                } else {
                    std::cerr << "Cannot convert to lunar date\n";
                    return 1;
                }
            } else {
                std::cerr << "Cannot convert to lunar date\n";
                return 1;
            }
        } else if (program.is_used("--lunar")) {
            const std::string lunar_str = program.get<std::string>("--lunar");
            auto d = lunar::parse_lunar(lunar_str);
            if (d.has_value()) {
                lunar::LunarDate lunar_date = *d;
                auto solar_date = lunar::to_solar(lunar_date, tz);
                if (solar_date.has_value()) {
                    std::cout << *solar_date << std::endl;
                } else {
                    std::cerr << "cannot convert to solar date\n";
                    return 1;
                }
            } else {
                std::cerr << "wrong format date\n";
                return 1;
            }
        } else if (program.is_used("--holidays")) {
            const int year = program.get<int>("--year");
        } else if (program.is_used("--solarterm")) {
            const int year = program.get<int>("--solarterm");
            auto t = lunar::solar_term(year, tz);
            for(auto d: t) {
                std::cout << d << std::endl;
            }
        } else {
            std::cout << "Current Date: " << lunar::to_string(current_day) << std::endl;
            auto l = lunar::to_lunar(today(), tz);
            if (l.has_value()) {
                std::cout << *l << std::endl;
            } else {
                std::cerr << "Something is wrong...\n";
                return 1;
            }
        }

    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    return 0;
}
