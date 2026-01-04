
add_executable(string_test string_test.c)

target_link_libraries(string_test PRIVATE parser_library)

add_test(NAME string_test COMMAND string_test)
