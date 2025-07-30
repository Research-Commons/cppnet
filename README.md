# cppnet

`cppnet` is a C++-based networking library designed to facilitate the creation of servers capable of handling both JSON and Protobuf data formats. It provides a modular architecture with components for parsing, routing, and handling network requests.

## Project Structure
The project is organized into several key components:

*   **Parser:** This component is responsible for parsing incoming data, determining whether it is in JSON or Protobuf format, and converting it into a usable data structure.
*   **Router:** The router directs incoming requests to the appropriate handler based on the request type or path. It acts as a central dispatch point for network traffic.
*   **Handler:** Handlers are the functional units that process the parsed data and generate responses. Different handlers can be created for different request types or API endpoints.
*   **Socket Layer:** (Currently Remaining) This layer will manage the actual network communication, including listening for incoming connections, receiving data, and sending responses.

### File Organization:

*   **`include/`**: Contains header files defining the interfaces for the parser, router, handlers, and any other public APIs.
*   **`src/`**: Contains the source code for the components mentioned above.  Notably includes `src/http/parser/parser.cpp`, `src/http/request.cpp`, `src/http/parser/callbacks.cpp`, and `src/http/parser/utils.cpp` which form the HTTP parsing functionality.
*   **`tests/`**: Contains unit tests for the various components. Includes gtests and simple tests of handlers.

## Usage

To use `cppnet`, you will need to:

1.  Implement the socket layer to handle network connections.

2.  Define the routes in the router to map request types/paths to specific handlers.

3.  Create handlers to process the data and generate appropriate responses.

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
## Contributing

Contributions to `cppnet` are welcome! Please fork the repository, create a feature branch, and submit a pull request with your changes.

## License

[Add License Information Here]

## Docker

A `Dockerfile` is provided to build the project in a containerized environment.  This sets up all dependencies, including `llhttp` and `nlohmann_json`.

## Git Log

*   Removed the mains from Cmake