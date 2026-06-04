#include <cpr/cpr.h>
#include <expected>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
using json = nlohmann::json; // Alias para facilitar su uso

struct Weather {
    std::string timezone;
    std::string timezone_abbr;
    float temperature;
    float feels_like;
    int humidity;
    float wind_speed;
    float dew_point;
    float pressure;
    int cloud_cover;
    float visibility;

    std::string temp_unit;
    std::string humid_unit;
    std::string wind_unit;
    std::string pressure_unit;
    std::string visibility_unit;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Weather, timezone, temperature, feels_like, humidity, wind_speed, dew_point,
                                   pressure, cloud_cover, visibility, temp_unit, humid_unit, wind_unit, pressure_unit,
                                   visibility_unit);
};

template <> struct fmt::formatter<Weather> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Weather& u, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", nlohmann::json(u).dump());
    }
};

struct Coordinates {
    double latitude;
    double longitude;
};

// current_units
struct CurrentUnitsResp {
    std::optional<std::string> time;
    std::optional<std::string> interval;
    std::optional<std::string> temperature_2m;
    std::optional<std::string> apparent_temperature;
    std::optional<std::string> precipitation;
    std::optional<std::string> precipitation_probability;
    std::optional<std::string> weather_code;
    std::optional<std::string> cloudcover;
    std::optional<std::string> windspeed_10m;
    std::optional<std::string> winddirection_10m;
    std::optional<std::string> uv_index;
    std::optional<std::string> relative_humidity_2m;
    std::optional<std::string> dewpoint_2m;
    std::optional<std::string> visibility;
    std::optional<std::string> surface_pressure;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CurrentUnitsResp, time, interval, temperature_2m, apparent_temperature,
                                   precipitation, precipitation_probability, weather_code, cloudcover, windspeed_10m,
                                   winddirection_10m, uv_index, relative_humidity_2m, dewpoint_2m, visibility,
                                   surface_pressure);
};

// current
struct CurrentResp {
    std::optional<std::string> time;
    std::optional<int> interval;
    std::optional<double> temperature_2m;
    std::optional<double> apparent_temperature;
    std::optional<double> precipitation;
    std::optional<int> precipitation_probability;
    std::optional<int> weather_code;
    std::optional<int> cloudcover;
    std::optional<double> windspeed_10m;
    std::optional<int> winddirection_10m;
    std::optional<double> uv_index;
    std::optional<int> relative_humidity_2m;
    std::optional<double> dewpoint_2m;
    std::optional<double> visibility;
    std::optional<double> surface_pressure;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CurrentResp, time, interval, temperature_2m, apparent_temperature, precipitation,
                                   precipitation_probability, weather_code, cloudcover, windspeed_10m,
                                   winddirection_10m, uv_index, relative_humidity_2m, dewpoint_2m, visibility,
                                   surface_pressure);
};

// handle response
struct ResponseJson {
    std::optional<float> latitude;
    std::optional<float> longitude;
    std::optional<float> generationtime_ms;
    std::optional<int> utc_offset_seconds;
    std::optional<std::string> timezone;
    std::optional<std::string> timezone_abbreviation;
    std::optional<float> elevation;
    std::optional<CurrentUnitsResp> current_units;
    std::optional<CurrentResp> current;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseJson, latitude, longitude, generationtime_ms, utc_offset_seconds, timezone,
                                   timezone_abbreviation, elevation, current_units, current);
};

template <> struct fmt::formatter<ResponseJson> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const ResponseJson& u, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", nlohmann::json(u).dump());
    }
};

template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, std::string_view>
std::string join(const R& range, std::string_view sep = ",")
{
    std::string result;

    auto it = range.begin();

    if (it == range.end())
        return result;

    result += std::string_view(*it++);

    while (it != range.end()) {
        result += sep;
        result += std::string_view(*it++);
    }

    return result;
}

std::expected<Weather, std::string> make_resp(const ResponseJson& response)
{
    if (!response.current)
        return std::unexpected("Missing current weather data");

    if (!response.current_units)
        return std::unexpected("Missing current weather units");

    const auto& current = response.current.value();
    const auto& units = response.current_units.value();

    Weather weather;

    weather.timezone = response.timezone.value_or("Unknown");
    weather.timezone_abbr = response.timezone_abbreviation.value_or("Unknown");
    weather.temperature = current.temperature_2m.value_or(0.0f);
    weather.feels_like = current.apparent_temperature.value_or(0.0f);
    weather.humidity = current.relative_humidity_2m.value_or(0);
    weather.wind_speed = current.windspeed_10m.value_or(0.0f);
    weather.dew_point = current.dewpoint_2m.value_or(0.0f);
    weather.pressure = current.surface_pressure.value_or(0.0f);
    weather.cloud_cover = current.cloudcover.value_or(0);

    float visibility = current.visibility.value_or(0.0f);

    if (visibility > 1000) {
        weather.visibility = visibility / 1000;
        weather.visibility_unit = "km";
    } else if (visibility > 0 && visibility < 1000) {
        weather.visibility = visibility;
        weather.visibility_unit = "m";
    } else {
        weather.visibility = visibility;
        weather.visibility_unit = units.visibility.value_or("");
    }

    weather.temp_unit = units.apparent_temperature.value_or("");
    weather.humid_unit = units.relative_humidity_2m.value_or("");
    weather.wind_unit = units.windspeed_10m.value_or("");
    weather.pressure_unit = units.surface_pressure.value_or("");

    return weather;
}

std::expected<Weather, std::string> current_weather(Coordinates coords, std::string tz)
{
    cpr::Url base_uri = cpr::Url{"https://api.open-meteo.com/v1/forecast"};

    std::vector<std::string> default_params = {
        "temperature_2m",   "apparent_temperature", "precipitation", "precipitation_probability",
        "weather_code",     "cloudcover",           "windspeed_10m", "winddirection_10m",
        "uv_index",         "relative_humidity_2m", "dewpoint_2m",   "visibility",
        "surface_pressure",
    };

    auto join_params = join(default_params);
    // fmt::println("params: {}", join_params);

    cpr::Parameters params = {
        {"latitude", std::to_string(coords.latitude)},
        {"longitude", std::to_string(coords.longitude)},
        {"current", join_params},
        {"timezone", tz},
    };

    cpr::Response resp_get = cpr::Get(base_uri, params);
    // fmt::println("GET resp url: {}", resp_get.url.str());
    fmt::println("GET resp status_code: {}", resp_get.status_code);
    // fmt::println("GET resp text: {}", resp_get.text);

    if (resp_get.error.code != cpr::ErrorCode::OK) {
        fmt::println("GET response network error");
        return std::unexpected("response network error");
    }

    if (resp_get.status_code != 200) {
        fmt::println("GET response HTTP error");
        return std::unexpected("response HTTP error");
    }

    try {
        auto json_resp = json::parse(resp_get.text);
        // fmt::println("GET json_resp (dump): {}", json(json_resp).dump(2));
        ResponseJson resp = json_resp.get<ResponseJson>();
        fmt::println("");
        // fmt::println("GET ResponseJson resp: {}", resp);

        return make_resp(resp);
    } catch (const std::exception& e) {
        fmt::println(stderr, "Error: {}", e.what());
        return std::unexpected(fmt::format("{}", e.what()));
    }
}

int main()
{
    fmt::println("current weather Api example");

    Coordinates coords{51.5186902, -0.1332287};

    auto result = current_weather(coords, "Europe/London");
    // std::expected<Weather, std::string> result = current_weather(coords, "Europe/London");

    fmt::println("current_weather response");
    if (!result) {
        fmt::println("Error: {}", result.error());
    } else {
        // fmt::println("Resultado: {}", *result); // result.value()
        // const Weather& result_weather = result.value();
        auto result_weather = result.value();

        fmt::println("Resume:");
        fmt::println("{}({})", result_weather.timezone, result_weather.timezone_abbr);
        fmt::println("temp {}{}", result_weather.temperature, result_weather.temp_unit);
        fmt::println("Feels like {}{}", result_weather.feels_like, result_weather.temp_unit);
        fmt::println("Humidity {}{}", result_weather.humidity, result_weather.humid_unit);
        fmt::println("Wind {} {}", result_weather.wind_speed, result_weather.wind_unit);
        fmt::println("Dew point {}{}", result_weather.dew_point, result_weather.temp_unit);
        fmt::println("Pressure {} {}", result_weather.pressure, result_weather.pressure_unit);
        fmt::println("Cloud cover {}{}", result_weather.cloud_cover, result_weather.humid_unit);
        fmt::println("Visibility {} {}", result_weather.visibility, result_weather.visibility_unit);
    }

    return 0;
}
