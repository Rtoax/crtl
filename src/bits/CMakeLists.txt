aux_source_directory(. DIR_BITS_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(crtl_bits STATIC ${DIR_BITS_SRCS})


