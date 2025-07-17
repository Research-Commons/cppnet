cmake_minimum_required(VERSION 3.10)
project(cppnet)

# Add include directories
include_directories(/usr/local/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(nlohmann_json 3.2.0 REQUIRED)

# Simple test executable
add_executable(test_parser_simple
    tests/http/parser/test_parser_simple.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # Add more sources as required
)
target_link_libraries(test_parser_simple llhttp nlohmann_json::nlohmann_json)

# Complex test executable
add_executable(test_parser_complex
    tests/http/parser/test_parser_complex.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # Add more sources as required
)
target_link_libraries(test_parser_complex llhttp nlohmann_json::nlohmann_json)

# Google Test executable
add_executable(test_parser_gtests
    tests/http/parser/test_parser_gtests.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # Add more sources as required
)

# Link Google Test libraries
target_link_libraries(test_parser_gtests
    llhttp
    nlohmann_json::nlohmann_json
    gtest
    gtest_main
    pthread
)
