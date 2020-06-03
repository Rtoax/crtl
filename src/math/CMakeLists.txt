aux_source_directory(. DIR_MATH_SRCS)

include_directories( ../../ )

set(CMAKE_C_FLAGS "-fPIC")

add_library(math STATIC ${DIR_MATH_SRCS})


