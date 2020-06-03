aux_source_directory(. DIR_MM_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(mm STATIC ${DIR_MM_SRCS})


