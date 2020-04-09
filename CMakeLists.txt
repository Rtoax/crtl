CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 
PROJECT(crtl)

#添加源文件 
aux_source_directory(./src  DIR_SRCS)
aux_source_directory(./src/basic  DIR_SRCS)
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

#设置编译参数 
set(CMAKE_C_FLAGS "-g -Wall")

#添加头文件路径 
include_directories( ./crtl ./ )

#添加库 
find_library(RT rt /usr/lib64) 
find_library(DL dl /usr/lib64) 
find_library(MATH m /usr/lib64)
find_library(CRYPT crypt /usr/lib64)
find_library(PTHREAD pthread /usr/lib64)

#link库 
link_libraries(${DL} ${RT} ${MATH} ${CRYPT} ${PTHREAD})

#定义 
add_definitions(-g3 -MD -Wall)

#生成动态库和静态库 
add_library(acc_lib_shared  SHARED ${DIR_SRCS}) 
add_library(acc_lib_static 	STATIC ${DIR_SRCS})


#将动态库和静态库的名字设置 
set_target_properties(acc_lib_shared PROPERTIES OUTPUT_NAME "crtl") 
set_target_properties(acc_lib_static PROPERTIES OUTPUT_NAME "crtl")


set(LIBRARY_OUTPUT_PATH ../lib) 

#设置动态库版本 
#set_target_properties(acc_lib_shared PROPERTIES VERSION 1.0 SOVERSION 1) 


#安装库文件
install(TARGETS acc_lib_shared DESTINATION /usr/lib64 PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)

#安装头文件
file(GLOB crtl_hdrs ./crtl/*.h)
file(GLOB crtl_hdrs_easy ./crtl/easy/*.h)
file(GLOB crtl_hdrs_bits ./crtl/bits/*.h)
file(GLOB crtl_hdrs_network ./crtl/network/*.h)
file(GLOB crtl_hdrs_linux ./crtl/linux/*.h)
file(GLOB crtl_hdrs_tcpip ./crtl/tcp-ip/*.h)
file(GLOB crtl_hdrs_tcpip_bits ./crtl/tcp-ip/bits/*.h)

install(FILES ${crtl_hdrs} DESTINATION /usr/local/include/crtl/)
install(FILES ${crtl_hdrs_easy} DESTINATION /usr/local/include/crtl/easy)
install(FILES ${crtl_hdrs_bits} DESTINATION /usr/local/include/crtl/bits)
install(FILES ${crtl_hdrs_network} DESTINATION /usr/local/include/crtl/network)
install(FILES ${crtl_hdrs_linux} DESTINATION /usr/local/include/crtl/linux)
install(FILES ${crtl_hdrs_tcpip} DESTINATION /usr/local/include/crtl/tcp-ip)
install(FILES ${crtl_hdrs_tcpip_bits} DESTINATION /usr/local/include/crtl/tcp-ip/bits)

