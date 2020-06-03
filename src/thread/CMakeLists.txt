aux_source_directory(. DIR_THREAD_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(thread STATIC ${DIR_THREAD_SRCS})




