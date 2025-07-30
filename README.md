# cppnet

`cppnet` is a C++-based networking library designed to facilitate the creation of servers capable of handling both JSON and Protobuf data formats. It provides a modular architecture with components for parsing, routing, and handling network requests.

## Project Structure

The project is organized into several key components:

*   **Parser:** This component is responsible for parsing incoming data, determining whether it is in JSON or Protobuf format, and converting it into a usable data structure.
*   **Router:** The router directs incoming requests to the appropriate handler based on the request type or path. It acts as a central dispatch point for network traffic.
*   **Handler:** Handlers are the functional units that process the parsed data and generate responses. Different handlers can be created for different request types or API endpoints.
*   **Socket Layer:** (Currently Remaining) This layer will manage the actual network communication, including listening for incoming connections, receiving data, and sending responses.

## Usage

To use `cppnet`, you will need to:

1.  Implement the socket layer to handle network connections.
2.  Define the routes in the router to map request types/paths to specific handlers.
3.  Create handlers to process the data and generate appropriate responses.

## Contributing

Contributions to `cppnet` are welcome! Please fork the repository, create a feature branch, and submit a pull request with your changes.

## License

[Add License Information Here]

## Git Log

*   Removed the mains from Cmake