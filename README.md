# cppnet

`cppnet` is a C++-based networking library designed to facilitate the creation of servers capable of handling both JSON and Protobuf data formats. It provides a modular architecture with components for parsing, routing, and handling network requests.

## Features

*   JSON and Protobuf support
*   Modular design
*   Easy to use

## Project Structure
The project is organized into several key components:
### Layers
This project follows layered architecture pattern.
*   **Parsing Layer**: Responsible for parsing incoming HTTP requests.
*   **Routing Layer**: Directs requests to appropriate handlers.
*   **Business Logic Handling Layer**: Contains handlers for different API endpoints
*   **Socket Layer**: (Currently Remaining) Manages network communication using `asio` or similar C++ networking library

## Repository Structure
Below is the structure of the repository:
```text
cppnet/
├── CMakeLists.txt
├── .gitignore
├── Dockerfile
├── include/
│   └── http/ 
│       ├── handlers/ 
│       │   ├── base_handler.h 
│       │   └── json_handler.h 
│       ├── parser/ 
│       │   ├── callbacks.h 
│       │   ├── parser.h 
│       │   └── utils.h 
│       ├── request.h 
│       ├── router.h 
│       └── types.h 
└── src/
    └── http/ 
        └── parser/ 
            ├── callbacks.cpp 
            ├── parser.cpp 
            └── utils.cpp 
└── tests/
    └── http/ 
        └── parser/ 
            ├── test_parser_complex.cpp 
            ├── test_parser_gtests.cpp 
            └── test_parser_simple.cpp 
    └── handler/ 
        ├── post_delete_test.cpp 
        ├── post_patch_test.cpp 
        └── post_put_test.cpp
```

### File Organization:
*   **`include/`**: Contains header files defining the interfaces for the parser, router, handlers, and any other public APIs.
    *   It is expected to find header files that expose the functionalities of the components, facilitating their integration.
        *   **`router.h`**: Defines the `Router` class, responsible for mapping incoming HTTP requests to the appropriate handler functions. It includes the `RouteKey` struct for identifying routes and uses `std::unordered_map` for efficient route lookups.
        *   **`request.h`**: Defines the `Request` class, which encapsulates all the information about an incoming HTTP request, such as the method, URL, headers, and body. It provides utility functions for accessing header and query parameter values.
        *   **`types.h`**: Defines the enums `Method` for HTTP methods (GET, POST, etc.), `Version` for HTTP versions, and `StatusCode` for HTTP status codes. It also defines type aliases for headers and query parameters.
        *   **`handlers/`**: Contains the base class and implementations for request handlers.
            *   **`base_handler.h`**: Defines the abstract `BaseHandler` class, which serves as the base class for all handlers. It specifies the `handle` method that derived classes must implement to process requests and return responses.
        *   **`parser/`**: Contains the components responsible for parsing HTTP requests.
            *   **`callbacks.h`**: Declares callback functions that are invoked by the `llhttp` parser at various stages of parsing, such as when the method, URL, headers, and body are parsed.
            *   **`parser.h`**: Defines the `Parser` class, which uses the `llhttp` library to parse HTTP requests. It manages the parser state and provides access to the parsed `Request` object.
            *   **`utils.h`**: Provides utility functions for URL decoding, query string parsing, header normalization, and string trimming.

        *   **`types.h`**: Defines the enums `Method` for HTTP methods (GET, POST, etc.), `Version` for HTTP versions, and `StatusCode` for HTTP status codes. It also defines type aliases for headers and query parameters.
        *   **`handlers/`**: Contains the base class and implementations for request handlers.
        *   **`parser/`**: Contains the components responsible for parsing HTTP requests.
            *   **`callbacks.h`**: Declares callback functions that are invoked by the `llhttp` parser at various stages of parsing, such as when the method, URL, headers, and body are parsed.
            *   **`parser.h`**: Defines the `Parser` class, which uses the `llhttp` library to parse HTTP requests. It manages the parser state and provides access to the parsed `Request` object.
            *   **`utils.h`**: Provides utility functions for URL decoding, query string parsing, header normalization, and string trimming.

*   **`src/`**: Contains the source code for the components mentioned above.  Notably includes `src/http/parser/parser.cpp`, `src/http/request.cpp`, `src/http/parser/callbacks.cpp`, and `src/http/parser/utils.cpp` which form the HTTP parsing functionality.
    *   This directory houses the implementations of the core functionalities, particularly focusing on HTTP request parsing.
        *   **`request.cpp`**: Implements the methods of the `Request` class, such as `get_header` and `get_query_param`, which provide convenient access to header and query parameter values.
        *   **`parser/callbacks.cpp`**: Implements the callback functions that are invoked by the `llhttp` parser. These functions populate the `Request` object with data parsed from the HTTP request.
        *   **`parser/parser.cpp`**: Implements the `Parser` class, which uses the `llhttp` library to parse HTTP requests. It manages the parser state, initializes `llhttp`, feeds data to the parser, and provides access to the parsed `Request` object.
        *   **`parser/utils.cpp`**: Implements the utility functions for URL decoding (`url_decode`), query string parsing (`parse_query_string`), header normalization (`normalize_header_field`), and string trimming (`trim`).

*   **`tests/`**: Contains unit tests for the various components. Includes gtests and simple tests of handlers.
    *   This directory ensures the reliability and correctness of the individual components through dedicated unit tests.
        *   **`http/parser/`**: Contains test files for the HTTP parser.
            *   **`test_parser_simple.cpp`**: Provides a basic test case for the HTTP parser using a simple HTTP request.
            *   **`test_parser_complex.cpp`**: Offers a more complex test case with various headers, query parameters, and a JSON body.
            *   **`test_parser_gtests.cpp`**: Uses Google Test (gtest) framework to define a set of test cases for the HTTP parser, covering different HTTP methods, versions, headers, and query parameters.
        *   **`handler/`**: Contains test files for the request handlers.
            *   **`post_put_test.cpp`**: Tests the `POST` and `PUT` handlers for user management, verifying the storage and retrieval of user data.
            *   **`post_patch_test.cpp`**: Tests the `PATCH` handler
            *   **`post_delete_test.cpp`**: Tests the `DELETE` handler

*   **`Dockerfile`**: Defines the environment and dependencies required to build and run the application within a container.
    *   Ensures consistency across different deployment environments by encapsulating the build process and dependencies.

*   **`CMakeLists.txt`**: Specifies the build process, dependencies, and executables to be built using CMake.
    *   CMake uses this file to manage the build process, link libraries, and create the necessary executables.

*   **.gitignore**: Lists the files and directories that Git should ignore.



    *   Prevents unnecessary files, such as build artifacts and temporary files, from being added to the repository.
*   **.git/**: Contains the Git repository metadata and version control information.
    *   This hidden directory is crucial for version control, tracking changes, and managing the project's history.

## Building the Project

The project uses CMake for building. Here's how to build it:

1.  **Clone the repository:**
    ```bash
    git clone <repository_url>
    cd cppnet
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Run CMake:**
    ```bash
    cmake ..
    ```

4.  **Build the project:**
    ```bash
    make
    ```


## Usage

To start using `cppnet` library you will need to complete Socket Layer implementation to make it fully functional.


To use `cppnet`, you will need to:

1.  Implement the socket layer to handle network connections.


2.  Define the routes in the router to map request types/paths to specific handlers.

3.  Create handlers to process the data and generate appropriate responses.

## Running Tests

The project includes several test executables:

*   `test_parser_simple`
*   `test_parser_complex`
*   `test_parser_gtests`
*   `test_post_delete`
*   `test_post_patch`
*   `test_post_put`

To run the tests, execute the corresponding binary from the `build` directory. For example:

```bash
./test_parser_gtests
```