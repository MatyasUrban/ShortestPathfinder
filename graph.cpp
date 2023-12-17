#include "graph.h"
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <limits>
#include <iostream>
#include <unordered_set>

/**
 * Constructor for Graph.
 *
 * @param numberOfVertices Number of vertices in the graph.
 */
Graph::Graph(int numberOfVertices) : size(numberOfVertices), edgeCount(0) {
    for (int i = 0; i < numberOfVertices; ++i) {
        vertices.emplace_back(i);
    }
}

/**
 * Adds a directed edge from vertex u to vertex v with weight w.
 *
 * @param u Index of the starting vertex.
 * @param v Index of the ending vertex.
 * @param w Weight of the edge.
 */
void Graph::addEdge(int u, int v, int w) {
    vertices[u].edges.emplace_back(v, w);
    edgeCount++;
}

/**
 * Finds the shortest path from the start vertex to the end vertex using Dijkstra's algorithm.
 *
 * @param start Index of the starting vertex.
 * @param end Index of the ending vertex.
 * @return A pair containing the shortest path (as a vector of vertex indices) and its total length.
 */
std::pair<std::vector<int>, int> Graph::shortestPathDijkstra(int start, int end) {
    std::vector<double> dist(size, std::numeric_limits<double>::max());
    std::vector<int> prev(size, -1);

    auto comp = [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, decltype(comp)> queue(comp);

    dist[start] = 0;
    queue.emplace(start, 0);

    while (!queue.empty()) {
        int u = queue.top().first;
        queue.pop();

        if (u == end) break;

        for (const auto& edge : vertices[u].edges) {
            int v = edge.first;
            double weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                queue.emplace(v, dist[v]);
            }
        }
    }

    if (prev[end] == -1) throw std::runtime_error("No viable path for from initial to destination vertex has been found.");

    std::vector<int> path;
    int pathLength = 0;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
        if (prev[at] != -1) {
            pathLength += static_cast<int>(dist[at] - dist[prev[at]]);
        }
    }
    std::reverse(path.begin(), path.end());
    return {path, pathLength};
}

/**
 * Finds the shortest path from the start vertex to the end vertex using Bellman-Ford algorithm.
 *
 * @param start Index of the starting vertex.
 * @param end Index of the ending vertex.
 * @return A pair containing the shortest path (as a vector of vertex indices) and its total length.
 */
std::pair<std::vector<int>, int> Graph::shortestPathBellmanFord(int start, int end) {
    std::vector<int> distance(size, INT_MAX);
    std::vector<int> predecessor(size, -1);

    distance[start] = 0;
    for (int i = 0; i < size - 1; i++) {
        for (int u = 0; u < size; u++) {
            for (const auto& edge : vertices[u].edges) {
                int v = edge.first;
                int weight = edge.second;
                if (distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    predecessor[v] = u;
                }
            }
        }
    }
    std::vector<int> path;
    for (int at = end; at != -1; at = predecessor[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] != start) {
        path.clear();
    }

    int pathLength = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        for (const auto& edge : vertices[u].edges) {
            if (edge.first == v) {
                pathLength += edge.second;
                break;
            }
        }
    }
    return {path, pathLength};
}

/**
 * Converts the graph to a text representation and writes it to "out.txt".
 */
void Graph::convertToText() {
    std::ofstream outFile("out.txt");
    outFile << vertices.size() << std::endl;
    for (const auto& vertex : vertices) {
        outFile << vertex.index << " " << vertex.edges.size();
        for (const auto& edge : vertex.edges) {
            outFile << " " << edge.first << " " << edge.second;
        }
        outFile << std::endl;
    }
    outFile.close();
}

/**
 * Reads a graph from a text file and creates a Graph object.
 *
 * @param filename Name of the file containing the graph representation.
 * @return A Graph object constructed from the file.
 */
Graph Graph::setUpFromText(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file '" << filename << "'" << std::endl;
        throw std::runtime_error("Failed to open file '" + filename + "'");
    }

    int totalVertices;
    inFile >> totalVertices;
    Graph graph(totalVertices);

    std::string line;
    std::getline(inFile, line); // Skip to the next line

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        int vertexIndex, numberOfEdges;
        iss >> vertexIndex >> numberOfEdges;
        for (int i = 0; i < numberOfEdges; ++i) {
            int direction;
            int weight;
            iss >> direction >> weight;
            graph.addEdge(vertexIndex, direction, weight);
        }
    }

    inFile.close();
    return graph;
}

/**
 * Generates a random graph with a specified number of vertices and a desired degree of outgoing edges.
 *
 * @param numberOfVertices Number of vertices in the graph.
 * @param desiredOutgoingDegree Desired number of outgoing edges for each vertex.
 * @return A Graph object generated based on the given parameters.
 */
Graph Graph::generateGraph(int numberOfVertices, int desiredOutgoingDegree) {
    int edgeGenerationFactor = 4;
    Graph graph(numberOfVertices);
    int k = numberOfVertices * desiredOutgoingDegree * edgeGenerationFactor;
    std::vector<int> directions(k);
    std::vector<int> weights(k);

    for (int i = 0; i < numberOfVertices; ++i) {
        for (int j = 0; j < desiredOutgoingDegree * edgeGenerationFactor; ++j) {
            directions[i * desiredOutgoingDegree * edgeGenerationFactor + j] = i;
            weights[i * desiredOutgoingDegree * edgeGenerationFactor + j] = (j % 10) + 1;
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(directions.begin(), directions.end(), g);
    std::shuffle(weights.begin(), weights.end(), g);

    int currentVertexIndex = 0;
    int edgesAddedToVertex = 0;
    int currentPairIndex = 0;
    std::unordered_set<int> directionsAddedToCurrentVertex;

    while (currentVertexIndex < numberOfVertices) {
        int direction = directions[currentPairIndex];
        int weight = weights[currentPairIndex];

        if (direction != currentVertexIndex &&
            directionsAddedToCurrentVertex.find(direction) == directionsAddedToCurrentVertex.end()) {
            graph.addEdge(currentVertexIndex, direction, weight);
            edgesAddedToVertex++;
            directionsAddedToCurrentVertex.insert(direction);
        }

        currentPairIndex++;

        if (edgesAddedToVertex == desiredOutgoingDegree) {
            currentVertexIndex++;
            edgesAddedToVertex = 0;
            directionsAddedToCurrentVertex.clear();
        }
    }

    return graph;
}