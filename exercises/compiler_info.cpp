#include <iostream>
#include <version>
// #include <expected>
// #include <generator>
// #include <mdspan>
// #include <stacktrace>
// #include <ranges>

// Macros de versión estándar para C++20/23
// __cpp_lib_print         // C++23 - std::print
// __cpp_lib_format        // C++20 - std::format
// __cpp_lib_expected      // C++23 - std::expected
// __cpp_lib_generator     // C++23 - std::generator
// __cpp_lib_ranges        // C++20 - Ranges library
// __cpp_lib_mdspan        // C++23 - std::mdspan
// __cpp_lib_stacktrace    // C++23 - std::stacktrace
// __cpp_lib_move_only_function // C++23 - std::move_only_function
// __cpp_lib_copyable_function  // C++23 - std::copyable_function

int main()
{
    std::cout << "=== Compiler ===\n";

#if defined(__clang__)
    std::cout << "Compiler: Clang\n";
    std::cout << "Version: " << __clang_major__ << '.' << __clang_minor__ << '.' << __clang_patchlevel__ << '\n';

#elif defined(_MSC_VER)
    std::cout << "Compiler: MSVC\n";
    std::cout << "_MSC_VER = " << _MSC_VER << '\n';

#elif defined(__GNUC__)
    std::cout << "Compiler: GCC\n";
    std::cout << "Version: " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << '\n';
#else
    std::cout << "Compiler: Unknown\n";
#endif

// GCC / libstdc++
#ifdef __GLIBCXX__
    std::cout << "__GLIBCXX__ build date = " << __GLIBCXX__ << '\n';
#endif
// Clang + libc++
#ifdef _LIBCPP_VERSION
    std::cout << "_LIBCPP_VERSION = " << _LIBCPP_VERSION << '\n';
#endif
// MSVC STL
#ifdef _MSVC_STL_VERSION
    std::cout << "_MSVC_STL_VERSION = " << _MSVC_STL_VERSION << '\n';
#endif

#ifdef _MSVC_LANG
    std::cout << "_MSVC_LANG = " << _MSVC_LANG << '\n';
#endif

    std::cout << "=== Core ===\n";

#ifdef __cplusplus
    std::cout << "__cplusplus = " << __cplusplus << '\n';
#endif

    std::cout << "\n=== Formatting ===\n";

#ifdef __cpp_lib_format
    std::cout << "__cpp_lib_format = " << __cpp_lib_format << '\n';
#endif

#ifdef __cpp_lib_print
    std::cout << "__cpp_lib_print = " << __cpp_lib_print << '\n';
#endif

    std::cout << "\n=== Containers ===\n";

#ifdef __cpp_lib_span
    std::cout << "__cpp_lib_span = " << __cpp_lib_span << '\n';
#endif

#ifdef __cpp_lib_mdspan
    std::cout << "__cpp_lib_mdspan = " << __cpp_lib_mdspan << '\n';
#endif

#ifdef __cpp_lib_flat_map
    std::cout << "__cpp_lib_flat_map = " << __cpp_lib_flat_map << '\n';
#endif

#ifdef __cpp_lib_flat_set
    std::cout << "__cpp_lib_flat_set = " << __cpp_lib_flat_set << '\n';
#endif

    std::cout << "\n=== Ranges ===\n";

#ifdef __cpp_lib_ranges
    std::cout << "__cpp_lib_ranges = " << __cpp_lib_ranges << '\n';
#endif

#ifdef __cpp_lib_ranges_zip
    std::cout << "__cpp_lib_ranges_zip = " << __cpp_lib_ranges_zip << '\n';
#endif

#ifdef __cpp_lib_ranges_chunk
    std::cout << "__cpp_lib_ranges_chunk = " << __cpp_lib_ranges_chunk << '\n';
#endif

    std::cout << "\n=== Error Handling ===\n";

#ifdef __cpp_lib_expected
    std::cout << "__cpp_lib_expected = " << __cpp_lib_expected << '\n';
#endif

#ifdef __cpp_lib_stacktrace
    std::cout << "__cpp_lib_stacktrace = " << __cpp_lib_stacktrace << '\n';
#endif

    std::cout << "\n=== Functional ===\n";

#ifdef __cpp_lib_move_only_function
    std::cout << "__cpp_lib_move_only_function = " << __cpp_lib_move_only_function << '\n';
#endif

#ifdef __cpp_lib_copyable_function
    std::cout << "__cpp_lib_copyable_function = " << __cpp_lib_copyable_function << '\n';
#endif

#ifdef __cpp_lib_function_ref
    std::cout << "__cpp_lib_function_ref = " << __cpp_lib_function_ref << '\n';
#endif

    std::cout << "\n=== Coroutines ===\n";

#ifdef __cpp_lib_coroutine
    std::cout << "__cpp_lib_coroutine = " << __cpp_lib_coroutine << '\n';
#endif

#ifdef __cpp_lib_generator
    std::cout << "__cpp_lib_generator = " << __cpp_lib_generator << '\n';
#endif

    std::cout << "\n=== Concurrency ===\n";

#ifdef __cpp_lib_jthread
    std::cout << "__cpp_lib_jthread = " << __cpp_lib_jthread << '\n';
#endif

#ifdef __cpp_lib_barrier
    std::cout << "__cpp_lib_barrier = " << __cpp_lib_barrier << '\n';
#endif

#ifdef __cpp_lib_latch
    std::cout << "__cpp_lib_latch = " << __cpp_lib_latch << '\n';
#endif

#ifdef __cpp_lib_semaphore
    std::cout << "__cpp_lib_semaphore = " << __cpp_lib_semaphore << '\n';
#endif

    std::cout << "\n=== Files / IO ===\n";

#ifdef __cpp_lib_filesystem
    std::cout << "__cpp_lib_filesystem = " << __cpp_lib_filesystem << '\n';
#endif

#ifdef __cpp_lib_spanstream
    std::cout << "__cpp_lib_spanstream = " << __cpp_lib_spanstream << '\n';
#endif

#ifdef __cpp_lib_syncbuf
    std::cout << "__cpp_lib_syncbuf = " << __cpp_lib_syncbuf << '\n';
#endif

    std::cout << "\n=== Modules ===\n";
#ifdef __cpp_modules
    std::cout << "__cpp_modules = " << __cpp_modules << '\n';
#endif

    std::cout << "\n=== ConstExpr ===\n";

#ifdef __cpp_lib_constexpr_vector
    std::cout << "__cpp_lib_constexpr_vector = " << __cpp_lib_constexpr_vector << '\n';
#endif
#ifdef __cpp_lib_constexpr_string
    std::cout << "__cpp_lib_constexpr_string = " << __cpp_lib_constexpr_string << '\n';
#endif

    return 0;
}
