
# CPP exercises

Simple exercises for learning modern C++ ([C++ 20 and above](https://isocpp.org/)).


## Requirements

 - [GCC](https://gcc.gnu.org/) **13.3.x** and above
 - [vcpkg](https://vcpkg.io/en/) **2026-05-27** and above
 - [Cmake](https://cmake.org/download/) **3.28.x** and above
 - [Ninja](https://ninja-build.org/) **1.11.x** and above

## Recommended tools
 - [xmake](https://xmake.io/) **3.0.6.x** and above
 - [just](https://github.com/casey/just) **1.47.x** and above
 - [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
 - [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
 - [clangd (lsp)](https://clangd.llvm.org/)


## Getting Started

### Clone the repo and make it yours:

```bash
$ git clone git@github.com:diniremix/cpp-examples.git
```

```bash
$ cd cpp-examples
```

### Install dependencies
```bash
$ vcpkg install
```

### Configure
```bash
$ just configure
# cmake --preset debug
```

### Build
```bash
$ just build
# cmake --build --preset debug -j
```

### Run exercise
```bash
$ just ex EXERCISE_NAME
# cmake --build --preset debug --target EXERCISE_NAME -j
# ./build/debug/EXERCISE_NAME
```


## Tutorials

create a new example:
 ```bash
 $ just new EXERCISE_NAME
 ```
 
 Then add the following line to `cmake/Exercises.cmake`:
  ```cmake
 add_exercise(EXERCISE_NAME)
 ```
 
 Finally, run:
  ```bash
 $ just ex EXERCISE_NAME
 ```


##  tested on

|         O.S.         |       compiler               |
| -------------------- | ---------------------------- |
| Ubuntu 24.04         |   (GCC 13.3.0)               |
| Fedora 44            |   (GCC 16.1.1)               |
| Windows 11           |   (MSVC 19.44.35226 for x64) |


## License

**CPP examples** is licensed under either of the following licenses, at your option:

- Apache License, Version 2.0, ([LICENSE-APACHE](LICENSE-APACHE) or [http://www.apache.org/licenses/LICENSE-2.0])
- MIT license ([LICENSE-MIT](LICENSE-MIT) or [http://opensource.org/licenses/MIT])
