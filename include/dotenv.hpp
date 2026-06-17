/*
 * from
 * - https://github.com/Ayush272002/dotenv/blob/main/example/main.cpp
 * commit info: Dic 1, 2025 #a6f963d0bbb509d2
 *
 */
#pragma once
#include <charconv>
#include <concepts>
#include <cstdlib>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dotenv {

    template <typename T>
    concept SupportedEnvType = std::integral<T> || std::floating_point<T> || std::same_as<T, std::string>;

    namespace detail {
        constexpr std::string_view trim_copy(std::string_view str)
        {
            size_t start = 0;
            size_t end = str.size();

            while (start < end && std::isspace(str[start]))
                start++;
            while (end > start && std::isspace(str[end - 1]))
                end--;
            return str.substr(start, end - start);
        }

        inline std::string strip_quotes(std::string_view sv)
        {
            if (sv.size() >= 2 && (sv.front() == '"' || sv.front() == '\'') && sv.front() == sv.back())
                return std::string(sv.substr(1, sv.size() - 2));
            return std::string(sv);
        }

        template <std::integral T> std::optional<T> parse_integral(std::string_view sv) noexcept
        {
            T value{};
            auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
            return ec == std::errc() ? std::optional(value) : std::nullopt;
        }

        template <std::floating_point T> std::optional<T> parse_floating(std::string_view sv) noexcept
        {
            try {
                return static_cast<T>(std::stod(std::string(sv)));
            } catch (...) {
                return std::nullopt;
            }
        }

        template <SupportedEnvType T> std::optional<T> parse_value(std::string_view sv) noexcept
        {
            if constexpr (std::same_as<T, std::string>)
                return std::string(sv);
            else if constexpr (std::integral<T>)
                return parse_integral<T>(sv);
            else if constexpr (std::floating_point<T>)
                return parse_floating<T>(sv);
            else
                return std::nullopt;
        }
    } // namespace detail

    class Dotenv
    {
        std::unordered_map<std::string, std::string> vars;

        Dotenv() = default;
        ~Dotenv() = default;
        Dotenv(const Dotenv&) = delete;
        Dotenv& operator=(const Dotenv&) = delete;

        static Dotenv& instance()
        {
            static Dotenv inst;
            return inst;
        }

        void load_file(std::string_view filename)
        {
            auto file = std::ifstream(std::string(filename));
            if (!file)
                return;

            std::string line;
            while (std::getline(file, line)) {
                line = std::string(detail::trim_copy(line));
                if (line.empty() || line[0] == '#')
                    continue;

                auto pos = line.find('=');
                if (pos == std::string::npos)
                    continue;

                std::string key = std::string(detail::trim_copy(line.substr(0, pos)));
                std::string val = detail::strip_quotes(detail::trim_copy(line.substr(pos + 1)));

                vars[key] = val;
#ifdef _WIN32
                _putenv_s(key.c_str(), val.c_str());
#else
                ::setenv(key.c_str(), val.c_str(), 1);
#endif
            }
        }

        template <SupportedEnvType T> std::optional<T> get_variable(std::string_view name) const noexcept
        {
            const auto it = vars.find(std::string(name));
            if (it == vars.end())
                return std::nullopt;
            return detail::parse_value<T>(it->second);
        }

        friend void load(std::string_view filename);
        template <SupportedEnvType T> friend std::optional<T> get(std::string_view name) noexcept;
    };

    inline void load(const std::string_view filename = ".env")
    {
        Dotenv::instance().load_file(filename);
    }

    template <SupportedEnvType T> inline std::optional<T> get(const std::string_view name) noexcept
    {
        return Dotenv::instance().get_variable<T>(name);
    }

} // namespace dotenv
