cmake_minimum_required (VERSION 2.8)
project (LOGOS)

set (LOGOS_VERSION "2.0b2")

##########
# Sub-directories
##########
add_subdirectory (tools/hhp2cached)
add_subdirectory (doc/manual)
add_subdirectory (lib/CppUnitLite)
add_subdirectory (src)
