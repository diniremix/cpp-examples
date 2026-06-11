find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(uni-algo CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(blake3 CONFIG REQUIRED)

set(COMMON_LIBS
    fmt::fmt
    spdlog::spdlog
    uni-algo::uni-algo
    nlohmann_json::nlohmann_json
    cpr::cpr
    unofficial::sqlite3::sqlite3
    SQLiteCpp
    CLI11::CLI11
    asio::asio
    blake3::blake3
)
