aux_source_directory(. DIR_MSGQUEUE_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(msgq STATIC ${DIR_MSGQUEUE_SRCS})

