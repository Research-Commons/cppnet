cmake_minimum_required(VERSION 3.10)
project(cppnet)

include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_parser
    tests/http/parser/test_parser_simple.cpp
    tests/http/parser/test_parser_complex.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # Add any other implementation files your parser depends on
)
target_link_libraries(test_parser llhttp)
