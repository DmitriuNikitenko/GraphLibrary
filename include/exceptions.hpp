#ifndef GRAPH_LIBRARY_EXCEPTIONS_HPP
#define GRAPH_LIBRARY_EXCEPTIONS_HPP

#include <stdexcept>  
#include <string>     

namespace graph_library {

    // Base class for all exceptions in the library
    class GraphException : public std::runtime_error {
    public:
        GraphException(const std::string& message) : std::runtime_error(message) {}
        virtual ~GraphException() = default;
    };

    // Exception for when a node is null
    class NodeIsNullException : public GraphException {
    public:
        NodeIsNullException(const std::string& message = "Node is null") : GraphException(message) {}
    };

    // The node not found in the list of available ones.
    class NodeNotFoundException : public GraphException {
    public:
        NodeNotFoundException(const std::string& message = "The node not found in the list of available ones") : GraphException(message) {}
    };

    // Exception for when an invalid index is passed
    class InvalidIndexException : public GraphException {
    public:
        InvalidIndexException(const std::string& message = "Invalid index") : GraphException(message) {}
    };

    // Exception for when a file cannot be read
    class FileReadException : public GraphException {
    public:
        FileReadException(const std::string& message) : GraphException(message) {}
    };

    // Exception for parsing errors
    class ParseException : public GraphException {
    public:
        ParseException(const std::string& message) : GraphException(message) {}
    };
}

#endif
