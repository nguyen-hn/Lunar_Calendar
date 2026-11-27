#include "lunar/astro.hpp"
#include "lunar/calendar.hpp"

namespace lunar {

namespace {
bool is_leap_year(int yyyy) {
    return (yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0);
}

int get_lunar_month11(int yy, double tz) {
    int k, off, nm, sunLong;
    off = jdn_from_date(SolarDate{31, 12, yy}) - 2415021;
    k = static_cast<int>(off / 29.530588853);
    nm = new_moon_day(k, tz);
    sunLong = sun_sector(nm, tz); // sun longitude at local midnight
    if (sunLong >= 9) {
        nm = new_moon_day(k-1, tz);
    }
    return nm;
}

int get_leap_month_offset(int a11, double tz) {
    int k, last, arc, i;
    k = static_cast<int>((a11 - 2415021.076998695) / 29.530588853 + 0.5);
    last = 0;
    i = 1; // We start with the month following lunar month 11
    arc = sun_sector(new_moon_day(k+i, tz), tz);
    do {
        last = arc;
        i++;
        arc = sun_sector(new_moon_day(k+i, tz), tz);
    } while (arc != last && i < 14);
    return i-1;
}

CanChi year_canchi(int lunar_year) {
    int can_idx = ((lunar_year + 6) % 10 + 10) % 10;
    int chi_idx = ((lunar_year + 8) % 12 + 12) % 12;
    return CanChi{can_idx, chi_idx};
}

CanChi month_canchi(int lunar_year, int lunar_month) {
    int chi_idx = (lunar_month + 1) % 12;
    int year_can_idx = ((lunar_year + 6) % 10 + 10) % 10;
    int first_month_can_idx = ((year_can_idx % 5) * 2 + 2) % 10;
    int can_idx = (first_month_can_idx + (lunar_month - 1)) % 10;
    return CanChi{can_idx, chi_idx};
}

CanChi day_canchi(const SolarDate& solar_date) {
    int jdn = jdn_from_date(solar_date); 
    int can_idx = static_cast<int>(((jdn + 9) % 10 + 10) % 10);
    int chi_idx = static_cast<int>(((jdn + 1) % 12 + 12) % 12);
    return CanChi{can_idx, chi_idx};
}


}

bool is_valid(SolarDate d) {
#if 0
    using namespace std::chrono;
    year_month_day ymd{year{d.year}, month{d.month}, day{d.day}};
    return ymd.ok();
#else
    if (d.year < 1 || d.month < 1 || d.month > 12 || d.day < 1)
        return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(d.year)) {
        daysInMonth[1] = 29;
    }
    return d.day <= daysInMonth[d.month - 1];
#endif
}

std::optional<LunarDate> to_lunar(SolarDate d, double tz) {
    if (! is_valid(d) ) return std::nullopt;
    int k, dayNumber, monthStart, a11, b11, lunarDay, lunarMonth, lunarYear, lunarLeap;
    int yy = d.year;
    dayNumber = jdn_from_date(d);
    k = static_cast<int>((dayNumber - 2415021.076998695) / 29.530588853);
    monthStart = new_moon_day(k+1, tz);
    while (monthStart > dayNumber) {
        --k;
        monthStart = new_moon_day(k + 1, tz);
    }
    a11 = get_lunar_month11(yy, tz);
    b11 = a11;
    if (a11 >= monthStart) {
        lunarYear = yy;
        a11 = get_lunar_month11(yy-1, tz);
    } else {
        lunarYear = yy+1;
        b11 = get_lunar_month11(yy+1, tz);
    }
    lunarDay = dayNumber-monthStart+1;
    int diff = static_cast<int>((monthStart - a11)/29);
    lunarLeap = 0;
    lunarMonth = diff+11;
    if (b11 - a11 > 365) {
        int leapMonthDiff = get_leap_month_offset(a11, tz);
        if (diff >= leapMonthDiff) {
            lunarMonth = diff + 10;
            if (diff == leapMonthDiff) {
                lunarLeap = 1;
            }
        }
    }
    if (lunarMonth > 12) {
        lunarMonth = lunarMonth - 12;
    }
    if (lunarMonth >= 11 && diff < 4) {
        lunarYear -= 1;
    }
    return LunarDate{lunarDay, lunarMonth, lunarYear, lunarLeap != 0};
}

std::optional<SolarDate> to_solar(LunarDate d, double tz) {
    int k, a11, b11, off, leapOff, leapMonth, monthStart;
    int lunarDay = d.day, lunarMonth = d.month, lunarYear = d.year;
    bool lunarLeap = d.leap;
    if (lunarMonth < 11) {
        a11 = get_lunar_month11(lunarYear - 1, tz);
        b11 = get_lunar_month11(lunarYear, tz);
    } else {
        a11 = get_lunar_month11(lunarYear, tz);
        b11 = get_lunar_month11(lunarYear + 1, tz);
    }
    off = lunarMonth - 11;
    if (off < 0) {
        off += 12;
    }
    if (b11 - a11 > 365) {
        leapOff = get_leap_month_offset(a11, tz);
        leapMonth = leapOff - 2;
        if (leapMonth < 0) {
            leapMonth += 12;
        }
        if (lunarLeap != 0 && lunarMonth != leapMonth) {
            return std::nullopt;
        } else if (lunarLeap != 0 || off >= leapOff) {
            off += 1;
        }
    }
    if (b11 - a11 <= 365 && lunarLeap) return std::nullopt;
    k = static_cast<int>(std::floor(0.5 + (a11 - 2415021.076998695) / 29.530588853));
    monthStart = new_moon_day(k+off, tz);
    int nextMonthStart = new_moon_day(k + off + 1, tz);
    if (lunarDay < 1 || lunarDay > nextMonthStart - monthStart) return std::nullopt;
    return date_from_jdn(monthStart + lunarDay - 1);
}
}
