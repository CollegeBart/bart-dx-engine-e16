# Install script for directory: D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletSource

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/BULLET_PHYSICS")
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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/bullet" TYPE FILE FILES
    "D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletSource/UseBullet.cmake"
    "D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletBuild/BulletConfig.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletBuild/examples/cmake_install.cmake")
  include("D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletBuild/Extras/cmake_install.cmake")
  include("D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletBuild/src/cmake_install.cmake")
  include("D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletBuild/test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/Users/1530025/Desktop/bart-dx-engine-e16/bart-dx-engine-e16/bulletBuild/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
