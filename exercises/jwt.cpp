#include "dotenv.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fmt/core.h>
#include <fstream> // Para ifstream, ofstream, fstream
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/kazuho-picojson/defaults.h>
#include <picojson/picojson.h>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

std::string read_file(const fs::path& path)
{
    fmt::println("loading: '{}'", path.string());
    std::ifstream file(path);

    if (!file) {
        auto msg = fmt::format("cannot open file: '{}'", path.string());
        throw std::runtime_error(msg);
    }

    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

static std::string env(std::string_view key)
{
    const char* v = std::getenv(key.data());
    if (!v) {
        throw std::runtime_error("missing env: " + std::string(key));
    }
    return v;
}

namespace token_service {
    /*static constexpr auto IMPLICIT_ASSERT = "api.dione.org";
    static constexpr auto ISSUER = "Dione Api";
    static constexpr auto AUDIENCE = "https://example.com/dione";
    static constexpr auto PASETO_TOKEN_TYPE = "v4.public.";*/

    inline constexpr std::string_view IMPLICIT_ASSERT = "api.dione.org";
    inline constexpr std::string_view ISSUER = "Dione Api";
    inline constexpr std::string_view AUDIENCE = "https://example.com/dione";
    inline constexpr std::string_view TOKEN_TYPE = "v4.public.";

    struct JwtConfig {
        // std::string issuer;
        // std::string audience;
        // std::string context;
        // std::string secret;
        std::string private_key;
        std::string public_key;
    };

    struct ClaimsData {
        std::string sub;
        std::string jti;

        std::chrono::system_clock::time_point iat;
        std::chrono::system_clock::time_point nbf;
        std::chrono::system_clock::time_point exp;

        picojson::value data;
    };

    class JwtService
    {
      public:
        explicit JwtService(const JwtConfig& cfg) : cfg(cfg) {}

        std::string generate(const ClaimsData& c)
        {
            return jwt::create()
                .set_issuer(std::string(ISSUER))
                .set_audience(std::string(AUDIENCE))
                .set_subject(c.sub)
                .set_id(c.jti)
                .set_issued_at(c.iat)
                .set_not_before(c.nbf)
                .set_expires_at(c.exp)
                .set_payload_claim("ctx", jwt::claim(std::string(IMPLICIT_ASSERT)))
                .set_payload_claim("data", jwt::claim(c.data))
                //.sign(jwt::algorithm::hs256{cfg.secret});
                //.sign(jwt::algorithm::ed25519{public_key_pem, private_key_pem});
                .sign(jwt::algorithm::ed25519{cfg.public_key, cfg.private_key});
        }

        void verify(const std::string& token)
        {
            auto decoded = jwt::decode(token);

            jwt::verify()
                //.allow_algorithm(jwt::algorithm::hs256{cfg.secret})
                //.allow_algorithm(jwt::algorithm::ed25519{public_key_pem})
                .allow_algorithm(jwt::algorithm::ed25519{cfg.public_key})
                .with_issuer(std::string(ISSUER))
                .with_audience(std::string(AUDIENCE))
                .verify(decoded);

            if (decoded.get_payload_claim("ctx").as_string() != IMPLICIT_ASSERT) {
                throw std::runtime_error("invalid ctx");
            }
        }

      private:
        JwtConfig cfg;
    };
} // namespace token_service

int main()
{
    fmt::println("JWT example");

    dotenv::load(".env");
    auto private_key = read_file(env("JWT_PRIVATE_KEY_FILE"));

    auto public_key = read_file(env("JWT_PUBLIC_KEY_FILE"));

    /*
    .issuer = "Dione Api",
    .audience = "https://example.com/dione",
    .context = "api.dione.org",
    .secret = env("JWT_SECRET_KEY")
    */
    token_service::JwtConfig cfg(public_key, private_key);

    token_service::JwtService service(cfg);

    picojson::object obj;
    obj["user_id"] = picojson::value(1.0);
    obj["role"] = picojson::value("admin");

    token_service::ClaimsData c{.sub = "123",
                                .jti = "uuid-1",
                                .iat = std::chrono::system_clock::now(),
                                .nbf = std::chrono::system_clock::now(),
                                .exp = std::chrono::system_clock::now() + std::chrono::minutes(15),
                                .data = picojson::value(obj)};

    auto token = service.generate(c);
    fmt::println("TOKEN: {}", token);

    service.verify(token);
    fmt::println("OK");
}
