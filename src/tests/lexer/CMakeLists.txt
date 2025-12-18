
add_executable(token_test test_token.c)
target_link_libraries(token_test PRIVATE lexer_library)
add_test(NAME token_test COMMAND token_test)
