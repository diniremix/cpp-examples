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

        -- librerias por defecto
        add_packages(
            "fmt"
        )
    target_end()
end

--
add_exercise("bin_files")
add_exercise("bin_pod_files")
add_exercise("caesar_cipher")
add_exercise("chars",{
    packages = {
        "uni_algo"
    }
})
add_exercise("classes")
add_exercise("cli_app",{
        packages = { "cli11"}
    }
)
add_exercise("compiler_info")
add_exercise("coroutines_ext",{
        include = true,
        packages = { "asio"}
    }
)
add_exercise("coroutines_simple")
add_exercise("create_text_files")
add_exercise("csv_files")
add_exercise("factorial")
add_exercise("fibonacci")
add_exercise("fibonacci_rec")
add_exercise("find_primes")
add_exercise("fizzbuzz")
add_exercise("formatter",{
        include = true,
        defines = { "HAS_FMT=1" }
    }
)
add_exercise("haversine")
add_exercise("logger",{
        packages = {
            "spdlog",
            "nlohmann_json_git"
        }
    }
)
add_exercise("loops")
add_exercise("matrix_struct")
add_exercise("morse",{
    packages = {
        "uni_algo"
    }
})
add_exercise(
    "open_meteo",
    {
        packages = {
            "cpr",
            -- "nlohmann_json"
            "nlohmann_json_git"
        }
    }
)
add_exercise("print")
add_exercise("read_text_files")
add_exercise("regex_demo")
add_exercise("rest_client",{
        packages = {
            "cpr",
            -- "nlohmann_json"
            "nlohmann_json_git"
        }
    }
)
add_exercise("result_error")
add_exercise("socket_client")
add_exercise("socket_server")
add_exercise("sqlite_demo", {
    packages = {
        "sqlite3",
    }
})
add_exercise("sqlite_orm_demo",{
        packages = {
            "sqlitecpp",
        }
    })
add_exercise("structs",{
        include = true,
    }
)
add_exercise("testing")
add_exercise("thread_simple")
add_exercise("base_x",{
    packages = {
        "libsodium",
    }
})
add_exercise("crypt",{
    packages = {
        "libsodium",
    }
})
add_exercise("hashing",{
    packages = {
        "blake3",
        "libsodium",
    }
})
add_exercise("rand_uuid",{
    packages = {
        "libsodium",
    }
})
add_exercise("terminal")
add_exercise("ranges")
add_exercise("concepts")
add_exercise("namespaces",  {include = true})
add_exercise("date_time",{
        packages = {
            "date",
            "date-tz",
        }
    }
)
add_exercise("jwt",{
        include=true,
        packages = {"jwt-cpp", "picojson", "libsodium"}
    }
)
add_exercise("stdio")
