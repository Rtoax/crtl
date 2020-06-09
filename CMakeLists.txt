CMAKE_MINIMUM_REQUIRED(VERSION 2.8) 

PROJECT(crtl LANGUAGES C)

#添加源文件 
aux_source_directory(./src  DIR_SRCS)
aux_source_directory(./src/basic  DIR_SRCS)
aux_source_directory(./src/basic/huffmantree  DIR_SRCS)
aux_source_directory(./src/basic/json  DIR_SRCS)
aux_source_directory(./src/bits  DIR_SRCS)
aux_source_directory(./src/math  DIR_SRCS)
aux_source_directory(./src/mm  DIR_SRCS)
aux_source_directory(./src/module  DIR_SRCS)
aux_source_directory(./src/mqueue  DIR_SRCS)
aux_source_directory(./src/network  DIR_SRCS)
aux_source_directory(./src/shm  DIR_SRCS)
aux_source_directory(./src/sort  DIR_SRCS)
aux_source_directory(./src/linux  DIR_SRCS)
aux_source_directory(./src/thread  DIR_SRCS)
aux_source_directory(./src/tcp-ip  DIR_SRCS)
aux_source_directory(./src/hpc  DIR_SRCS)

#add_subdirectory(./src/bits)


#设置编译参数 
set(CMAKE_C_FLAGS "-g -Wall")

#添加头文件路径 
include_directories( ./ )

#添加库 
find_library(RT rt /usr/lib64) 
find_library(DL dl /usr/lib64) 
find_library(MATH m /usr/lib64)
find_library(CRYPT crypt /usr/lib64)
find_library(PTHREAD pthread /usr/lib64)



#link库 
link_libraries(${DL} ${RT} ${MATH} ${CRYPT} ${PTHREAD})

#定义 
add_definitions(-g3 -MD -Wall -fopenmp)

#生成动态库和静态库 
add_library(crtl_shared_library  SHARED ${DIR_SRCS}) 



#将动态库和静态库的名字设置 
set_target_properties(crtl_shared_library PROPERTIES OUTPUT_NAME "crtl") 

#target_link_libraries(crtl_shared_library crtl_bits)


set(LIBRARY_OUTPUT_PATH ../lib) 

#设置动态库版本 
#set_target_properties(crtl_shared_library PROPERTIES VERSION 1.0 SOVERSION 1) 


#安装库文件  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
install(TARGETS crtl_shared_library DESTINATION /usr/lib64)

#安装头文件
file(GLOB crtl_hdrs ./crtl/*.h)
file(GLOB crtl_hdrs_easy ./crtl/easy/*.h)
file(GLOB crtl_hdrs_bits ./crtl/bits/*.h)
file(GLOB crtl_hdrs_hpc ./crtl/hpc/*.h)
file(GLOB crtl_hdrs_network ./crtl/network/*.h)
file(GLOB crtl_hdrs_linux ./crtl/linux/*.h)
file(GLOB crtl_hdrs_tcpip ./crtl/tcp-ip/*.h)


install(FILES ${crtl_hdrs} DESTINATION /usr/local/include/crtl/)
install(FILES ${crtl_hdrs_easy} DESTINATION /usr/local/include/crtl/easy)
install(FILES ${crtl_hdrs_bits} DESTINATION /usr/local/include/crtl/bits)
install(FILES ${crtl_hdrs_hpc} DESTINATION /usr/local/include/crtl/hpc)
install(FILES ${crtl_hdrs_network} DESTINATION /usr/local/include/crtl/network)
install(FILES ${crtl_hdrs_linux} DESTINATION /usr/local/include/crtl/linux)
install(FILES ${crtl_hdrs_tcpip} DESTINATION /usr/local/include/crtl/tcp-ip)

# For pkg-config --libs --cflags libcrtl	FILE_PERMISSIONS "666" 
install(FILES ${PROJECT_BINARY_DIR}/../libcrtl.pc DESTINATION /usr/lib/pkgconfig)