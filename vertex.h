#ifndef SHORTEST_PATH_VERTEX_H
#define SHORTEST_PATH_VERTEX_H

#include <list>
#include <utility>

/**
 * Represents a vertex in a graph.
 *
 * Each vertex has an index and a list of outgoing edges. Each edge is represented as a pair
 * where the first element is the index of the neighbor vertex and the second element is the weight of the edge.
 */
class Vertex {
public:
    int index;
    std::list<std::pair<int, int>> edges;
    Vertex(int idx);
};

#endif //SHORTEST_PATH_VERTEX_H
