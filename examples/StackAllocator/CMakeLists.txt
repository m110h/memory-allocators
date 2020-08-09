cmake_minimum_required(VERSION 3.10)

project(StackAllocator)

file(GLOB STACKALLOCATOR_SOURCES_LIST "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB STACKALLOCATOR_HEADERS_LIST "${PROJECT_SOURCE_DIR}/*.h")

set(STACKALLOCATOR_NAME "stackallocator-example-dbg")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(STACKALLOCATOR_NAME "stackallocator-example")
endif()

add_executable(${STACKALLOCATOR_NAME} ${STACKALLOCATOR_SOURCES_LIST} ${STACKALLOCATOR_HEADERS_LIST})

set_target_properties(${STACKALLOCATOR_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${STACKALLOCATOR_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(${STACKALLOCATOR_NAME} PRIVATE cxx_std_17)

target_link_libraries(${STACKALLOCATOR_NAME} ${LIBRARY_NAME})
