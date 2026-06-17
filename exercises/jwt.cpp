#include "dotenv.hpp"

#include <chrono>
#include <cstdlib>
#include <fmt/core.h>
#include <jwt-cpp/jwt.h>
#include <stdexcept>
#include <string>

struct JwtConfig {
    std::string issuer;
    std::string audience;
    std::string context;
    std::string secret;
};

struct ClaimsData {
    std::string sub;
    std::string jti;

    std::chrono::system_clock::time_point iat;
    std::chrono::system_clock::time_point nbf;
    std::chrono::system_clock::time_point exp;

    std::string data_json; // simplificado para evitar nlohmann dependencia aquí
};

static std::string env(std::string_view key)
{
    const char* v = std::getenv(key.data());
    if (!v) {
        throw std::runtime_error("missing env: " + std::string(key));
    }
    return v;
}

class JwtService
{
  public:
    explicit JwtService(const JwtConfig& cfg) : cfg(cfg) {}

    std::string generate(const ClaimsData& c)
    {
        return jwt::create()
            .set_issuer(cfg.issuer)
            .set_audience(cfg.audience)
            .set_subject(c.sub)
            .set_id(c.jti)
            .set_issued_at(c.iat)
            .set_not_before(c.nbf)
            .set_expires_at(c.exp)
            .set_payload_claim("ctx", jwt::claim(cfg.context))
            .set_payload_claim("data", jwt::claim(c.data_json))
            .sign(jwt::algorithm::hs256{cfg.secret});
    }

    void verify(const std::string& token)
    {
        auto decoded = jwt::decode(token);

        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{cfg.secret})
            .with_issuer(cfg.issuer)
            .with_audience(cfg.audience)
            .verify(decoded);

        if (decoded.get_payload_claim("ctx").as_string() != cfg.context) {
            throw std::runtime_error("invalid ctx");
        }
    }

  private:
    JwtConfig cfg;
};

int main()
{
    fmt::println("JWT working (jwt-cpp v0.7.x)");

    dotenv::load(".env");

    JwtConfig cfg{.issuer = "Dione Api",
                  .audience = "https://example.com/dione",
                  .context = "api.dione.org",
                  .secret = env("JWT_SECRET_KEY")};

    JwtService service(cfg);

    ClaimsData c{.sub = "123",
                 .jti = "uuid-1",
                 .iat = std::chrono::system_clock::now(),
                 .nbf = std::chrono::system_clock::now(),
                 .exp = std::chrono::system_clock::now() + std::chrono::minutes(15),
                 .data_json = R"({"user_id":1,"role":"admin"})"};

    auto token = service.generate(c);
    fmt::println("TOKEN: {}", token);

    service.verify(token);
    fmt::println("OK");
}
