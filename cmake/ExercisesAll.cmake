# aqui se utiliza GLOB para buscar de forma
# automatica todos los archivos

file(
    GLOB EXERCISES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/exercises/*.cpp"
)

foreach(EXERCISE ${EXERCISES})

    get_filename_component(
        EXERCISE_NAME
        ${EXERCISE}
        NAME_WE
    )

    add_executable(
        ${EXERCISE_NAME}
        ${EXERCISE}
    )

    target_include_directories(
        ${EXERCISE_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(
        ${EXERCISE_NAME}
        PRIVATE
            project_options
            project_warnings
            ${COMMON_LIBS}
    )

    target_compile_definitions(
        ${EXERCISE_NAME}
        PRIVATE
            HAS_FMT=1
    )

    endif()

endforeach()
