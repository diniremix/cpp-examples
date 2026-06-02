set dotenv-load := false
just_home := justfile_directory()

# globals vars
PROJECT_NAME := "hello_cpp"

configure:
    cmake --preset debug

configure-r:
    cmake --preset release

build:
    # cmake --build build
    cmake --build --preset debug

build-r: configure-r
    cmake --build --preset release -j

run-d: build
    ./build/debug/{{ PROJECT_NAME }}

run-r:
    ./build/release/{{ PROJECT_NAME }}

cc cmd:
    g++ -std=c++23 src/{{ cmd }}.cpp -o build/{{ cmd }} && ./build/{{ cmd }}

config-test:
    clear && cmake --preset debug
    # cmake --build --preset debug
    # cmake --build --preset debug --target help

test pkg:
    clear && cmake --build --preset debug --target {{ pkg }} && ./build/debug/{{ pkg }}

fmt-test:
    clang-format -i exercises/*.cpp include/*.hpp

clean-d:
    cmake --build --preset debug --target clean

clean-r:
    cmake --build --preset release --target clean

clean-all:
    # rm -rf build
    rm -rf build/debug/* build/release/*

fmt:
    # clang-format -i $(shell git ls-files '*.cc' '*.h')
    clang-format -i src/*.cpp

rebuild: clean-d build

test-all:
    ctest --test-dir build

[doc("🔍 Git check repo")]
[group("Git")]
gitc:
    git fsck && git gc --prune=now --aggressive && git count-objects -vH
