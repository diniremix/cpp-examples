set_xmakever("3.0.6")
add_rules("mode.debug", "mode.release")
set_policy("package.precompiled", true)
includes("@builtin/xpack")
--
includes("xmake")
includes("xmake/dependencies_git.lua")
--
set_project("hello-cpp")
set_version("0.6.0")
set_description("A C++23 examples")
--
set_rundir("$(projectdir)")
set_installdir("dist")
--
-- set_languages("cxx20","cxx23")
set_languages("cxx23")
-- set_arch("x64")
--
-- dependencies
--add_defines("ASIO_STANDALONE")
add_requires(
    "fmt",
    "spdlog",
    "sqlite3",
    "sqlitecpp",
    "cli11",
    "blake3",
    "asio",
    "date",
    "libsodium"
)
--
-- add_requires("nlohmann_json")
--
add_requires("uni_algo",{configs = {header_only = true}})
--
add_requires("picojson")
add_requires("jwt-cpp", {configs = {picojson = true, ssl = "openssl3"}})
--
--add_requires("openssl3", {system = false})
add_requires("cpr", {configs = {ssl = true}, system = false})
--
-- enable options by xmake flags
option("enable_sanitizers")
    set_default(false)
    set_showmenu(true)
    set_description("Enable AddressSanitizer and UBSan")
option_end()

option("warnings_as_errors")
    set_default(false)
    set_showmenu(true)
    set_description("Treat warnings as errors (debug only)")
option_end()

-- platforms
if is_plat("windows") then
    -- 0x0601 cubre Windows 7
    -- 0x0A00 cubre Windows 10 y Windows 11
    add_defines("_WIN32_WINNT=0x0A00", "NOMINMAX", "WIN32_LEAN_AND_MEAN")
end

-- rules
rule("config_options")
    on_load(function (target)
        local tc = target:toolchain("gcc", "clang")
        local name = tc and tc:name()

        -- activable manualmente con `xmake f --enable-sanitizers=y`
        if has_config("enable_sanitizers") then
            if name == "gcc" or name == "clang" then
                target:add("cxflags",
                    "-fsanitize=address",
                    "-fsanitize=undefined",
                    "-fno-omit-frame-pointer",
                    "-fno-optimize-sibling-calls"
                )
                target:add("ldflags",
                    "-fsanitize=address",
                    "-fsanitize=undefined"
                )
            end
        end

        -- activable manualmente con `xmake f --warnings-as-errors=y`
        if has_config("warnings_as_errors") then
            if name == "gcc" or name == "clang" then
                target:add("cxflags","-Werror")
            elseif name == "msvc" then
                target:add("cxflags","/WX")
            end
        end
    end)

rule("config_base")
    on_load(function (target)
        local tc = target:toolchain("gcc", "clang")
        local name = tc and tc:name()

        if name == "gcc" or name == "clang" then
            target:add("cxflags",
                "-Wall",
                "-Wextra",
                "-Wpedantic",
                "-Wshadow",
                "-Wconversion",
                "-Wsign-conversion",
                "-Wcast-qual",
                "-Wold-style-cast",
                "-Wnon-virtual-dtor",
                "-Woverloaded-virtual",
                "-Wnull-dereference",
                "-Wdouble-promotion",
                "-Wimplicit-fallthrough",
                "-Wdeprecated",
                "-Wdeprecated-declarations",
                "-Wundef",
                "-Wformat=2"
            )
            if name == "gcc" then
                target:add("cxflags",
                    "-fdiagnostics-color=always",
                    "-fdiagnostics-show-option",
                    "-fmessage-length=0"
                )
            elseif name == "clang" then
                target:add("cxflags", "-fcolor-diagnostics")
            end
        elseif name == "msvc" then
            -- "/std:c++latest",
            target:add("cxflags", "/Zc:__cplusplus", "/W4", {tools = {"cl"}})
        end
    end)

rule("config_release")
    -- print("entro config release")
    on_load(function(target)
        local tc = target:toolchain("gcc", "clang")
        local name = tc and tc:name()

        if name == "gcc" or name == "clang" then
            target:add("cxflags", "-fstack-protector-strong")
            target:set("optimize", "fastest")
            target:set("strip", "all")
        elseif name == "msvc" then
            target:add("runtimes", "MD") -- MT|MTd
        end
    end)

-- main target
target("hello_cpp")
    set_kind("binary")
    set_plat(os.host())
    --
    add_rules("config_base")
    add_rules("config_options")

    if is_mode("release") then
        add_rules("config_release")
    end

    add_includedirs("src", {public = false})

    add_headerfiles("include/**.hpp")

    add_files(
        "src/main.cpp"
    )

    add_packages(
        "fmt",
        "spdlog"
    )
target_end()

-- packaging
xpack("hello_cpp")
    set_title("hello-cpp")
    set_author("diniremix")
    set_homepage("https://gitlab.com/diniremix")
    set_formats("zip", "targz")
    add_targets("hello_cpp")
    set_license("Apache-2.0 or MIT")
    add_installfiles("LICENSE-MIT", "LICENSE-APACHE", "README.md")
    set_bindir("bin")
    set_libdir("lib")
