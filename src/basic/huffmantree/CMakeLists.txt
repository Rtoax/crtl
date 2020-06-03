aux_source_directory(. DIR_HUFFMAN_SRCS)

include_directories( ../../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(huffman STATIC ${DIR_HUFFMAN_SRCS})