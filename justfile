set dotenv-load := false
just_home := justfile_directory()

# globals vars
PROJECT_NAME := "hello_cpp"

configure:
    cmake --preset debug

configure-r:
    cmake --preset release

build: configure
    # cmake --build build
    cmake --build --preset debug

build-r: configure-r
    cmake --build --preset release -j

release: build-r
    ./build/{{ PROJECT_NAME }}

run-d:
    ./build/debug/{{ PROJECT_NAME }}

run-r:
    ./build/release/{{ PROJECT_NAME }}

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

rebuild: clean-d configure build

test:
    ctest --test-dir build
