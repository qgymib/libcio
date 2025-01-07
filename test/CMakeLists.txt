set(test_name ${PROJECT_NAME}_test)

add_executable(${test_name}
    "cases/synchronization_once.c"
    "cases/synchronization_thread.c"
    "cases/synchronization_thread_key.c"
    "cutest.c"
    "main.c"
)

target_include_directories(${test_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${test_name}
    PRIVATE
        ${PROJECT_NAME}
)

cio_setup_target_wall(${test_name})

add_test(NAME ${test_name} COMMAND ${test_name})
