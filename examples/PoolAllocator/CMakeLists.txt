cmake_minimum_required(VERSION 3.10)

project(PoolAllocator)

file(GLOB POOLALLOCATOR_SOURCES_LIST "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB POOLALLOCATOR_HEADERS_LIST "${PROJECT_SOURCE_DIR}/*.h")

set(POOLALLOCATOR_NAME "poolallocator-example-dbg")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(POOLALLOCATOR_NAME "poolallocator-example")
endif()

add_executable(${POOLALLOCATOR_NAME} ${POOLALLOCATOR_SOURCES_LIST} ${POOLALLOCATOR_HEADERS_LIST})

set_target_properties(${POOLALLOCATOR_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${POOLALLOCATOR_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(${POOLALLOCATOR_NAME} PRIVATE cxx_std_17)

target_link_libraries(${POOLALLOCATOR_NAME} ${LIBRARY_NAME})
