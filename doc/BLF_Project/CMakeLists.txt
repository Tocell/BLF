project(BL)
cmake_minimum_required(VERSION 3.6)


set(LIB_DIRECTORY "Libs")


# Perform Linux specific configuration
if(UNIX)
  # Set RPath of executable to $ORIGIN if Linux so that libbinlog.so can be found in same directory
  set(CMAKE_INSTALL_RPATH "$ORIGIN")
  set(LIB_DIRECTORY "${LIB_DIRECTORY}/Linux64")
  message("Set Library directory to: ${LIB_DIRECTORY}") 
elseif(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LIB_DIRECTORY "${LIB_DIRECTORY}/Win64")
  else()
    set(LIB_DIRECTORY "${LIB_DIRECTORY}/Win32")
  endif()
endif()

find_library(BINLOG NAMES binlog HINTS ${LIB_DIRECTORY})
find_file(BINLOG_BINARY NAMES binlog.dll libbinlog.so HINTS ${LIB_DIRECTORY})

add_executable(bl
  bl.c)

target_include_directories(bl PRIVATE ../Include)  
target_link_libraries(bl PUBLIC ${BINLOG})

install (TARGETS bl DESTINATION ${CMAKE_INSTALL_PREFIX})
install (FILES ${BINLOG_BINARY} DESTINATION ${CMAKE_INSTALL_PREFIX})