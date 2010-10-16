
##########
# Find wxWidgets
##########
find_package (wxWidgets REQUIRED base)
include (${wxWidgets_USE_FILE})


##########
# Get the list of sources
##########
file (GLOB CPPUNIT_SOURCE *.cpp)
file (GLOB CPPUNIT_HEADERS *.h)


##########
# Build the library
##########
add_library (cppunitlite STATIC ${CPPUNIT_SOURCE} ${CPPUNIT_HEADERS})
target_link_libraries (cppunitlite ${wxWidgets_LIBRARIES})
