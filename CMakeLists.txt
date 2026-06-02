cmake_minimum_required(VERSION 3.28)

project(hello_cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Activar advertencias de compilación
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow")

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# target_include_directories(
#    ${PROJECT_NAME}
#    PRIVATE
#    ${CMAKE_SOURCE_DIR}/include
#)

set(COMMON_LIBS
    fmt::fmt
    spdlog::spdlog
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
     ${COMMON_LIBS}
)

# exercises folder
file(GLOB EXERCISES "exercises/*.cpp")

foreach(EXERCISE ${EXERCISES})
    get_filename_component(NAME ${EXERCISE} NAME_WE)

    add_executable(${NAME} ${EXERCISE})

     target_include_directories(
        ${NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(
        ${NAME}
        PRIVATE
        ${COMMON_LIBS}
    )
endforeach()
