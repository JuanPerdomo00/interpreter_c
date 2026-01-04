
add_executable(token_test test_token.c)
target_link_libraries(token_test PRIVATE lexer_library)
add_test(NAME token_test COMMAND token_test)

add_executable(lexer_test test_lexer.c)
target_link_libraries(lexer_test PRIVATE lexer_library)
add_test(NAME lexer_test COMMAND lexer_test)
