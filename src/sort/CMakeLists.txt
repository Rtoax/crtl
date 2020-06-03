aux_source_directory(. DIR_SORT_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(sort STATIC ${DIR_SORT_SRCS})


