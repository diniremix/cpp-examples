add_library(project_warnings INTERFACE)

target_compile_options(
    project_warnings
    INTERFACE

        # GCC
        $<$<CXX_COMPILER_ID:GNU>:
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wcast-qual
            -Wold-style-cast
            -Wnon-virtual-dtor
            -Woverloaded-virtual
            -Wnull-dereference
            -Wdouble-promotion
            -Wimplicit-fallthrough
            -Wdeprecated
            -Wdeprecated-declarations
            -Wundef
            -Wformat=2
            -fdiagnostics-color=always
            -fdiagnostics-show-option
            # -fno-diagnostics-show-caret
            -fmessage-length=0
        >

        # Clang
        $<$<CXX_COMPILER_ID:Clang>:
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wcast-qual
            -Wold-style-cast
            -Wnon-virtual-dtor
            -Woverloaded-virtual
            -Wnull-dereference
            -Wdouble-promotion
            -Wimplicit-fallthrough
            -Wdeprecated
            -Wdeprecated-declarations
            -Wundef
            -Wformat=2
            -fcolor-diagnostics
            # -fno-diagnostics-show-caret
        >

        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:
            # /std:c++latest
            /Zc:__cplusplus
            /W4
        >
)

if(WARNINGS_AS_ERRORS)
  target_compile_options(
         project_warnings
         INTERFACE
             $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
                 -Werror
             >
             $<$<CXX_COMPILER_ID:MSVC>:/WX>
     )
endif()
