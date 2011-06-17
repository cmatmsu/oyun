cmake_minimum_required (VERSION 2.8)
project (Oyun)

set (OYUN_VERSION "2.0.b3")
set (CPACK_PACKAGE_VERSION_MAJOR "2")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "b3")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "Oyun 2.0.b3")


##########
# Sub-directories
##########
add_subdirectory (tools/hhp2cached)
add_subdirectory (doc/manual)
add_subdirectory (lib/CppUnitLite)
add_subdirectory (src)


##########
# ChangeLog
##########
add_custom_target (changelog_file ${CMAKE_CURRENT_SOURCE_DIR}/build/gitlog2changelog.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


##########
# NEWS file
##########
add_custom_target (news_file pandoc -f markdown -t plain -o ${CMAKE_CURRENT_SOURCE_DIR}/NEWS ${CMAKE_CURRENT_SOURCE_DIR}/doc/manual/gh-wiki/Release-Notes.md
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


##########
# Install root-level files to the documentation directory, if we're on *nix
##########
if (NOT APPLE AND NOT WIN32 AND NOT MSVC)
  install (FILES AUTHORS COPYING ChangeLog ChangeLog-2009 ChangeLog-2008 ChangeLog-2007
    ChangeLog-2006 ChangeLog-2005 NEWS README THANKS
    DESTINATION share/doc/oyun)
endif()


##########
# Configure installation packages
##########
include (InstallRequiredSystemLibraries)

set (CPACK_PACKAGE_VERSION ${OYUN_VERSION})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Oyun evolutionary game-theory simulator")
set (CPACK_PACKAGE_VENDOR "Charles Pence")
set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
if (WIN32 OR MSVC)
  set (CPACK_PACKAGE_EXECUTABLES "oyun" "Oyun")
  set (CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/build\\\\oyun.ico")
  set (CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\oyun.exe")
  set (CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_INSTALL_DIRECTORY})
  set (CPACK_NSIS_HELP_LINK "http://charlespence.net/oyun/")
  set (CPACK_NSIS_URL_INFO_ABOUT "http://charlespence.net/")
  set (CPACK_NSIS_CONTACT "charles@charlespence.net")
  set (CPACK_NSIS_MODIFY_PATH ON)
  set (CPACK_GENERATOR "NSIS")
  set (CPACK_PACKAGE_FILE_NAME "oyun-${OYUN_VERSION}-win32")
elseif (APPLE)
  set (CPACK_GENERATOR "DragNDrop")
  set (CPACK_PACKAGE_FILE_NAME "oyun-${OYUN_VERSION}-mac")
else ()
  set (CPACK_STRIP_FILES "bin/oyun")
  set (CPACK_SOURCE_STRIP_FILES "")
  set (CPACK_GENERATOR "TBZ2")
  set (CPACK_PACKAGE_FILE_NAME "oyun-${OYUN_VERSION}-linux")
endif()
set (CPACK_SOURCE_PACKAGE_FILE_NAME "oyun-${OYUN_VERSION}")
set (CPACK_SOURCE_GENERATOR "TBZ2;TGZ")

include (CPack)
