aux_source_directory(. DIR_DIM_SRCS)

include_directories( ../../ )

add_subdirectory(huffmantree)

set(CMAKE_C_FLAGS "-fPIC")

add_library(basic STATIC ${DIR_DIM_SRCS})

target_link_libraries(basic huffman)
