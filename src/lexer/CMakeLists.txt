add_library(lexer_library STATIC lexer.c kind.c token.c utils.c)
target_link_libraries(lexer_library PRIVATE utils_library)
