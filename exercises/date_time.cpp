/**
 * se utiliza la librería `HowardHinnant/date`
 * para suplir funciones como `chrono::parse`
 */

#include <chrono>
#include <date/date.h>
#include <fmt/chrono.h>
#include <fmt/core.h>

using namespace std;

namespace datetime {
    /// convierte un objeto `time_point` con zona
    /// horaria a un string en formato `YYYY-MM-DD HH:mm:ss`
    string format_tp(auto tp)
    {
        auto dp = floor<chrono::days>(tp);

        chrono::year_month_day ymd{dp};

        chrono::hh_mm_ss time{tp - dp};

        return fmt::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}", int(ymd.year()), unsigned(ymd.month()),
                           unsigned(ymd.day()), time.hours().count(), time.minutes().count(), time.seconds().count());
    }

    /// convierte un objeto `zoned_time` con zona
    /// horaria a un string en formato `YYYY-MM-DD HH:mm:ss`
    string format_zoned(auto zt)
    {
        auto local = zt.get_local_time();

        auto dp = floor<chrono::days>(local);

        chrono::year_month_day ymd{dp};

        chrono::hh_mm_ss time{local - dp};

        return fmt::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}", int(ymd.year()), unsigned(ymd.month()),
                           unsigned(ymd.day()), time.hours().count(), time.minutes().count(), time.seconds().count());
    }

    struct Date {
        chrono::year_month_day value;

        string to_string() const
        {
            // no funcionan
            // return date::format("%F", value);
            // return fmt::format("{}", date::format("%F", value));
            return date::format("%F", chrono::sys_days{value});
        }

        string get_year()
        {
            return fmt::format("{}", int(value.year()));
        }
        string get_month()
        {
            return fmt::format("{}", unsigned(value.month()));
        }
        string get_day()
        {
            return fmt::format("{}", unsigned(value.day()));
        }
    };

    struct Time {
        chrono::hh_mm_ss<chrono::seconds> value;

        string to_string() const
        {
            return fmt::format("{:02}:{:02}:{:02}", value.hours().count(), value.minutes().count(),
                               value.seconds().count());
        }

        string get_hour()
        {
            return fmt::format("{}", value.hours().count());
        }
        string get_min()
        {
            return fmt::format("{}", value.minutes().count());
        }
        string get_sec()
        {
            return fmt::format("{}", value.seconds().count());
        }
    };

    struct DateTime {
        chrono::sys_seconds value;

        string to_string() const
        {
            return fmt::format("{}", date::format("%F %T", value));
        }

        string to_string_tz(string_view tz) const
        {
            auto zone = chrono::locate_zone(string(tz));

            chrono::zoned_time zt{zone, value};

            // funciona pero maneja UTC
            // return date::format("%F %T", zt.get_sys_time());
            // falla hay que formatear
            // return date::format("%F %T", zt.get_local_time());
            return format_zoned(zt);
        }
    };

    Date parse_date(string_view s)
    {
        istringstream in{string{s}};

        chrono::sys_days dp;
        in >> date::parse("%F", dp);

        return Date{chrono::year_month_day{dp}};
    }

    Time parse_time(string_view s)
    {
        int h{};
        int m{};
        int sec{};

        sscanf(s.data(), "%d:%d:%d", &h, &m, &sec);

        return Time{chrono::hh_mm_ss{chrono::hours{h} + chrono::minutes{m} + chrono::seconds{sec}}};
    }

    DateTime parse_date_time(string_view s)
    {

        istringstream in{string{s}};

        chrono::sys_seconds tp;
        in >> date::parse("%F %T", tp);

        return DateTime{tp};
    }

} // namespace datetime

int main()
{
    fmt::println("manejo de fechas y horas");
    fmt::println("");

    // El instante es absoluto y no tiene zona horaria asociada.
    auto now = chrono::system_clock::now();

    //
    auto timestamp = duration_cast<chrono::seconds>(now.time_since_epoch()).count();
    fmt::println("timestamp(unix): {}", timestamp);
    fmt::println("");

    //
    fmt::println("===sumar dias===");
    auto the_date = chrono::sys_days{2026y / chrono::January / 1};
    fmt::println("the_date: {:%F}", the_date);

    auto next_week = the_date + chrono::days{7};
    fmt::println("next_week: {:%F}", next_week);
    fmt::println("");

    //
    fmt::println("===restar dias===");
    auto yesterday = the_date - chrono::days{1};
    fmt::println("yesterday: {:%F}", yesterday);
    fmt::println("");

    //
    fmt::println("===diferencia entre fechas===");
    auto start_date = chrono::sys_days{2026y / chrono::February / 28};
    auto end_date = chrono::sys_days{2026y / chrono::March / 2};
    fmt::println("start date: {:%F}", start_date);
    fmt::println("end date: {:%F}", end_date);

    auto diff = end_date - start_date;

    fmt::println("diferencia: {} días", diff.count());
    fmt::println("");

    //
    fmt::println("===comparar fechas===");
    if (start_date < now) {
        fmt::println("start_date sin formato: ({})", start_date);
        fmt::println("start_date: ({:%F}): tiempo expirado", start_date);
    }
    fmt::println("");

    //
    // para hora local, se necesitas la base de datos IANA de zonas horarias.
    fmt::println("===con Timezone por defecto===");
    auto zone = chrono::current_zone();
    chrono::time_point local = zone->to_local(now);
    fmt::println("{}", datetime::format_tp(local));
    fmt::println("");

    //
    chrono::zoned_time current_zt{zone, now};
    fmt::println("zoned_time: {}", datetime::format_zoned(current_zt));
    fmt::println("");

    //
    fmt::println("===definiendo Timezones===");
    auto madrid_zone = chrono::locate_zone("Europe/Madrid");

    chrono::time_point madrid = madrid_zone->to_local(now);
    fmt::println("Europe/Madrid Timezone: {}", datetime::format_tp(madrid));
    fmt::println("");

    // convertir hora local a UTC
    auto utc = madrid_zone->to_sys(madrid);
    fmt::println("Europe/Madrid Timezone to UTC: {}", utc);
    fmt::println("");

    //
    // usando `chrono::zoned_time` directamente
    fmt::println("==usando `chrono::zoned_time` directamente==");
    auto madrid_zt = chrono::zoned_time{"Europe/Madrid", now};

    // Hora local Madrid
    fmt::println("Europe/Madrid local: {}", datetime::format_zoned(madrid_zt));

    // UTC
    utc = madrid_zt.get_sys_time();
    fmt::println("Europe/Madrid Timezone to UTC: {}", utc);
    fmt::println("");

    //
    fmt::println("===conversión entre Zonas Horarias===");
    // La clave es recordar que la conversión siempre pasa por `sys_time` (UTC),
    // y no de `local_time` a `local_time` directamente.
    auto london_zone = chrono::locate_zone("Europe/London");
    auto london_local = london_zone->to_local(now);
    fmt::println("Europe/Madrid local: {}", datetime::format_tp(madrid));
    fmt::println("Europe/London local: {}", datetime::format_tp(london_local));
    fmt::println("");

    //
    fmt::println("===parse_date===");
    auto d = datetime::parse_date("2026-01-01");
    fmt::println("parse_date: {}", d.to_string());
    fmt::println("parse_date: YYYY:{} MM:{} DD:{}", d.get_year(), d.get_month(), d.get_day());
    fmt::println("");

    //
    fmt::println("===parse_time===");
    auto t = datetime::parse_time("15:02:29");
    fmt::println("parse_time: {}", t.to_string());
    fmt::println("parse_time: HH:{} mm:{} ss:{}", t.get_hour(), t.get_min(), t.get_sec());
    fmt::println("");

    //
    fmt::println("===parse_date_time===");
    auto dt = datetime::parse_date_time("2026-01-01 15:02:29");
    fmt::println("parse_date_time (UTC): {}", dt.to_string());
    fmt::println("parse_date_time Tokyo: {}", dt.to_string_tz("Asia/Tokyo"));
    fmt::println("parse_date_time Bogota: {}", dt.to_string_tz("America/Bogota"));
    fmt::println("parse_date_time London: {}", dt.to_string_tz("Europe/London"));
    fmt::println("");
}
