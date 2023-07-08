set(A2_SOURCE_FILES
    toilet.c
    vector.c
    vector.h
    definitions.h
    resources.h)

add_executable(toilet ${A2_SOURCE_FILES})
target_link_libraries(toilet pthread)
