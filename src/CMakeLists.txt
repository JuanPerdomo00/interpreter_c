
file(GLOB SRC_FILES "*.c")

#add subdirectories
add_subdirectory(utils)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(tests)

add_executable(nivy ${SRC_FILES})

set_target_properties(nivy PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})



target_link_libraries(nivy PRIVATE utils_library)
target_link_libraries(nivy PRIVATE lexer_library)
target_link_libraries(nivy PRIVATE parser_library)
