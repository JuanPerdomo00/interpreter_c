
add_executable(arena_test memory_arena.c)
target_link_libraries(arena_test PRIVATE memory_library)
add_test(NAME arena_test COMMAND arena_test)

