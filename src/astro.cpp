#include "lunar/astro.hpp"
#include <cstdio>
#include <string>

namespace lunar {

constexpr double PI = 3.14159265358979323846;

double normalize_deg(double deg) {
    deg = std::fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

int jdn_from_date(SolarDate d) {
    int dd = d.day, mm = d.month, yy = d.year; 
    int a, y, m, jd;
    a = static_cast<int>((14 - mm) / 12);
    y = yy+4800-a;
    m = mm+12*a-3;
    jd = dd + static_cast<int>((153*m+2)/5) + 365*y + static_cast<int>(y/4) - static_cast<int>(y/100) + static_cast<int>(y/400) - 32045;
    if (jd < 2299161) {
        jd = dd + static_cast<int>((153*m+2)/5) + 365*y + static_cast<int>(y/4) - 32083;
    }
    return jd;
}

SolarDate date_from_jdn(int jd) {
    int a, b, c, d, e, m, day, month, year;
    if (jd > 2299160) { // After 5/10/1582, Gregorian calendar
        a = jd + 32044;
        b = static_cast<int>((4*a+3)/146097);
        c = a - static_cast<int>((b*146097)/4);
    } else {
        b = 0;
        c = jd + 32082;
    }
    d = static_cast<int>((4*c+3)/1461);
    e = c - static_cast<int>((1461*d)/4);
    m = static_cast<int>((5*e+2)/153);
    day = e - static_cast<int>((153*m+2)/5) + 1;
    month = m + 3 - 12*static_cast<int>(m/10);
    year = b*100 + d - 4800 + static_cast<int>(m/10);
    return SolarDate{day, month, year};
}

int new_moon_day(int k, double tz) {
    double T = k/1236.85;
    double T2 = T*T;
    double T3 = T2*T;
    double dr = PI/180;
    double deltat;
    double jd1 = 2415020.75933 + 29.53058868*k + 0.0001178*T2 - 0.000000155*T3;
    jd1 = jd1 + 0.00033*std::sin((166.56 + 132.87*T - 0.009173*T2)*dr); // mean new moon
    double M = 359.2242 + 29.10535608*k - 0.0000333*T2 - 0.00000347*T3; // sun's mean anomaly
    double Mpr = 306.0253 + 385.81691806*k + 0.0107306*T2 + 0.00001236*T3; // Moon's mean anomaly
    double F = 21.2964 + 390.67050646*k - 0.0016528*T2 - 0.00000239*T3; // Moon's argument of latitude
    double C1 = (0.1734 - 0.000393*T)*std::sin(M*dr) + 0.0021*std::sin(2*dr*M);
    C1 = C1 - 0.4068*std::sin(Mpr*dr) + 0.0161*std::sin(dr*2*Mpr);
    C1 = C1 - 0.0004*std::sin(dr*3*Mpr);
    C1 = C1 + 0.0104*std::sin(dr*2*F) - 0.0051*std::sin(dr*(M+Mpr));
    C1 = C1 - 0.0074*std::sin(dr*(M-Mpr)) + 0.0004*std::sin(dr*(2*F+M));
    C1 = C1 - 0.0004*std::sin(dr*(2*F-M)) - 0.0006*std::sin(dr*(2*F+Mpr));
    C1 = C1 + 0.0010*std::sin(dr*(2*F-Mpr)) + 0.0005*std::sin(dr*(2*Mpr+M));
    if (T < -11) {
        deltat= 0.001 + 0.000839*T + 0.0002261*T2 - 0.00000845*T3 - 0.000000081*T*T3;
    } else {
        deltat= -0.000278 + 0.000265*T + 0.000262*T2;
    };
    double JdNew = jd1 + C1 - deltat;
    return static_cast<int>(JdNew + 0.5 + tz/24);
}

double sun_longitude_deg(double jd_ut) {
    double T, T2, dr, M, L0, DL, L;
    T = (jd_ut - 2451545.0) / 36525; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
    T2 = T*T;
    dr = PI/180; // degree to radian
    M = 357.52910 + 35999.05030*T - 0.0001559*T2 - 0.00000048*T*T2; // mean anomaly, degree
    L0 = 280.46645 + 36000.76983*T + 0.0003032*T2; // mean longitude, degree
    DL = (1.914600 - 0.004817*T - 0.000014*T2)*std::sin(dr*M);
    DL = DL + (0.019993 - 0.000101*T)*std::sin(dr*2*M) + 0.000290*std::sin(dr*3*M);
    L = L0 + DL; // true longitude, degree
    // L = L*dr;
    // L = L - PI*2*(std::floor(L/(PI*2))); // Normalize to (0, 2*PI)
    L = normalize_deg(L);
    return L;
}

int sun_sector(int jdn, double tz) {
    return std::floor(sun_longitude_deg(jdn - 0.5 - tz/24) / 30);
}

double apparent_sun_longitude_deg(double jd) {
    double T = (jd - 2451545.0) / 36525;
    double T2 = T * T;
    double L0 = 280.46646 + 36000.76983 * T + 0.0003032 * T2;
    double M = 357.52910 + 35999.05029 * T - 0.0001537 * T2;
    double Mrad = M * PI/180.0;
    double C = (1.914602 - 0.004817 * T - 0.000014 * T2) * std::sin(Mrad) + (0.019993 - 0.000101*T) * std::sin(2*Mrad) + 0.000289 * std::sin(3*Mrad);
    double L = L0 + C;
    double o = 125.04 - 1934.136 * T;
    L = L - 0.00569 - 0.00478 * std::sin(o * PI / 180.0);
    L = normalize_deg(L);
    return L;
}

double term_instant(double target_deg, double jd_guess) {
    double jd = jd_guess;
    for (int i = 0; i < 6; ++i) {
        double d = target_deg - apparent_sun_longitude_deg(jd);
        d = std::fmod(d + 540.0, 360.0) - 180.0;
        jd += d / 0.98565;
        if (std::abs(d) < 1e-6) break;
    }
    return jd;
}

std::string jd_to_string(double jd, double tz = 7.0) {
    jd += tz / 24.0 + 0.5;
    int Z = static_cast<int>(std::floor(jd));
    double F = jd - Z;
    int A = Z;
    if (Z >= 2299161) {
        int alpha = static_cast<int>(std::floor((Z - 1867216.25) / 36524.25));
        A = Z + 1 + alpha - (alpha / 4);
    }
    int B = A + 1524;
    int C = static_cast<int>(std::floor((B - 122.1) / 365.25));
    int D = static_cast<int>(std::floor(365.25 * C));
    int E = static_cast<int>(std::floor((B - D) / 30.6001));

    double day_exact = B - D - std::floor(30.6001 * E) + F;
    int day = static_cast<int>(std::floor(day_exact));
    int month = (E < 14) ? (E - 1) : (E - 13);
    int year = (month > 2) ? (C - 4716) : (C - 4715);

    double rem_hours = (day_exact - day) * 24.0;
    int hour = static_cast<int>(std::floor(rem_hours));
    double rem_mins = (rem_hours - hour) * 60.0;
    int minute = static_cast<int>(std::floor(rem_mins));
    int second = static_cast<int>(std::round((rem_mins - minute) * 60.0));

    char buf[64];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    return std::string(buf);
}

} // namespace lunar
