
add_executable(vector_test test_vector.c)
target_link_libraries(vector_test PRIVATE utils_library)
add_test(NAME vector_test COMMAND vector_test)


add_executable(hashmap_test test_hashmap.c)
target_link_libraries(hashmap_test PRIVATE utils_library)
add_test(NAME hashmap_test COMMAND hashmap_test)

