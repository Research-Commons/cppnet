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

# --------------------------------------------------------
# JSON Router/Server Demo (main.cpp) Target
# --------------------------------------------------------
add_executable(run_json_router_demo
    src/http/main.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # router and handlers:
    # src/http/router.cpp           # If you add this source file in future
    # src/http/handlers/json_handler.cpp  # If you implement it as .cpp, otherwise comment this line out
    # Add more .cpp files for new handlers as required
)
target_link_libraries(run_json_router_demo
    llhttp
    nlohmann_json::nlohmann_json
)

# ----------------------------------------
# Handler method sequence test executables
# ----------------------------------------

add_executable(test_post_delete
    tests/handler/post_delete_test.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # add other .cpp as required
)
target_link_libraries(test_post_delete llhttp nlohmann_json::nlohmann_json)

add_executable(test_post_patch
    tests/handler/post_patch_test.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # add other .cpp as required
)
target_link_libraries(test_post_patch llhttp nlohmann_json::nlohmann_json)

add_executable(test_post_put
    tests/handler/post_put_test.cpp
    src/http/parser/parser.cpp
    src/http/request.cpp
    src/http/parser/callbacks.cpp
    src/http/parser/utils.cpp
    # add other .cpp as required
)
target_link_libraries(test_post_put llhttp nlohmann_json::nlohmann_json)

