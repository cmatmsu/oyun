
##########
# Find wxWidgets
##########
find_package (wxWidgets REQUIRED base core html)
include (${wxWidgets_USE_FILE})


##########
# Build the executable
##########
add_executable (hhp2cached hhp2cached.cpp)
target_link_libraries (hhp2cached ${wxWidgets_LIBRARIES})
if (MSVC OR WIN32)
  set_target_properties (hhp2cached PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
