add_executable(static_tasks static_tasks.c)
target_link_libraries(static_tasks PRIVATE LibLL::LibLL)

add_executable(ready_blocked ready_blocked.c)
target_link_libraries(ready_blocked PRIVATE LibLL::LibLL)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(static_tasks PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(ready_blocked PRIVATE -Wall -Wextra -Wpedantic)
endif()
