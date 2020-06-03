aux_source_directory(. DIR_SHM_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(shm STATIC ${DIR_SHM_SRCS})

