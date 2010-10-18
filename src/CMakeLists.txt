
##########
# Get the source lists
##########
file (GLOB_RECURSE LOGOS_SOURCE *.cpp)
if (MSVC OR WIN32)
  list (APPEND LOGOS_SOURCE "${CMAKE_SOURCE_DIR}/build/logos.rc")
endif()
file (GLOB_RECURSE LOGOS_HEADERS *.h)


##########
# Add CppUnitLite directories
##########
include_directories (${CMAKE_SOURCE_DIR}/lib/CppUnitLite)
link_directories (${CMAKE_BINARY_DIR}/lib/CppUnitLite)


##########
# Link wxWidgets
##########
find_package (wxWidgets REQUIRED base core adv html)
include (${wxWidgets_USE_FILE})


##########
# Link the executable
##########
add_executable (logos ${LOGOS_SOURCE} ${LOGOS_HEADERS})
target_link_libraries (logos cppunitlite ${wxWidgets_LIBRARIES})


##########
# Set executable properties
##########
if (MSVC OR WIN32)
  set_target_properties (logos PROPERTIES WIN32_EXECUTABLE TRUE)
elseif (APPLE)
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE TRUE)
  
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_INFO_STRING "${LOGOS_VERSION}")
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/build/logos.icns")
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "net.charlespence.logos")
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "${LOGOS_VERSION}")
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Logos")
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "${LOGOS_VERSION}")
  set_target_properties (logos PROPERTIES MACOSX_BUNDLE_COPYRIGHT "Copyright (C) 2004-2010 Charles Pence")
endif ()


##########
# Install the executable
##########
install (TARGETS logos 
  RUNTIME DESTINATION bin COMPONENT Runtime
  BUNDLE DESTINATION . COMPONENT Runtime)
