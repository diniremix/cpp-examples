add_library(project_options INTERFACE)

target_compile_features(
    project_options
    INTERFACE
        cxx_std_23
)

set(CMAKE_CXX_EXTENSIONS OFF)

option(
    ENABLE_SANITIZERS
    "Enable AddressSanitizer and UBSan"
    OFF
)

if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
            project_options
            INTERFACE
                -fsanitize=address
                -fsanitize=undefined
        )

        target_link_options(
            project_options
            INTERFACE
                -fsanitize=address
                -fsanitize=undefined
        )
    endif()
endif()
