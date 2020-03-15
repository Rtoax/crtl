# Install script for directory: /mnt/hgfs/G/crtl

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib64/libcrtl.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib64/libcrtl.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/lib64/libcrtl.so"
         RPATH "")
  ENDIF()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/lib64/libcrtl.so")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/lib64" TYPE SHARED_LIBRARY PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/mnt/hgfs/G/crtl/lib/libcrtl.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib64/libcrtl.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib64/libcrtl.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/lib64/libcrtl.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/crtl_alloc.h;/usr/local/include/crtl/crtl_assert.h;/usr/local/include/crtl/crtl_file.h;/usr/local/include/crtl/crtl_iterator.h;/usr/local/include/crtl/crtl_keyboard.h;/usr/local/include/crtl/crtl_list.h;/usr/local/include/crtl/crtl_lock.h;/usr/local/include/crtl/crtl_log.h;/usr/local/include/crtl/crtl_mask.h;/usr/local/include/crtl/crtl_random.h;/usr/local/include/crtl/crtl_terminal.h;/usr/local/include/crtl/crtl_time.h;/usr/local/include/crtl/crtl_tree.h;/usr/local/include/crtl/crtl_types.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl" TYPE FILE FILES
    "/mnt/hgfs/G/crtl/./crtl/crtl_alloc.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_assert.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_file.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_iterator.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_keyboard.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_list.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_lock.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_log.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_mask.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_random.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_terminal.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_time.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_tree.h"
    "/mnt/hgfs/G/crtl/./crtl/crtl_types.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/easy/attribute.h;/usr/local/include/crtl/easy/byteswap.h;/usr/local/include/crtl/easy/endian.h;/usr/local/include/crtl/easy/extern.h;/usr/local/include/crtl/easy/macro.h;/usr/local/include/crtl/easy/operator.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl/easy" TYPE FILE FILES
    "/mnt/hgfs/G/crtl/./crtl/easy/attribute.h"
    "/mnt/hgfs/G/crtl/./crtl/easy/byteswap.h"
    "/mnt/hgfs/G/crtl/./crtl/easy/endian.h"
    "/mnt/hgfs/G/crtl/./crtl/easy/extern.h"
    "/mnt/hgfs/G/crtl/./crtl/easy/macro.h"
    "/mnt/hgfs/G/crtl/./crtl/easy/operator.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/bits/crtl_assert_backtrace.h;/usr/local/include/crtl/bits/crtl_list.h;/usr/local/include/crtl/bits/crtl_lock_mutex.h;/usr/local/include/crtl/bits/crtl_lock_semaphore.h;/usr/local/include/crtl/bits/crtl_mask.h;/usr/local/include/crtl/bits/crtl_time_def.h;/usr/local/include/crtl/bits/crtl_tree_rbtree.h;/usr/local/include/crtl/bits/crtl_types_bits_set.h;/usr/local/include/crtl/bits/crtl_types_ctype.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl/bits" TYPE FILE FILES
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_assert_backtrace.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_list.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_lock_mutex.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_lock_semaphore.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_mask.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_time_def.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_tree_rbtree.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_types_bits_set.h"
    "/mnt/hgfs/G/crtl/./crtl/bits/crtl_types_ctype.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/network/byteorder.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl/network" TYPE FILE FILES "/mnt/hgfs/G/crtl/./crtl/network/byteorder.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/mnt/hgfs/G/crtl/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/mnt/hgfs/G/crtl/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
