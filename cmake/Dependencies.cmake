find_package(spdlog CONFIG REQUIRED)
find_package(uni-algo CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

find_package(fmt CONFIG QUIET)

set(COMMON_LIBS
    spdlog::spdlog
    uni-algo::uni-algo
    nlohmann_json::nlohmann_json
    cpr::cpr
    unofficial::sqlite3::sqlite3
    SQLiteCpp
    CLI11::CLI11
    asio::asio
)

if(fmt_FOUND)
    list(APPEND COMMON_LIBS fmt::fmt)
endif()
