#include "lunar/format.hpp"
#include <ostream>
#include <iostream>
#include <cstdio>
#include <charconv>

namespace lunar {

namespace {
template<typename T>
std::optional<T> parse_date(std::string_view s) {
    static_assert(std::is_same_v<T, lunar::SolarDate> || std::is_same_v<T, lunar::LunarDate>,
        "T must be SolarDate or LunarDate");

    const char* ptr = s.data();
    const char* end = s.data() + s.size();
    auto num = [&](int& out) {
        auto r = std::from_chars(ptr, end, out);
        if (r.ec != std::errc{}) return false;
        ptr = r.ptr;
        return true;
    };
    auto sep = [&] {
        if (ptr == end || (*ptr != '-' && *ptr != '/')) return false;
        ++ptr;
        return true;
    };

    int day = 0, month = 0, year = 0;
    bool isLeap = false;
    if (!num(day) || !sep() || !num(month)) return std::nullopt;
    if (ptr < end && (*ptr == 'L' || *ptr == 'l')) {
        isLeap = true;
        ++ptr;
    }
    if (!sep() || !num(year) || ptr != end) return std::nullopt;

    if constexpr (std::is_same_v<T, lunar::SolarDate>) {
        if (isLeap) {
            return std::nullopt;
        }
        return lunar::SolarDate{day, month, year};
    } else if constexpr (std::is_same_v<T, lunar::LunarDate>) {
        return lunar::LunarDate{day, month, year, isLeap};
    } else {}
}
}

std::string to_string(const SolarDate& solar_date) {
    char buf[32];
    const char* fmt = "%02d/%02d/%04d";
    int len = std::snprintf(buf, sizeof(buf), fmt, solar_date.day, solar_date.month, solar_date.year);
    if (len <= 0) return {};
    return std::string(buf, len);
}

std::string to_string(const LunarDate& lunar_date) {
    char buf[32];
    const char* fmt = (lunar_date.leap) ? "%02d/%02dL/%04d" : "%02d/%02d/%04d";
    int len = std::snprintf(buf, sizeof(buf), fmt, lunar_date.day, lunar_date.month, lunar_date.year);
    if (len <= 0) return {};
    return std::string(buf, len);
}

std::string to_string(const SolarTerm& solar_term) {
    return std::string(solar_term.name) + ": " + to_string(solar_term.date);
}

std::ostream& operator<<(std::ostream& out, const SolarDate& solar_date) {
    out << to_string(solar_date);
    return out;
}

std::ostream& operator<<(std::ostream& out, const LunarDate& lunar_date) {
    out << to_string(lunar_date);
    return out;
}

std::ostream& operator<<(std::ostream& out, const SolarTerm& solar_term) {
    out << to_string(solar_term);
    return out;
}

std::optional<SolarDate> parse_solar(std::string_view str) {
    return parse_date<SolarDate>(str);
}

std::optional<LunarDate> parse_lunar(std::string_view str) {
    return parse_date<LunarDate>(str);
}

}
