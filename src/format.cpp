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

std::string zodiac_sign(const SolarDate& solar) {
    const int m = solar.month;
    const int d = solar.day;

    if ((m == 1 && d >= 20) || (m == 2 && d <= 18)) return "Bảo Bình ♒";
    if ((m == 2 && d >= 19) || (m == 3 && d <= 20)) return "Song Ngư ♓";
    if ((m == 3 && d >= 21) || (m == 4 && d <= 19)) return "Bạch Dương ♈";
    if ((m == 4 && d >= 20) || (m == 5 && d <= 20)) return "Kim Ngưu ♉";
    if ((m == 5 && d >= 21) || (m == 6 && d <= 21)) return "Song Tử ♊";
    if ((m == 6 && d >= 22) || (m == 7 && d <= 22)) return "Cự Giải ♋";
    if ((m == 7 && d >= 23) || (m == 8 && d <= 22)) return "Sư Tử ♌";
    if ((m == 8 && d >= 23) || (m == 9 && d <= 22)) return "Xử Nữ ♍";
    if ((m == 9 && d >= 23) || (m == 10 && d <= 23)) return "Thiên Bình ♎";
    if ((m == 10 && d >= 24) || (m == 11 && d <= 22)) return "Bọ Cạp ♏";
    if ((m == 11 && d >= 23) || (m == 12 && d <= 21)) return "Nhân Mã ♐";
    return "Ma Kết ♑";
}

constexpr std::array<const char*, 10> kCan = {
    "Giáp", "Ất", "Bính", "Đinh", "Mậu",
    "Kỷ", "Canh", "Tân", "Nhâm", "Quý"
};

constexpr std::array<const char*, 12> kChi = {
    "Tý", "Sửu", "Dần", "Mão", "Thìn", "Tỵ",
    "Ngọ", "Mùi", "Thân", "Dậu", "Tuất", "Hợi"
};

constexpr std::array<const char*, 7> kWeekdayFull = {
    "Chủ nhật", "Thứ hai", "Thứ ba", "Thứ tư",
    "Thứ năm", "Thứ sáu", "Thứ bảy"
};

std::string weekday_name(const SolarDate& solar_date) {
    int jdn = jdn_from_date(solar_date);
    int idx = static_cast<int>(((jdn + 1) % 7 + 7) % 7);
    return std::string{kWeekdayFull[idx]};
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

std::string to_string(const CanChi& canchi) {
    return std::string{kCan[canchi.can_idx]} + " " + std::string{kChi[canchi.chi_idx]};
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

void print_fulldate(const SolarDate& solar_date) {
    std::cout << weekday_name(solar_date) << ", " << to_string(solar_date) << " " << zodiac_sign(solar_date) << std::endl;
}

void print_canchi(std::vector<CanChi>& canchi_arr) {
    std::cout << "(";
    for(auto c: canchi_arr) {
        std::cout << to_string(c) << " ";
    }
    std::cout << ")" << std::endl;
}

//void print_fulldate(const LunarDate& lunar_date) { 
//    std::cout << to_string(lunar_date) << " (" 
//        //<< to_string(day_canchi()) << ", " 
//        << to_string(month_canchi(lunar_date.year, lunar_date.month)) << ", "
//        << to_string(year_canchi(lunar_date.year)) << ")" << std::endl;
//}

}
