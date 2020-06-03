aux_source_directory(. DIR_TCPIP_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(tcpip STATIC ${DIR_TCPIP_SRCS})



