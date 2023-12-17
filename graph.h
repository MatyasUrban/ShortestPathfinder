#ifndef SHORTEST_PATH_GRAPH_H
#define SHORTEST_PATH_GRAPH_H

#include "vertex.h"
#include <vector>
#include <set>
#include <limits>
#include <stdexcept>

/**
 * Represents a directed graph.
 *
 * The graph is represented as a vector of Vertex objects. It supports operations like adding an edge,
 * finding the shortest path using Dijkstra's or Bellman-Ford algorithm, converting the graph to text format,
 * and initializing the graph from a text file.
 */
class Graph {
public:
    std::vector<Vertex> vertices;
    int size;
    int edgeCount;

    Graph(int numberOfVertices);
    void addEdge(int u, int v, int w);
    std::pair<std::vector<int>, int> shortestPathDijkstra(int start, int end);
    std::pair<std::vector<int>, int> shortestPathBellmanFord(int start, int end);
    void convertToText();
    static Graph setUpFromText(const std::string& filename);
    static Graph generateGraph(int numberOfVertices, int desiredOutgoingDegree);
};

#endif //SHORTEST_PATH_GRAPH_H
