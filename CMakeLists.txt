cmake_minimum_required(VERSION 3.10)
project(cppnet)

# Add include directories
include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Simple test executable
add_executable(test_parser_simple
    tests/http/parser/test_parser_simple.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # Add more sources as required
)
target_link_libraries(test_parser_simple llhttp)

# Complex test executable
add_executable(test_parser_complex
    tests/http/parser/test_parser_complex.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # Add more sources as required
)
target_link_libraries(test_parser_complex llhttp)
