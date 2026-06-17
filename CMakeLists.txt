cmake_minimum_required(VERSION 3.28)

project(
    hello_cpp
    VERSION 0.5.0
    LANGUAGES CXX
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message("using GCC version: ${CMAKE_CXX_COMPILER_VERSION}")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.3)
    message(FATAL_ERROR "Error: GCC version must be 13.3 or higher")
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message("using Clang version: ${CMAKE_CXX_COMPILER_VERSION}")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18.0)
    message(FATAL_ERROR "Error: Clang version must be 18.0 or higher")
  endif()
endif()

if(MSVC)
  message("using MSVC version: ${MSVC_VERSION}")
  if(MSVC_VERSION LESS 1944)
    message(FATAL_ERROR "Error: MSVC version must be 19.44 or higher")
  endif()
endif()



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
include(ProjectPlatform)
include(Dependencies)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
)

# target_include_directories(
#     ${PROJECT_NAME}
#     PRIVATE
#         ${PROJECT_SOURCE_DIR}/include
# )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        project_options
        project_warnings
        project_platform
        # ${COMMON_LIBS}
        fmt::fmt
        spdlog::spdlog
)

# include(ExercisesAll)
include(Exercises)
