function add_exercise(name, opts)
    opts = opts or {}

    target(name)
        set_kind("binary")
        add_rules("config_base")
        add_rules("config_options")
        add_files("$(projectdir)/exercises/" .. name .. ".cpp")

        if opts.include then
            add_includedirs("$(projectdir)/include")
        end

        if opts.defines then
            add_defines(table.unpack(opts.defines))
        end

        if opts.packages then
            -- print("add_exercise opts.packages:", table.unpack(opts.packages))
            add_packages(table.unpack(opts.packages))
        end

        add_packages(
            "fmt",
            "nlohmann_json",
            "spdlog"
        )
    target_end()
end

-- add_exercise("bin_files")
-- add_exercise("bin_pod_files")
add_exercise("caesar_cipher")
add_exercise(
    "chars",
      {
          packages = {
              "fmt",
              "uni_algo"
          }
      }
)
add_exercise("classes")
-- -- add_exercise(
-- --     cli_app
-- --     LIBS
-- --       CLI11::CLI11
-- -- )
add_exercise("compiler_info")
add_exercise(
    "coroutines_ext",
        {
            include = true,
            packages = { "asio"}
        }
)
add_exercise("coroutines_simple")
-- add_exercise("create_text_files")
-- add_exercise("csv_files")
add_exercise("factorial")
add_exercise("fibonacci")
add_exercise("fibonacci_rec")
add_exercise("find_primes")
add_exercise("fizzbuzz")
add_exercise(
    "formatter",
    {
        include = true,
        defines = { "HAS_FMT=1" },
        packages = { "fmt"}
    }
)
add_exercise("haversine")
add_exercise(
    "logger",
    {
        packages = {
            "fmt",
            "spdlog"
        }
    }
)
add_exercise("loops")
add_exercise("matrix_struct")
add_exercise("morse",{
    packages = {
        "fmt",
        "uni_algo"
    }
})
add_exercise(
    "open_meteo",
    {
        packages = {
            "fmt",
            "cpr",
            "nlohmann_json"
        }
    }
)
add_exercise("print")
-- add_exercise("read_text_files")
add_exercise("regex_demo")
add_exercise(
    "rest_client",
    {
        packages = {
            "fmt",
            "cpr",
            "nlohmann_json"
        }
    }
)
add_exercise("result_error")
-- add_exercise("socket_client")
-- add_exercise("socket_server")
-- -- add_exercise(
-- --     sqlite_demo
-- --     LIBS
-- --         unofficial::sqlite3::sqlite3
-- -- )
-- -- add_exercise(
-- --     sqlite_orm_demo
-- --     LIBS
-- --         SQLiteCpp
-- -- )
add_exercise(
    "structs",
    {include = true}
)
add_exercise("testing")
add_exercise("thread_simple")
add_exercise("base_x",{
    packages = {
        "fmt",
        "libsodium",
    }
})
add_exercise("crypt",{
    packages = {
        "fmt",
        "libsodium",
    }
})
add_exercise("hashing",{
    packages = {
        "fmt",
        "blake3",
        "libsodium",
    }
})
add_exercise("rand_uuid",{
    packages = {
        "fmt",
        "libsodium",
    }
})
add_exercise("terminal")
add_exercise("ranges")
add_exercise("concepts")
add_exercise("namespaces",  {include = true})
add_exercise("date_time",
    {
        packages = {
            "date",
            "date-tz",
        }
    }
)
add_exercise("jwt",
    {
        include=true,
        packages = { "fmt", "jwt-cpp", "picojson", "libsodium"}
    }
)
