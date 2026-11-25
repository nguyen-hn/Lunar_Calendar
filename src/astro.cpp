#include "astro.hpp"
#include <cmath>

namespace lunar {

constexpr double PI = 3.14159265358979323846;

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

double sun_longtitude_deg(int jdn, double tz) {
    return 0.0;
}

int sun_sector(int jdn, double tz) {
    double T, T2, dr, M, L0, DL, L;
    T = (jdn - 2451545.5 - tz/24) / 36525; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
    T2 = T*T;
    dr = PI/180; // degree to radian
    M = 357.52910 + 35999.05030*T - 0.0001559*T2 - 0.00000048*T*T2; // mean anomaly, degree
    L0 = 280.46645 + 36000.76983*T + 0.0003032*T2; // mean longitude, degree
    DL = (1.914600 - 0.004817*T - 0.000014*T2)*std::sin(dr*M);
    DL = DL + (0.019993 - 0.000101*T)*std::sin(dr*2*M) + 0.000290*std::sin(dr*3*M);
    L = L0 + DL; // true longitude, degree
    L = L*dr;
    L = L - PI*2*(std::floor(L/(PI*2))); // Normalize to (0, 2*PI)
    return std::floor(L / PI * 6);
}

} // namespace lunar
