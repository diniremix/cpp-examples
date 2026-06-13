add_library(project_platform INTERFACE)

# 0x0601 cubre Windows 7
# 0x0A00 cubre Windows 10 y Windows 11
target_compile_definitions(project_platform INTERFACE
    $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0A00>
    $<$<PLATFORM_ID:Windows>:NOMINMAX>
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
)

# POSIX tweaks
# target_compile_definitions(project_platform INTERFACE
#     $<$<NOT:$<PLATFORM_ID:Windows>>:_GNU_SOURCE>
# )
