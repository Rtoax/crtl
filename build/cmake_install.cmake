# Install script for directory: /work/workspace/crtl

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
  FOREACH(file
      "$ENV{DESTDIR}/usr/lib64/libcrtl.so.1.0"
      "$ENV{DESTDIR}/usr/lib64/libcrtl.so.1"
      "$ENV{DESTDIR}/usr/lib64/libcrtl.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/lib64/libcrtl.so.1.0;/usr/lib64/libcrtl.so.1;/usr/lib64/libcrtl.so")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/lib64" TYPE SHARED_LIBRARY PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES
    "/work/workspace/crtl/lib/libcrtl.so.1.0"
    "/work/workspace/crtl/lib/libcrtl.so.1"
    "/work/workspace/crtl/lib/libcrtl.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/lib64/libcrtl.so.1.0"
      "$ENV{DESTDIR}/usr/lib64/libcrtl.so.1"
      "$ENV{DESTDIR}/usr/lib64/libcrtl.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/crtl_assert.h;/usr/local/include/crtl/crtl_tree.h;/usr/local/include/crtl/crtl_file.h;/usr/local/include/crtl/crtl_types.h;/usr/local/include/crtl/crtl_list.h;/usr/local/include/crtl/crtl_random.h;/usr/local/include/crtl/crtl_mask.h;/usr/local/include/crtl/crtl_terminal.h;/usr/local/include/crtl/crtl_alloc.h;/usr/local/include/crtl/crtl_log.h;/usr/local/include/crtl/crtl_time.h;/usr/local/include/crtl/crtl_iterator.h;/usr/local/include/crtl/crtl_keyboard.h;/usr/local/include/crtl/crtl_lock.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl" TYPE FILE FILES
    "/work/workspace/crtl/./crtl/crtl_assert.h"
    "/work/workspace/crtl/./crtl/crtl_tree.h"
    "/work/workspace/crtl/./crtl/crtl_file.h"
    "/work/workspace/crtl/./crtl/crtl_types.h"
    "/work/workspace/crtl/./crtl/crtl_list.h"
    "/work/workspace/crtl/./crtl/crtl_random.h"
    "/work/workspace/crtl/./crtl/crtl_mask.h"
    "/work/workspace/crtl/./crtl/crtl_terminal.h"
    "/work/workspace/crtl/./crtl/crtl_alloc.h"
    "/work/workspace/crtl/./crtl/crtl_log.h"
    "/work/workspace/crtl/./crtl/crtl_time.h"
    "/work/workspace/crtl/./crtl/crtl_iterator.h"
    "/work/workspace/crtl/./crtl/crtl_keyboard.h"
    "/work/workspace/crtl/./crtl/crtl_lock.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/easy/operator.h;/usr/local/include/crtl/easy/attribute.h;/usr/local/include/crtl/easy/byteswap.h;/usr/local/include/crtl/easy/endian.h;/usr/local/include/crtl/easy/macro.h;/usr/local/include/crtl/easy/extern.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl/easy" TYPE FILE FILES
    "/work/workspace/crtl/./crtl/easy/operator.h"
    "/work/workspace/crtl/./crtl/easy/attribute.h"
    "/work/workspace/crtl/./crtl/easy/byteswap.h"
    "/work/workspace/crtl/./crtl/easy/endian.h"
    "/work/workspace/crtl/./crtl/easy/macro.h"
    "/work/workspace/crtl/./crtl/easy/extern.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/crtl/bits/crtl_tree_rbtree.h;/usr/local/include/crtl/bits/crtl_list.h;/usr/local/include/crtl/bits/crtl_mask.h;/usr/local/include/crtl/bits/crtl_lock_semaphore.h;/usr/local/include/crtl/bits/crtl_types_ctype.h;/usr/local/include/crtl/bits/crtl_assert_backtrace.h;/usr/local/include/crtl/bits/crtl_time_def.h;/usr/local/include/crtl/bits/crtl_types_bits_set.h")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/usr/local/include/crtl/bits" TYPE FILE FILES
    "/work/workspace/crtl/./crtl/bits/crtl_tree_rbtree.h"
    "/work/workspace/crtl/./crtl/bits/crtl_list.h"
    "/work/workspace/crtl/./crtl/bits/crtl_mask.h"
    "/work/workspace/crtl/./crtl/bits/crtl_lock_semaphore.h"
    "/work/workspace/crtl/./crtl/bits/crtl_types_ctype.h"
    "/work/workspace/crtl/./crtl/bits/crtl_assert_backtrace.h"
    "/work/workspace/crtl/./crtl/bits/crtl_time_def.h"
    "/work/workspace/crtl/./crtl/bits/crtl_types_bits_set.h"
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
FILE(INSTALL DESTINATION "/usr/local/include/crtl/network" TYPE FILE FILES "/work/workspace/crtl/./crtl/network/byteorder.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/work/workspace/crtl/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/work/workspace/crtl/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
