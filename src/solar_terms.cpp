#include "lunar/solar_terms.hpp"
#include "lunar/astro.hpp"

namespace lunar {

namespace {
constexpr std::array<std::string_view, 24> kTermNames = {
    "Tiểu Hàn", "Đại Hàn", "Lập Xuân", "Vũ Thủy", "Kinh Trập", "Xuân Phân", 
    "Thanh Minh", "Cốc Vũ", "Lập Hạ",   "Tiểu Mãn", "Mang Chủng", "Hạ Chí",
    "Tiểu Thử",   "Đại Thử",    "Lập Thu",    "Xử Thử",     "Bạch Lộ",    "Thu Phân",   
    "Hàn Lộ",   "Sương Giáng",  "Lập Đông", "Tiểu Tuyết", "Đại Tuyết", "Đông Chí", };
}

std::array<SolarTerm, 24> solar_term(int year, double tz) {
    std::array<SolarTerm, 24> terms;
    double jd_guess = jdn_from_date(lunar::SolarDate{1, 1, year});
    for (int i = 0; i < 24; i++) {
        int k = (19 + i)%24;
        double target_deg = k * 15.0;
        double jd_exact = term_instant(target_deg, jd_guess);
        SolarDate solar_date = date_from_jdn(static_cast<int>(std::floor(jd_exact + tz/24 + 0.5)));
        terms[i] = { k, kTermNames[i], solar_date };
        jd_guess = jd_exact + 15.2;
    }
    return terms;
}

} // namespace lunar
