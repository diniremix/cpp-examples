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

option(
    WARNINGS_AS_ERRORS
    "Treat warnings as errors"
    OFF
)

# Sanitizers solamente en Debug
if(ENABLE_SANITIZERS)
  target_compile_options(
        project_options
        INTERFACE
            $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
                -fsanitize=address
                -fsanitize=undefined
                -fno-omit-frame-pointer
                -fno-optimize-sibling-calls
            >

            $<$<AND:$<NOT:$<CONFIG:Debug>>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
                -fstack-protector-strong
            >
    )

  target_link_options(
        project_options
        INTERFACE
            $<$<AND:$<NOT:$<CONFIG:Debug>>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
                -fsanitize=address
                -fsanitize=undefined
            >
    )
endif()
