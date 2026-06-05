#include <CLI/CLI.hpp>
#include <fmt/core.h>

// cmd gen
// cli_app gen module_name
// cli_app gen module_name --dry-run
// cmd test
// cli_app test module_name
// cmd config
// cli_app config create
// cli_app config validate
//
constexpr auto VERSION = "1.0.0";

int main(int argc, char** argv)
{
    CLI::App app{"Demo CLI"};
    // version
    app.set_version_flag("-v,--version", VERSION);

    // gen
    std::string gen_target;
    bool dry_run = false;

    auto* cmd_gen = app.add_subcommand("gen", "Generate module");

    cmd_gen->add_option("module", gen_target, "module name")->required();

    cmd_gen->add_flag("--dry-run", dry_run, "For testing purposes only; do not create the module");

    // test
    std::string test_target;

    auto* cmd_test = app.add_subcommand("test", "Test module");

    cmd_test->add_option("module", test_target, "test module name")->required();

    // config
    auto* cmd_config = app.add_subcommand("config", "Configuration");

    auto* config_create = cmd_config->add_subcommand("create", "create configuration file");

    auto* config_validate = cmd_config->add_subcommand("validate", "validate configuration file");

    //
    // para comando sencillos usar `if (*cmd_xxx)`

    /*
    if (*cmd_gen) {
        fmt::println("Generating '{}' (dry-run={})", gen_target, dry_run);
    }
    if (*cmd_test) {
        fmt::println("Testing '{}'", test_target);
    }
    if (*config_create) {
        fmt::println("Creating config");
    }
    if (*config_validate) {
        fmt::println("Validating config");
    }
    */

    // si crece mucho usar el modo callback

    cmd_gen->callback([&]() { fmt::println("Generating '{}' (dry-run={})", gen_target, dry_run); });
    cmd_test->callback([&]() { fmt::println("Testing '{}'", test_target); });
    config_create->callback([&]() { fmt::println("Creating config"); });
    config_validate->callback([&]() { fmt::println("Validating config"); });

    CLI11_PARSE(app, argc, argv);

    return 0;
}
