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
# Configuration & Build
# ============================================================================

[doc("🔧 Configure debug build")]
configure:
    cmake --preset debug

[doc("🔧 Configure release build")]
configure-r:
    cmake --fresh --preset release

[doc("🔨 Build debug")]
build:
    cmake --build --preset debug -j

[doc("🔨 Build release")]
build-r: configure-r
    cmake --build --preset release -j

[doc("🔄 Reconfigure and rebuild debug")]
rebuild: clean-build configure build

# ============================================================================
# Run Main Application
# ============================================================================

[doc("▶️  Run main app (debug)")]
run: build
    ./{{ BUILD_DIR }}/debug/{{ PROJECT_NAME }}

[doc("▶️  Run main app (release)")]
run-r: build-r
    ./{{ BUILD_DIR }}/release/{{ PROJECT_NAME }}

# ============================================================================
# Quick Compilation (Single file, no CMake)

[doc("⚡ Quick compile single file (no CMake/VCPKG)")]
[unix]
quick file:
    g++ -std=c++23 -I include src/{{ file }}.cpp -o {{ BUILD_DIR }}/{{ file }}
    ./{{ BUILD_DIR }}/{{ file }}

[doc("⚡ Quick compile single file (no CMake/VCPKG)")]
[windows]
quick file:
    cl /EHsc /std:c++latest /W4 /Zc:__cplusplus /Fe:{{ file }}.exe exercises/{{ file }}.cpp
    .\{{ file }}.exe

# ============================================================================
# Exercises Management
# ============================================================================

[doc("📝 List all exercises")]
list:
    #!/usr/bin/env bash
    ls -1 exercises/*.cpp 2>/dev/null | sed 's|exercises/||' | sed 's|\.cpp$||' || echo "No exercises found"

[doc("🎯 Build and run specific exercise")]
ex name: (build-ex name)
    ./{{ BUILD_DIR }}/debug/{{ name }}

[doc("🎯 Build specific exercise without running")]
build-ex name:
    cmake --build --preset debug --target {{ name }} -j

[doc("🎯 Build specific exercise without running")]
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

# llama a configure la primera vez

# new name: (_new_exercise name) configure
new name: (_new_exercise name)

# ============================================================================
# Formatting & Linting
# ============================================================================

[doc("🎨 Format code")]
fmt:
    clang-format -i include/*.hpp src/*.cpp exercises/*.cpp

[doc("🔍 Check formatting (CI mode)")]
fmt-check:
    clang-format -i include/*.hpp src/*.cpp exercises/*.cpp --Werror

# ============================================================================
# Cleanup
# ============================================================================

[doc("🧹 Clean build artifacts")]
[unix]
clean:
    rm -fv *.bin
    rm -fv *.csv
    rm -fv *_text_file.txt
    rm -rf logs/

[doc("🧹 Clean build artifacts")]
[windows]
clean:
    Remove-Item -Verbose *.bin
    Remove-Item -Verbose *.csv
    Remove-Item -Verbose *_text_file.txt
    Remove-Item -Verbose -Recurse -Force logs

[doc("💥 Clean build files)")]
clean-build: clean
    cmake --build --preset debug --target clean
    cmake --build --preset release --target clean

[doc("💥 Clean everything (build + generated files)")]
[unix]
clean-all: clean
    rm -rf {{ BUILD_DIR }}/debug/* {{ BUILD_DIR }}/release/*

[doc("💥 Clean everything (build + generated files)")]
[windows]
clean-all: clean
    Remove-Item -Recurse -Force {{ BUILD_DIR }}/debug/* , {{ BUILD_DIR }}/release/*

# ============================================================================
# Testing
# ============================================================================

[doc("🧪 Run all tests")]
test: build
    ctest --test-dir {{ BUILD_DIR }}/debug --output-on-failure

# ============================================================================
# VCPKG Management
# ============================================================================

[doc("📦 Update VCPKG baseline")]
update:
    #!/usr/bin/env bash
    echo "Current baseline:"
    grep -A 1 '"baseline"' vcpkg-configuration.json
    echo ""
    echo "To update to latest vcpkg, run:"
    echo " cd $VCPKG_ROOT && git pull"
    echo "Then update baseline in vcpkg-configuration.json"

[doc("📦 Install dependencies (vcpkg-install in manifest mode)")]
add lib:
    vcpkg add port {{ lib }} && vcpkg install

# ============================================================================
# Git Utilities
# ============================================================================

[doc("🔍 Git repository check and cleanup")]
[group("Git")]
gitc:
    git fsck && git gc --prune=now --aggressive && git count-objects -vH

# ============================================================================
# Default
# ============================================================================

[doc("ℹ️  Show available commands")]
default:
    @just --list
