#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
using json = nlohmann::json;

constexpr std::uint32_t FILE_LOG_SIZE = 5 * 1024 * 1024; // 5Mb

spdlog::logger create_sync_logger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    //  Crear un sink rotativo para archivos (tamaño máximo de 5MB y hasta 3 archivos)
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/sync.log", FILE_LOG_SIZE, 3);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("sync_multi_sink", {console_sink, file_sink});

    // se puede fijar el nivel en el nuevo logegr
    logger.set_level(spdlog::level::trace);
    logger.flush_on(spdlog::level::trace); // spdlog::level::err

    return logger;
}

std::shared_ptr<spdlog::logger> create_async_logger()
{
    spdlog::init_thread_pool(8192, 1);

    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    //  Crear un sink rotativo para archivos (tamaño máximo de 5MB y hasta 3 archivos)
    auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/async.log", FILE_LOG_SIZE, 3);

    std::vector<spdlog::sink_ptr> sinks{console, file};

    // `spdlog::async_overflow_policy::block`
    //  El productor espera, so se pierden logs.
    // `spdlog::async_overflow_policy::overrun_oldest`
    // Descarta los mensajes más viejos.
    auto async_logger = std::make_shared<spdlog::async_logger>(
        "async_multi_sink", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    async_logger->set_level(spdlog::level::trace);
    async_logger->flush_on(spdlog::level::trace); // spdlog::level::err

    return async_logger;
}

int main()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::trace("Trace message");
    spdlog::debug("Debug message");
    spdlog::info("Info message");
    spdlog::warn("Warning message");
    spdlog::error("Error message");
    spdlog::critical("Critical message");

    fmt::println("");

    spdlog::set_level(spdlog::level::warn);
    spdlog::trace("Trace message using level");
    spdlog::debug("Debug message using level");
    spdlog::info("Info message using level");
    spdlog::warn("Warning message using level");
    spdlog::error("Error message using level");
    spdlog::critical("Critical message using level");

    fmt::println("");
    spdlog::set_level(spdlog::level::trace);

    // async_logger
    auto async_logger = create_async_logger();

    // logs a un archivo de forma asincrona
    // y a la consola al mismo tiempo
    async_logger->trace("Mensaje asíncrono de trace a consola y archivo de logs");
    async_logger->debug("Mensaje asíncrono de debug a consola y archivo de logs");
    async_logger->info("Mensaje asíncrono de info a consola y archivo de logs");
    async_logger->warn("Mensaje asíncrono de warn a consola y archivo de logs");
    async_logger->error("Mensaje asíncrono de error a consola y archivo de logs");
    async_logger->critical("Mensaje asíncrono de critical a consola y archivo de logs");

    spdlog::set_pattern("[%H:%M:%S.%e] "
                        "[%l] "
                        "[thread %t] "
                        "%v");
    fmt::println("");

    for (int i = 0; i < 1'000; ++i) {
        async_logger->info("mensaje {}", i);
    }

    // sync_logger
    auto sync_logger = create_sync_logger();
    sync_logger.info("Info message with custom pattern");
    sync_logger.warn("Warning message with custom pattern");

    // resetear el pattern para el log
    spdlog::set_pattern("%+");

    fmt::println("");

    try {
        throw std::runtime_error("Error de conexión con la Base de datos");
    } catch (const std::exception& e) {
        spdlog::error("Excepción: {}", e.what());
    }

    for (int i = 0; i < 1'000; ++i) {
        sync_logger.info("mensaje {}", i);
    }

    json log_entry = {{"severity", "INFO"}, {"message", "User login"}, {"user_id", 42}};

    async_logger->info("log_entry output: {}", log_entry.dump());

    async_logger->flush();
    sync_logger.flush();
    spdlog::shutdown();

    return 0;
}
