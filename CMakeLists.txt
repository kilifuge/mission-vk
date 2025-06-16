cmake_minimum_required(VERSION 3.14)
project(metrics_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(GTest REQUIRED)
include(GoogleTest)

add_library(metrics_lib STATIC
    metric_collector.cpp
    metric_writer.cpp
)

target_include_directories(metrics_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(tests
    tests/tests.cpp
)

target_link_libraries(tests
    metrics_lib
    GTest::gtest_main
)

gtest_discover_tests(tests)