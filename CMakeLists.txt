cmake_minimum_required(VERSION 3.10)
project(cppnet)

include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_parser tests/http/parser/test_parser.cpp)
target_link_libraries(test_parser llhttp)
