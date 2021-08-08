cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# includes
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("SetupConfigurations")
include("SetupGlobalProperties")
include("SetupGlobalDirectories")
include("SetupVendorLibs")

enable_testing()

project(Structs CXX)
add_subdirectory(src/Structs Structs)
add_subdirectory(src/Tests Tests)

# set startrup project
set_property(DIRECTORY ${STRUCTS_ROOT_DIR} PROPERTY VS_STARTUP_PROJECT Tests)