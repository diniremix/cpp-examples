#include <cpr/cpr.h>
#include <fmt/core.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json; // Alias para facilitar su uso

struct User {
    std::string username;
    std::string avatar;
    bool active;
    int age;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, username, avatar, active, age);
};

template <> struct fmt::formatter<User> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const User& u, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", nlohmann::json(u).dump());
    }
};

int main()
{
    fmt::println("cliente rest");
    fmt::println("");
    fmt::println("method: GET");

    cpr::Response resp_get = cpr::Get(cpr::Url{"https://httpbin.org/get"});
    // std::cout << resp_get.url << std::endl;
    fmt::println("GET resp url: {}", resp_get.url.str());
    fmt::println("GET resp status_code: {}", resp_get.status_code);
    fmt::println("GET resp text: {}", resp_get.text);

    json responseJson = json::parse(resp_get.text);
    // fmt::println("resp responseJson: {}", responseJson);
    std::cout << "GET resp responseJson: " << responseJson << std::endl;
    // no genera error si los campos no existen...
    std::cout << "GET Response name: " << responseJson["json"]["name"] << std::endl;
    std::cout << "GET Response age: " << responseJson["json"]["age"] << std::endl;

    fmt::println("");
    fmt::println("method: POST");

    json json_data;
    json_data["name"] = "John";
    json_data["age"] = 30;
    json_data["robot"] = false;

    cpr::Response resp_post = cpr::Post(cpr::Url{"https://httpbin.org/post"}, cpr::Body{json_data.dump()},
                                        cpr::Header{{"Content-Type", "application/json"}}, cpr::Bearer{"ACCESS_TOKEN"});

    fmt::println("POST resp status_code: {}", resp_post.status_code);
    fmt::println("POST resp text: {}", resp_post.text);

    fmt::println("");
    fmt::println("method: PUT");

    cpr::Response resp_put = cpr::Put(cpr::Url{"https://httpbin.org/put"}, cpr::Body{json_data.dump()},
                                      cpr::Header{{"Content-Type", "application/json"}});

    fmt::println("resp url: {}", resp_put.url.str());
    fmt::println("PUT resp status_code: {}", resp_put.status_code);
    fmt::println("PUT resp text: {}", resp_put.text);

    fmt::println("");
    fmt::println("method: DELETE");

    cpr::Response resp_delete = cpr::Delete(cpr::Url{"https://httpbin.org/delete"});

    fmt::println("DELETE resp url: {}", resp_delete.url.str());
    fmt::println("DELETE resp status_code: {}", resp_delete.status_code);
    fmt::println("DELETE resp text: {}", resp_delete.text);

    fmt::println("");
    fmt::println("method: POST usando structs");

    User user{"John", "/pictures/avatars/user1.png", true, 30};
    json ju = user;

    cpr::Response resp_post_s = cpr::Post(cpr::Url{"https://httpbin.org/post"}, cpr::Body{ju.dump()},
                                          cpr::Header{{"Content-Type", "application/json"}});

    fmt::println("POST resp url: {}", resp_post_s.url.str());
    fmt::println("POST resp status_code: {}", resp_post_s.status_code);
    fmt::println("POST resp text: {}", resp_post_s.text);

    // error de red
    if (resp_post_s.error.code != cpr::ErrorCode::OK) {
        fmt::println("POST response network error");
        return 1;
    }

    if (resp_post_s.status_code != 200) {
        fmt::println("POST response HTTP error");
        return 1;
    }

    auto json_resp = json::parse(resp_post_s.text);

    if (!json_resp.contains("json")) {
        // throw std::runtime_error("response error: missing json field");
        fmt::println("POST response error: missing json field");
        return 1;
    }

    // User user_resp = json_resp["json"].get<User>();
    // preferir 'at'
    User user_resp = json_resp.at("json").get<User>();
    fmt::println("POST resp user_resp: {}", user_resp);

    return 0;
}
