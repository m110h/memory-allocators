cmake_minimum_required(VERSION 3.10)

project(memory-allocators LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Set a default build type if none was specified by option -DCMAKE_BUILD_TYPE=Release
set(DEFAULT_BUILD_TYPE "Debug")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
else()
	message("Configuration: ${CMAKE_BUILD_TYPE}")
endif()

# Check a platform
if(WIN32)
	message("Platform: WIN32")
else()
	message("Platform: UNIX-like OS")
endif()

file(GLOB LIBRARY_SOURCES_LIST "src/*.cpp")
file(GLOB LIBRARY_HEADERS_LIST "includes/*.h")

set(LIBRARY_NAME "allocatorsd")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(LIBRARY_NAME "allocators")
endif()

add_library(${LIBRARY_NAME} ${LIBRARY_HEADERS_LIST} ${LIBRARY_SOURCES_LIST})

set_target_properties(${LIBRARY_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_17)

add_subdirectory("benchmark")
add_subdirectory("examples")
