cmake_minimum_required(VERSION 3.27)

project(hello_cpp VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Activar advertencias de compilación
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow")

#
#
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)  # No mostrar todos los comandos
#
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Colores y formato amigable
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-diagnostics-show-caret")  # Oculta el caret (^^^^^^^)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")  # Sin truncar líneas
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang tiene salida aún más clara
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-diagnostics-show-caret")
endif()
#

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(uni-algo CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

# target_include_directories(
#    ${PROJECT_NAME}
#    PRIVATE
#    ${CMAKE_SOURCE_DIR}/include
#)

set(COMMON_LIBS
    fmt::fmt
    spdlog::spdlog
    uni-algo::uni-algo
    nlohmann_json::nlohmann_json
    cpr::cpr
    unofficial::sqlite3::sqlite3
    SQLiteCpp
    CLI11::CLI11
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
