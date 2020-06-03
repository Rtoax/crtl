aux_source_directory(. DIR_MODULE_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(module STATIC ${DIR_MODULE_SRCS})



