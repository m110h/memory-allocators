cmake_minimum_required(VERSION 3.10)
project(Benchmark)

file(GLOB BENCHMARK_SOURCES_LIST "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB BENCHMARK_HEADERS_LIST "${PROJECT_SOURCE_DIR}/*.h")

set(BENCHMARK_NAME "benchmark-dbg")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(BENCHMARK_NAME "benchmark")
endif()

add_executable(${BENCHMARK_NAME} ${BENCHMARK_SOURCES_LIST} ${BENCHMARK_HEADERS_LIST})

set_target_properties(${BENCHMARK_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${BENCHMARK_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(${BENCHMARK_NAME} PRIVATE cxx_std_17)

target_link_libraries(${BENCHMARK_NAME} ${LIBRARY_NAME})
