aux_source_directory(. DIR_LINUX_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(linux STATIC ${DIR_LINUX_SRCS})

