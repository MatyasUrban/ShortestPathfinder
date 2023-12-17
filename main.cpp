#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include <fstream>
#include <future>
#include <iomanip>
#include <cstdlib>
#include <cerrno>
#include <charconv>
#include "Graph.h"
std::mutex printMutex;
/**
 * Enum representing the types of used shortest path algorithms.
 */
enum class Algorithm {
    Dijkstra,
    BellmanFord
};

/**
 * Converts an Algorithm enum to its string representation.
 *
 * @param algo The algorithm enum (Dijkstra or BellmanFord).
 * @return The string representation of the algorithm.
 */
std::string algorithmToString(Algorithm algo) {
    switch (algo) {
        case Algorithm::Dijkstra:
            return "Dijkstra";
        case Algorithm::BellmanFord:
            return "Bellman-Ford";
        default:
            return "Unknown Algorithm";
    }
}

/**
 * Executes a shortest path algorithm on a given graph and measures the execution time.
 *
 * @param g Reference to the graph on which the algorithm is run.
 * @param start The starting vertex index.
 * @param end The ending (target) vertex index.
 * @param algo The algorithm to run (Dijkstra or BellmanFord).
 * @return The execution time in microseconds.
 */
long long runShortestPathAlgorithm(Graph& g, int start, int end, Algorithm algo) {
    std::stringstream ss;
    ss << "[Thread " << std::this_thread::get_id() << "] ";
    std::string threadPrefix = ss.str();

    {
        std::lock_guard<std::mutex> guard(printMutex);
        std::cout << threadPrefix <<  "Shortest-path for graph (v=" << g.size
                  << ", e=" << g.edgeCount << ") | Initial vertex: "
                  << start << " | Destination vertex: " << end << std::endl;
    }

    std::string algoName = algorithmToString(algo);
    std::pair<std::vector<int>, int> result;

    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << threadPrefix <<  "Finding path using " << algoName << " ...\n" << std::endl;

    if (algo == Algorithm::Dijkstra) {
        result = g.shortestPathDijkstra(start, end);
    } else if (algo == Algorithm::BellmanFord) {
        result = g.shortestPathBellmanFord(start, end);
    } else {
        throw std::invalid_argument("Unrecognized algorithm");
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    std::cout << threadPrefix << "Shortest path: ";
    for (int v : result.first) {
        std::cout << v << " -> ";
    }
    std::cout << "\b\b\b \n"; // Erase the last arrow
    std::cout << threadPrefix << "Path length: " << result.second << std::endl;
    std::cout << threadPrefix << algoName << " found the path in " << duration << " microseconds" << std::endl << std::endl;
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}

/**
 * Runs a single experiment by executing both Dijkstra's and Bellman-Ford's algorithms on a given graph.
 *
 * @param graph The graph on which the algorithms are executed.
 * @return A pair of long long values representing the execution times of Dijkstra's and Bellman-Ford's algorithms, respectively.
 */
std::pair<long long, long long> oneExperiment(Graph& graph) {
    try {
        std::promise<long long> dijkstraPromise;
        std::future<long long> dijkstraFuture = dijkstraPromise.get_future();
        std::thread dijkstraThread([&]() {
            long long time = runShortestPathAlgorithm(graph, 0, 1, Algorithm::Dijkstra);
            dijkstraPromise.set_value(time);
        });

        std::promise<long long> bellmanFordPromise;
        std::future<long long> bellmanFordFuture = bellmanFordPromise.get_future();
        std::thread bellmanFordThread([&]() {
            long long time = runShortestPathAlgorithm(graph, 0, 1, Algorithm::BellmanFord);
            bellmanFordPromise.set_value(time);
        });

        dijkstraThread.join();
        bellmanFordThread.join();

        return {dijkstraFuture.get(), bellmanFordFuture.get()};
    } catch (const std::exception& e) {
        std::cerr << "Error in oneExperiment: " << e.what() << std::endl;
        return {-1, -1};
    }
}

/**
 * Executes a set of default experiments using predefined graph files.
 * The function iterates through a list of filenames, sets up graphs from these files,
 * and runs experiments to compare the performance of Dijkstra's and Bellman-Ford's algorithms.
 */
void defaultExperiment() {
    std::vector<std::string> filenames = {"1k.txt", "2k.txt", "3k.txt", "5k.txt", "8k.txt", "13k.txt"};
    std::vector<int> graph_sizes = {1000, 2000, 3000, 5000, 8000, 13000};
    std::vector<long long> dijkstraTimes, bellmanFordTimes;

    for (const auto & filename : filenames) {

        std::cout << "Running experiment from file: " << filename << std::endl;
        std::cout << "=================================" << std::endl;
        Graph graphFromFile = Graph::setUpFromText(filename);
        auto times = oneExperiment(graphFromFile);
        dijkstraTimes.push_back(times.first);
        bellmanFordTimes.push_back(times.second);
    }

    std::cout << "\nExperiment Results:\n";
    std::cout << std::left << std::setw(12) << "Experiment" << " | "
              << std::setw(10) << "File" << " | "
              << std::setw(6) << "V" << " | "
              << std::setw(6) << "E" << " | "
              << std::setw(12) << "Dijkstra" << " | "
              << "Bellman-Ford\n";
    std::cout << "-------------|------------|--------|--------|--------------|-------------\n";
    for (size_t i = 0; i < filenames.size(); ++i) {
        std::cout << std::left << std::setw(12) << "#" + std::to_string(i + 1) << " | "
                  << std::setw(10) << filenames[i] << " | "
                  << std::setw(6) << graph_sizes[i] << " | "
                  << std::setw(6) << graph_sizes[i] * 2 << " | "
                  << std::setw(12) << dijkstraTimes[i] << " | "
                  << bellmanFordTimes[i] << "\n";
    }

    std::cout << "\nNotes:\n";
    std::cout << "1. Results for Dijkstra and Bellman-Ford are displayed in microseconds.\n";
    std::cout << "2. V := number of vertices, E := number of edges.\n";
    std::cout << "3. Graphs are directed and weighted (1-10).\n";
    std::cout << "4. All vertices have 2 outgoing edges.\n";
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "Error: No arguments provided. Use --help for usage information." << std::endl;
        return 1;
    }
    std::string arg = argv[1];
    if (arg == "--help") {
        std::cout << "Graph Pathfinding Experiment Help\n";
        std::cout << "--------------------------------\n";
        std::cout << "[IMPORTANT]\n";
        std::cout << "!!! Before running, move the following files to \"cmake-build-debug\" folder: ";
        std::cout << "\"1k.txt\", \"2k.txt\", \"3k.txt\", \"5k.txt\", \"8k.txt\", \"13k.txt\", \"end.txt\"\n\n";

        std::cout << "Usage:\n";
        std::cout << "  --help: Display this help message.\n";
        std::cout << "  --default: Run a set of default experiments comparing Dijkstra and Bellman-Ford algorithms.\n";
        std::cout << "  --custom -v [num] -d [num]: Run a custom experiment with a generated graph.\n\n";

        std::cout << "Options for --custom:\n";
        std::cout << "  -v [num]: Specify the number of vertices in the graph. Must be greater than or equal to 50.\n";
        std::cout << "  -d [num]: Specify the degree of outgoing edges for each vertex. Must be between 1 and 10.\n";
        std::cout << "Example for --custom:\n";
        std::cout << "  --custom -v 50 -d 3\n";
        std::cout << "  (will generate a graph with 50 vertices, each having 3 outgoing edges pointing to random vertices within the graph)\n\n";

        std::cout << "The custom experiment generates a graph with the specified parameters, runs pathfinding algorithms, and exports the graph to out.txt.\n";
    } else if (arg == "--default") {
        std::cout << "Running the default set of experiments:" << std::endl;
        std::cout << ".......................................\n" << std::endl;
        defaultExperiment();
    } else if (arg == "--custom") {
        int vertices = -1;
        int degree = -1;
        char *end;

        for (int i = 2; i < argc; i += 2) {
            std::string param = argv[i];
            if (param == "-v" && i + 1 < argc) {
                errno = 0;
                vertices = std::strtol(argv[i + 1], &end, 10);
                if (errno != 0 || *end != '\0') {
                    vertices = -1;
                    break;
                }
            } else if (param == "-d" && i + 1 < argc) {
                errno = 0;
                degree = std::strtol(argv[i + 1], &end, 10);
                if (errno != 0 || *end != '\0') {
                    degree = -1;
                    break;
                }
            }
        }

        if (vertices >= 50 && degree >= 1 && degree <= 10) {
            std::cout << "Running your custom experiment:" << std::endl;
            std::cout << "...............................\n" << std::endl;
            Graph graph = Graph::generateGraph(vertices, degree);
            graph.convertToText();
            oneExperiment(graph);
            std::cout << "Custom experiment completed. You can find the graph in out.txt." << std::endl;
        } else {
            std::cerr << "Invalid arguments for custom experiment. Ensure v >= 50 and 1 <= d <= 10." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Invalid argument. Use --help for usage information." << std::endl;
        return 1;
    }

    std::ifstream endFile("end.txt");
    if (endFile.is_open()) {
        std::string line;
        while (getline(endFile, line)) {
            std::cout << line << std::endl;
        }
        endFile.close();
    } else {
        std::cerr << "Unable to open end.txt" << std::endl;
        return 1;
    }

    return 0;
}