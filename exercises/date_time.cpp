/**
 * se utiliza la librería `HowardHinnant/date`
 * para suplir funciones como `std::chrono::parse`
 */

#include <chrono>
#include <date/date.h>
#include <fmt/core.h>

using namespace std;

namespace datetime {
    /// convierte un objeto `zoned_time` con zona
    /// horaria a un string en formato `YYYY-MM-DD HH:mm:ss`
    std::string format_zoned(auto zt)
    {
        auto local = zt.get_local_time();

        auto dp = floor<chrono::days>(local);

        chrono::year_month_day ymd{dp};

        chrono::hh_mm_ss time{local - dp};

        return fmt::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}", int(ymd.year()), unsigned(ymd.month()),
                           unsigned(ymd.day()), time.hours().count(), time.minutes().count(), time.seconds().count());
    }

    struct Date {
        std::chrono::year_month_day value;

        std::string to_string() const
        {
            // no funciona
            // return date::format("%F", value);
            // no funciona
            // return fmt::format("{}", date::format("%F", value));
            // funciona
            return date::format("%F", std::chrono::sys_days{value});
        }

        std::string get_year()
        {
            return fmt::format("{}", int(value.year()));
        }
        std::string get_month()
        {
            return fmt::format("{}", unsigned(value.month()));
        }
        std::string get_day()
        {
            return fmt::format("{}", unsigned(value.day()));
        }
    };

    struct Time {
        std::chrono::hh_mm_ss<std::chrono::seconds> value;

        std::string to_string() const
        {
            return fmt::format("{:02}:{:02}:{:02}", value.hours().count(), value.minutes().count(),
                               value.seconds().count());
        }
        std::string get_hour()
        {
            return fmt::format("{}", value.hours().count());
        }
        std::string get_min()
        {
            return fmt::format("{}", value.minutes().count());
        }
        std::string get_sec()
        {
            return fmt::format("{}", value.seconds().count());
        }
    };

    struct DateTime {
        std::chrono::sys_seconds value;

        std::string to_string() const
        {
            return fmt::format("{}", date::format("%F %T", value));
        }

        std::string to_string_tz(std::string_view tz) const
        {
            auto zone = std::chrono::locate_zone(std::string(tz));

            std::chrono::zoned_time zt{zone, value};

            // funciona
            // return date::format("%F %T", zt.get_sys_time());
            // falla hay que formatear
            // return date::format("%F %T", zt.get_local_time());
            // funciona
            return format_zoned(zt);
        }
    };

    Date parse_date(std::string_view s)
    {
        std::istringstream in{std::string{s}};

        std::chrono::sys_days dp;
        in >> date::parse("%F", dp);

        return Date{std::chrono::year_month_day{dp}};
    }

    Time parse_time(std::string_view s)
    {
        int h{};
        int m{};
        int sec{};

        std::sscanf(s.data(), "%d:%d:%d", &h, &m, &sec);

        using namespace std::chrono;

        return Time{hh_mm_ss{hours{h} + minutes{m} + seconds{sec}}};
    }

    DateTime parse_date_time(std::string_view s)
    {
        using namespace date;
        using namespace std::chrono;

        std::istringstream in{std::string{s}};

        sys_seconds tp;
        in >> date::parse("%F %T", tp);

        return DateTime{tp};
    }

} // namespace datetime

int main()
{
    fmt::println("manejo de fechas y horas");
    fmt::println("");
    auto dt = datetime::parse_date_time("2026-01-01 15:02:29");
    fmt::println("parse_date_time (UTC): {}", dt.to_string());
    fmt::println("parse_date_time Tokyo: {}", dt.to_string_tz("Asia/Tokyo"));
    fmt::println("parse_date_time Bogota: {}", dt.to_string_tz("America/Bogota"));
    fmt::println("parse_date_time London: {}", dt.to_string_tz("Europe/London"));
    fmt::println("");

    auto d = datetime::parse_date("2026-01-01");
    fmt::println("parse_date: {}", d.to_string());
    fmt::println("parse_date: YYYY:{} MM:{} DD:{}", d.get_year(), d.get_month(), d.get_day());
    fmt::println("");

    auto t = datetime::parse_time("15:02:29");
    fmt::println("parse_time: {}", t.to_string());
    fmt::println("parse_time: H:{} m:{} s:{}", t.get_hour(), t.get_min(), t.get_sec());
}
