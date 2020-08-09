cmake_minimum_required(VERSION 3.10)

project(FreeListAllocator)

file(GLOB FREELISTALLOCATOR_SOURCES_LIST "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB FREELISTALLOCATOR_HEADERS_LIST "${PROJECT_SOURCE_DIR}/*.h")

set(FREELISTALLOCATOR_NAME "freelistallocator-example-dbg")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(FREELISTALLOCATOR_NAME "freelistallocator-example")
endif()

add_executable(${FREELISTALLOCATOR_NAME} ${FREELISTALLOCATOR_SOURCES_LIST} ${FREELISTALLOCATOR_HEADERS_LIST})

set_target_properties(${FREELISTALLOCATOR_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${FREELISTALLOCATOR_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(${FREELISTALLOCATOR_NAME} PRIVATE cxx_std_17)

target_link_libraries(${FREELISTALLOCATOR_NAME} ${LIBRARY_NAME})
