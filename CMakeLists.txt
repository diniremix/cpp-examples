cmake_minimum_required(VERSION 3.28)

project(
    hello_cpp
    VERSION 0.3.0
    LANGUAGES CXX
)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_COLOR_MAKEFILE ON)
# set(CMAKE_VERBOSE_MAKEFILE OFF)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(ProjectOptions)
include(ProjectWarnings)
include(Dependencies)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        project_options
        project_warnings
        ${COMMON_LIBS}
)

include(Exercises)
