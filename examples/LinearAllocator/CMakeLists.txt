cmake_minimum_required(VERSION 3.10)

project(LinearAllocator)

file(GLOB LINEARALLOCATOR_SOURCES_LIST "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB LINEARALLOCATOR_HEADERS_LIST "${PROJECT_SOURCE_DIR}/*.h")

set(LINEARALLOCATOR_NAME "linearallocator-example-dbg")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(LINEARALLOCATOR_NAME "linearallocator-example")
endif()

add_executable(${LINEARALLOCATOR_NAME} ${LINEARALLOCATOR_SOURCES_LIST} ${LINEARALLOCATOR_HEADERS_LIST})

set_target_properties(${LINEARALLOCATOR_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${LINEARALLOCATOR_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(${LINEARALLOCATOR_NAME} PRIVATE cxx_std_17)

target_link_libraries(${LINEARALLOCATOR_NAME} ${LIBRARY_NAME})
