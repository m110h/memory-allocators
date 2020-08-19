cmake_minimum_required(VERSION 3.10)

project(HeapAllocator)

file(GLOB HEAPALLOCATOR_SOURCES_LIST "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB HEAPALLOCATOR_HEADERS_LIST "${PROJECT_SOURCE_DIR}/*.h")

set(HEAPALLOCATOR_NAME "heapallocator-example-dbg")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(HEAPALLOCATOR_NAME "heapallocator-example")
endif()

add_executable(${HEAPALLOCATOR_NAME} ${HEAPALLOCATOR_SOURCES_LIST} ${HEAPALLOCATOR_HEADERS_LIST})

set_target_properties(${HEAPALLOCATOR_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${HEAPALLOCATOR_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(${HEAPALLOCATOR_NAME} PRIVATE cxx_std_17)

target_link_libraries(${HEAPALLOCATOR_NAME} ${LIBRARY_NAME})
