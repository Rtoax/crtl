aux_source_directory(. DIR_NETWORK_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(network STATIC ${DIR_NETWORK_SRCS})


