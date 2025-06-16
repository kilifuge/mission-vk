cmake_minimum_required(VERSION 3.14)
project(metrics_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include(GoogleTest)

add_library(metrics_lib STATIC
    metric_collector.cpp
    metric_writer.cpp
)

add_executable(tests
    tests/tests.cpp
)

add_executable(http_sim
    tests/http_sim.cpp
)

target_link_libraries(tests
    metrics_lib
    GTest::gtest_main
)

target_link_libraries(http_sim
    metrics_lib
)