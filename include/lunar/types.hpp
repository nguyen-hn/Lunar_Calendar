#ifndef LUNAR_TYPES_HPP_
#define LUNAR_TYPES_HPP_ 

#include <vector>
#include <array>
#include <string_view>
#include <optional>
#include <cmath>
#include <initializer_list>

namespace lunar {
struct SolarDate {
    int day, month, year;
    bool operator==(const SolarDate& rhs) const {
        return ( day == rhs.day) && (month == rhs.month) && (year == rhs.year);
    }
};

struct LunarDate {
    int day, month, year;
    bool leap;
    bool operator==(const LunarDate& rhs) const {
        return ( day == rhs.day) && (month == rhs.month) && (year == rhs.year) && (leap == rhs.leap);
    }
};

struct CanChi {
    int can_idx, chi_idx;
};

}

#endif // LUNAR_TYPES_HPP_
