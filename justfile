set dotenv-load := false
just_home := justfile_directory()

# globals vars
PROJECT_NAME := "hello_cpp"

configure:
    cmake --preset debug

configure-r:
    cmake --fresh --preset release

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

config-ninja:
    cmake --preset ninjad

config-test:
    # clear && cmake --preset debug
    cmake --preset debug

build-ninja:
    cmake --build --preset ninjad -j

build-test:
    # cmake --build --preset debug --target help
    cmake --build --preset debug -j

test pkg:
    # clear && cmake --build --preset debug --target {{ pkg }} && ./build/debug/{{ pkg }}
    cmake --build --preset debug --target {{ pkg }} && ./build/debug/{{ pkg }}

ninja pkg:
    cmake --build --preset ninjad --target {{ pkg }} && ./build/ninjad/{{ pkg }}

fmt-test:
    clang-format -i exercises/*.cpp include/*.hpp

clean-n:
    cmake --build --preset ninjad --target clean

clean-d:
    cmake --build --preset debug --target clean

clean-r:
    cmake --build --preset release --target clean

clean:
    rm -fv *.bin
    rm -fv *.csv
    rm -fv *_text_file.txt
    rm -rfv logs/

clean-all: clean
    rm -rf build/debug/* build/release/* build/ninjad/*

fmt:
    # clang-format -i $(shell git ls-files '*.cc' '*.h')
    clang-format -i include/*.hpp src/*.cpp --Werror

rebuild: clean-d build

test-all:
    ctest --test-dir build

[doc("🔍 Git check repo")]
[group("Git")]
gitc:
    git fsck && git gc --prune=now --aggressive && git count-objects -vH
