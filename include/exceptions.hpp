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

    // Exception for when a node is not found in the graph
    class NodeNotFoundException : public GraphException {
    public:
        NodeNotFoundException(const std::string& message = "Node not found") : GraphException(message) {}
    };

    // Exception for when an edge is not found
    class EdgeNotFoundException : public GraphException {
    public:
        EdgeNotFoundException(const std::string& message = "Edge not found") : GraphException(message) {}
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

    // Exception for when a cycle is detected in a graph
    // and the algorithm doesn't support it
    class CycleDetectedException : public GraphException {
    public:
        CycleDetectedException(const std::string& message = "Cycle detected") : GraphException(message) {}
    };

    // Exception for when a weighted graph is required
    // but the graph is unweighted
    class UnweightedGraphException : public GraphException {
    public:
        UnweightedGraphException(const std::string& message = "Graph must have weights") : GraphException(message) {}
    };

    // Exception for when an unweighted graph is required
    // but the graph has weights
    class WeightedGraphException : public GraphException {
    public:
        WeightedGraphException(const std::string& message = "Graph must not have weights") : GraphException(message) {}
    };

}

#endif
