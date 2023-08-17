# Install script for directory: /Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/miniupnpc" TYPE FILE FILES
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/miniupnpc.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/miniwget.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/upnpcommands.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/igd_desc_parse.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/upnpreplyparse.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/upnperrors.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/upnpdev.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/miniupnpctypes.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/portlistingparse.h"
    "/Users/traaitt/Documents/GitHub/traaitt/external/miniupnpc/miniupnpc_declspec.h"
    )
endif()

