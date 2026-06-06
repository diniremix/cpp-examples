/*
 * A pure C++23 implementation of Haversine formula
 * Based on Bartek Górny implementation and others.
 *
 * Author: Jorge Brunal Perez <diniremix@gmail.com>
 * [Oxide/haversine](https://gitlab.com/HomeInside/Oxide/-/blob/master/src/utils/haversine.rs)
 * commit: bfbe832323f6
 *
 * ## Examples
 * How to use:
 *
 * ```cpp
 * Coords origin{51.5223337, -0.7198055};
 * Coords destiny{51.5226574, -0.7219567};
 *
 * auto result_in_kms = haversine(origin, destiny, "kms");
 * fmt::println("result in kms: {:.3f}", *result_in_kms);
 *
 * // Otherwise
 * auto result_in_mts = haversine(origin, destiny, "mts");
 * fmt::println("result in mts: {:.3f}", *result_in_mts);
 * ```
 */

#include <cmath>
#include <expected>
#include <fmt/core.h>
#include <numbers>
#include <string>
// #include <vector>

struct Coords {
    double lat;
    double lon;
};

// static std::expected<double, std::string> haversine(std::vector<double> origin, std::vector<double> destiny,
static std::expected<double, std::string> haversine(Coords origin, Coords destiny, std::string_view type_measure)
{
    constexpr double RADIUS = 6371;
    constexpr double RAD = std::numbers::pi / 180;
    constexpr double SECOND = 2;

    /*
    // using `std::vector<double> origin, std::vector<double> destiny`
    //
    double orig_lat = origin[0];
    double orig_lon = origin[1];
    double dest_lat = destiny[0];
    double dest_lon = destiny[1];
    */

    double orig_lat = origin.lat;
    double orig_lon = origin.lon;
    double dest_lat = destiny.lat;
    double dest_lon = destiny.lon;

    double delta_lat = (dest_lat - orig_lat) * RAD;
    double delta_lon = (dest_lon - orig_lon) * RAD;
    double rad_orig_lat = orig_lat * RAD;
    double rad_dest_lat = dest_lat * RAD;

    double a = std::pow(std::sin(delta_lat / SECOND), 2) +
               std::cos(rad_orig_lat) * std::cos(rad_dest_lat) * std::pow(std::sin(delta_lon / SECOND), 2);

    double c = SECOND * std::sin(std::sqrt(a));
    double d = RADIUS * c;

    if (type_measure == "kms") {
        fmt::println("type_measure results: in kms!");
        return d;
    } else {
        fmt::println("type_measure results: in meters!");
        double mts = d * 1000;
        return mts;
    }
}

int main()
{
    fmt::println("haversine cli");

    // std::vector<double> origin = {51.5223337, -0.7198055};
    // std::vector<double> destiny = {51.5226574, -0.7219567};

    Coords origin{51.5223337, -0.7198055};
    Coords destiny{51.5226574, -0.7219567};

    fmt::println("");

    auto result_in_kms = haversine(origin, destiny, "kms");

    if (!result_in_kms) {
        fmt::println(stderr, "haversine kms Error: {}", result_in_kms.error());
    }

    fmt::println("result in kms: {:.3f}", *result_in_kms);

    fmt::println("");
    auto result_in_mts = haversine(origin, destiny, "mts");

    if (!result_in_mts) {
        fmt::println(stderr, "haversine mts Error: {}", result_in_mts.error());
    }

    fmt::println("result in mts: {:.3f}", *result_in_mts);
    return 0;
}
