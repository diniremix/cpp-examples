set dotenv-load := false

just_home := justfile_directory()

# for Windows

set windows-shell := ["pwsh.exe", "-NoLogo", "-Command"]

# set positional-arguments
#
# Project configuration
# globals vars

PROJECT_NAME := "hello_cpp"
BUILD_DIR := "build"

# ============================================================================
# Default
# ============================================================================

[doc("ℹ️  Show available commands")]
[group("Help")]
default: help

[doc("📋 Show all recipes")]
[group("Help")]
help:
    @just --list

# ============================================================================
# Configuration & Build
# ============================================================================

[doc("🔧 Configure debug build")]
[group("Configure")]
configure:
    cmake --preset debug

[doc("🔧 Configure release build")]
[group("Configure")]
configure-r:
    cmake --fresh --preset release

[doc("🔨 Build debug")]
[group("Build")]
build:
    cmake --build --preset debug -j

[doc("🔨 Build release")]
[group("Build")]
build-r: configure-r
    cmake --build --preset release -j

[doc("🔄 Reconfigure and rebuild debug")]
[group("Build")]
rebuild: clean-build configure build

# ============================================================================
# Run Main Application
# ============================================================================

[doc("▶️  Run main app (debug)")]
[group("Development")]
run: build
    ./{{ BUILD_DIR }}/debug/{{ PROJECT_NAME }}

[doc("▶️  Run main app (release)")]
[group("Development")]
run-r: build-r
    ./{{ BUILD_DIR }}/release/{{ PROJECT_NAME }}

# ============================================================================
# Quick Compilation (Single file, no CMake)
# ============================================================================

[doc("⚡ Quick compile single file (no CMake/VCPKG)")]
[group("Development")]
[unix]
quick file:
    g++ -std=c++23 -I include src/{{ file }}.cpp -o {{ BUILD_DIR }}/{{ file }}
    ./{{ BUILD_DIR }}/{{ file }}

[doc("⚡ Quick compile single file (no CMake/VCPKG)")]
[group("Development")]
[windows]
quick file:
    cl /EHsc /std:c++latest /W4 /Zc:__cplusplus /Fe:{{ file }}.exe exercises/{{ file }}.cpp
    .\{{ file }}.exe

# ============================================================================
# Exercises Management
# ============================================================================

[doc("📝 List all exercises")]
[group("Development")]
list:
    #!/usr/bin/env bash
    ls -1 exercises/*.cpp 2>/dev/null | sed 's|exercises/||' | sed 's|\.cpp$||' || echo "No exercises found"

[doc("🎯 Build and run specific exercise")]
[group("Development")]
ex name: (build-ex name)
    ./{{ BUILD_DIR }}/debug/{{ name }}

[doc("🎯 Build specific exercise without running")]
[group("Build")]
build-ex name:
    cmake --build --preset debug --target {{ name }} -j

[doc("🎯 Build specific exercise without running")]
[group("Development")]
[unix]
_new_exercise name:
    #!/usr/bin/env bash
    cat > "exercises/{{ name }}.cpp" <<'EOF'
    #include <fmt/core.h>

    int main(){
        fmt::println("hello world");
        return 0;
    }
    EOF

[group("Development")]
[windows]
_new_exercise name:
    #!pwsh
    @"
    #include <fmt/core.h>

    int main(){
        fmt::println("hello world");
        return 0;
    }
    "@ | Set-Content -Path (Join-Path -Path .\exercises -ChildPath "{{ name }}.cpp") -Encoding utf8

# puede llamar a configure la primera vez
[doc("📑 create a new exercise")]
[group("Development")]
new name: (_new_exercise name)

# ============================================================================
# Formatting & Linting
# ============================================================================

[doc("🎨 Format code")]
[group("Development")]
fmt:
    clang-format -i include/*.hpp src/*.cpp exercises/*.cpp

[doc("🔍 Check formatting (CI mode)")]
[group("Development")]
fmt-check:
    clang-format -i include/*.hpp src/*.cpp exercises/*.cpp --Werror

# ============================================================================
# Cleanup
# ============================================================================

[doc("🧹 Clean build artifacts")]
[group("Maintenance")]
[unix]
clean:
    rm -fv *.bin
    rm -fv *.csv
    rm -fv *_text_file.txt
    rm -rf logs/

[doc("🧹 Clean build artifacts")]
[group("Maintenance")]
[windows]
clean:
    Remove-Item -Verbose *.bin
    Remove-Item -Verbose *.csv
    Remove-Item -Verbose *_text_file.txt
    Remove-Item -Verbose -Recurse -Force logs

[doc("💥 Clean build files)")]
[group("Maintenance")]
clean-build: clean
    cmake --build --preset debug --target clean
    cmake --build --preset release --target clean

[doc("💥 Clean everything (build + generated files)")]
[group("Maintenance")]
[unix]
clean-all: clean
    rm -rf {{ BUILD_DIR }}/debug/* {{ BUILD_DIR }}/release/*

[doc("💥 Clean everything (build + generated files)")]
[group("Maintenance")]
[windows]
clean-all: clean
    Remove-Item -Recurse -Force {{ BUILD_DIR }}/debug/* , {{ BUILD_DIR }}/release/*

# ============================================================================
# Testing
# ============================================================================

[doc("🧪 Run all tests")]
[group("Testing")]
test: build
    ctest --test-dir {{ BUILD_DIR }}/debug --output-on-failure

# ============================================================================
# VCPKG Management
# ============================================================================

[doc("📦 Update VCPKG baseline")]
[group("Packages")]
update:
    #!/usr/bin/env bash
    echo "Current baseline:"
    grep -A 1 '"baseline"' vcpkg-configuration.json
    echo ""
    echo "To update to latest vcpkg, run:"
    echo " cd $VCPKG_ROOT && git pull"
    echo "Then update baseline in vcpkg-configuration.json"

[doc("📦 Install dependencies (vcpkg-install in manifest mode)")]
[group("Packages")]
add lib:
    vcpkg add port {{ lib }} && vcpkg install

# ============================================================================
# Git Utilities
# ============================================================================

[doc("🔍 Git repository check and cleanup")]
[group("Git")]
gitc:
    git fsck && git gc --prune=now --aggressive && git count-objects -vH
