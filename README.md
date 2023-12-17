# Shortest Pathfinder | Dijkstra vs Bellman-Ford

This is my semester project for the *Programming in C++* [B6B36PCC](https://bilakniha.cvut.cz/cs/predmet4539406.html#gsc.tab=0) course, part of my Software Engineering undergraduate curriculum at the Czech Technical University in Prague, CZ, EU.

Designed, developed and programmed by  [Matyas Urban](https://www.linkedin.com/in/matyasurban/)  in December 2023.

## Table of Contents
1. [Running the Project in CLion](#1-running-the-project-in-clion)
2. [Project Motive](#2-project-motive)
3. [Project Structure](#3-project-structure)
4. [Project Features](#4-project-features)
5. [Implementation Notes](#5-implementation-notes)
6. [Algorithmic Analysis](#6-algorithmic-analysis)
7. [Additional Remarks](#7-additional-remarks)

## 1. Running the Project in CLion

Before running, ensure the following files are in the `cmake-build-debug` folder: `1k.txt`, `2k.txt`, `3k.txt`, `5k.txt`, `8k.txt`, `13k.txt`, `end.txt`.

![](/Users/m.urban/CLionProjects/shortest-path/readme-pictures/move-files.png)

### Usage Options
- `--help`: Displays a help message outlining usage options.
- `--default`: Runs a series of predefined experiments comparing Dijkstra's and Bellman-Ford algorithms.
- `--custom -v [num] -d [num]`: Runs a custom experiment with a graph generated based on user-defined parameters (number of vertices (-v) and outgoing degree (-d)). It is required that vertices > 50 to ensure valid assignment and outgoing degree to be between 1 and 10 inclusive (not to make the graph too dense).

**Example:**
![](/Users/m.urban/CLionProjects/shortest-path/readme-pictures/edit-debug.png)

![](/Users/m.urban/CLionProjects/shortest-path/readme-pictures/edit-arguments.png)

## 2. Project Motive

I chose to compare Dijkstra's and Bellman-Ford algorithms as they are both foundational in understanding graph theory and pathfinding algorithms. The challenge was to implement them efficiently on positively weighted directed graphs and to analyze their performance and complexities.

## 3. Project Structure

- `vertex.h` & `vertex.cpp`: Defines the Vertex class, representing graph nodes.
- `graph.h` & `graph.cpp`: Implements the Graph class, encapsulating all graph-related functionalities, including the algorithms.
- `main.cpp`: Handles user input, executes experiments, and drives the overall program flow.
- `CMakeLists.txt`: Configures build settings for the project.
- `testing-data`: Contains files used for testing and running default experiments.
## 4. Project Features

- **Dynamic Graph Generation**: Ability to create random graphs with user-defined parameters, such as the number of vertices and degree of outgoing edges.
- **Graph Construction and Export**: Features for building graphs from text files and exporting them for diverse testing and analysis.
- **Algorithm Comparative Analysis**: Compares the efficiency of Dijkstra's and Bellman-Ford's algorithms in various graph scenarios.
- **Multithreaded Algorithm Execution**: Enhances performance through parallel execution of algorithms using multithreading.
- **User-Friendly Interface**: Offers interactive command-line options for custom experiments and provides helpful guidance.
- **Detailed Path Reporting**: Provides thorough information on the shortest paths, including vertex sequences and path lengths.
- **Modular Design**: The project's architecture allows for easy scalability and future feature additions.
- **Comprehensive Graph Tools**: A complete suite for generating graphs, finding shortest paths, and exporting data, making it a versatile tool for graph analysis.
## 5. Implementation Notes

- **Multithreading**: Implemented using `std::thread` for parallel execution of Dijkstra's and Bellman-Ford's algorithms, enhancing CPU resource utilization.
- **Concurrency Primitives**: Utilized `std::promise` and `std::future` for thread-safe synchronization of algorithm results.
- **High-Resolution Clock**: Used `std::chrono::high_resolution_clock` for microsecond precision in measuring algorithm performance.
- **Priority Queue with Custom Comparator**: Demonstrated advanced C++ features such as lambda expressions in Dijkstra's algorithm using a custom comparator in the priority queue.
- **Exception Handling**: Managed runtime errors like file reading failures to ensure robust code.
- **Dynamic Data Structures**: Employed `std::vector` and `std::list` for flexible and efficient graph and path representation.
- **File I/O**: Enabled reading from and writing to files, showcasing file handling in C++.
- **Random Number Generation and Shuffling**: Leveraged C++'s `<random>` library, including `std::mt19937` and `std::shuffle`, to generate diverse and unpredictable graphs with random edges and weights.

## 6. Algorithmic Analysis

The theoretical complexity of the implemented **Dijkstra**'s algorithm, utilizing a priority queue, is **O(V + E log V)**, where V represents vertices and E edges. **Bellman-Ford**'s algorithm stands at **O(V * E)**. Given these complexities, I expected Dijkstra's algorithm to be (V * E) / (V + E log V) times faster than Bellman-Ford. This led me to assign only two outgoing edges per vertex, as denser graphs would overwhelmingly favor Dijkstra's algorithm, preventing meaningful comparison.

Measurements were conducted against commit xT90O959 on an **Apple M1 Pro processor with 8 performance cores clocked at 3.2 GHz and 2 efficiency cores at 2 GHz**. Each algorithm ran on its separate thread, although the project did not utilize multithreaded algorithms.

![](/Users/m.urban/CLionProjects/shortest-path/readme-pictures/running-default.png)
```
Running the default set of experiments:
.......................................

Running experiment from file: 1k.txt
=================================
[Thread 0x16baa3000] Shortest-path for graph (v=1000, e=2000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16baa3000] Finding path using Dijkstra ...

[Thread 0x16bb2f000] Shortest-path for graph (v=1000, e=2000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16bb2f000] Finding path using Bellman-Ford ...

[Thread 0x16baa3000] Shortest path: 0 -> 648 -> 587 -> 147 -> 485 -> 107 -> 632 -> 846 -> 1  
[Thread 0x16baa3000] Path length: 30
[Thread 0x16baa3000] Dijkstra found the path in 261 microseconds

[Thread 0x16bb2f000] Shortest path: 0 -> 648 -> 587 -> 147 -> 485 -> 107 -> 632 -> 846 -> 1  
[Thread 0x16bb2f000] Path length: 30
[Thread 0x16bb2f000] Bellman-Ford found the path in 35888 microseconds

Running experiment from file: 2k.txt
=================================
[Thread 0x16baa3000] Shortest-path for graph (v=2000, e=4000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16baa3000] Finding path using Dijkstra ...

[Thread 0x16bb2f000] Shortest-path for graph (v=2000, e=4000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16bb2f000] Finding path using Bellman-Ford ...

[Thread 0x16baa3000] Shortest path: 0 -> 287 -> 1506 -> 1374 -> 1881 -> 1574 -> 1659 -> 1883 -> 1941 -> 1816 -> 1  
[Thread 0x16baa3000] Path length: 40
[Thread 0x16baa3000] Dijkstra found the path in 449 microseconds

[Thread 0x16bb2f000] Shortest path: 0 -> 287 -> 1506 -> 1374 -> 1881 -> 1574 -> 1659 -> 1883 -> 1941 -> 1816 -> 1  
[Thread 0x16bb2f000] Path length: 40
[Thread 0x16bb2f000] Bellman-Ford found the path in 139759 microseconds

Running experiment from file: 3k.txt
=================================
[Thread 0x16bb2f000] Shortest-path for graph (v=3000, e=6000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16bb2f000] Finding path using Bellman-Ford ...

[Thread 0x16baa3000] Shortest-path for graph (v=3000, e=6000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16baa3000] Finding path using Dijkstra ...

[Thread 0x16baa3000] Shortest path: 0 -> 882 -> 238 -> 1673 -> 564 -> 1275 -> 1718 -> 785 -> 2611 -> 1779 -> 1  
[Thread 0x16baa3000] Path length: 46
[Thread 0x16baa3000] Dijkstra found the path in 1158 microseconds

[Thread 0x16bb2f000] Shortest path: 0 -> 882 -> 238 -> 1673 -> 564 -> 1275 -> 1718 -> 785 -> 2611 -> 1779 -> 1  
[Thread 0x16bb2f000] Path length: 46
[Thread 0x16bb2f000] Bellman-Ford found the path in 315381 microseconds

Running experiment from file: 5k.txt
=================================
[Thread 0x16baa3000] Shortest-path for graph (v=5000, e=10000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16baa3000] Finding path using Dijkstra ...

[Thread 0x16bb2f000] Shortest-path for graph (v=5000, e=10000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16bb2f000] Finding path using Bellman-Ford ...

[Thread 0x16baa3000] Shortest path: 0 -> 1646 -> 1064 -> 3906 -> 2026 -> 1462 -> 4034 -> 4676 -> 1071 -> 4978 -> 589 -> 1641 -> 3354 -> 1715 -> 1208 -> 1  
[Thread 0x16baa3000] Path length: 61
[Thread 0x16baa3000] Dijkstra found the path in 2141 microseconds

[Thread 0x16bb2f000] Shortest path: 0 -> 1646 -> 1064 -> 3906 -> 2026 -> 1462 -> 4034 -> 4676 -> 1071 -> 4978 -> 589 -> 1641 -> 3354 -> 1715 -> 1208 -> 1  
[Thread 0x16bb2f000] Path length: 61
[Thread 0x16bb2f000] Bellman-Ford found the path in 860198 microseconds

Running experiment from file: 8k.txt
=================================
[Thread 0x16baa3000] Shortest-path for graph (v=8000, e=16000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16baa3000] Finding path using Dijkstra ...

[Thread 0x16bb2f000] Shortest-path for graph (v=8000, e=16000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16bb2f000] Finding path using Bellman-Ford ...

[Thread 0x16baa3000] Shortest path: 0 -> 216 -> 2186 -> 7635 -> 6370 -> 5425 -> 170 -> 7270 -> 6441 -> 6765 -> 3321 -> 703 -> 5002 -> 1703 -> 7182 -> 1  
[Thread 0x16baa3000] Path length: 60
[Thread 0x16baa3000] Dijkstra found the path in 3376 microseconds

[Thread 0x16bb2f000] Shortest path: 0 -> 216 -> 2186 -> 7635 -> 6370 -> 5425 -> 170 -> 7270 -> 6441 -> 6765 -> 3321 -> 703 -> 5002 -> 1703 -> 7182 -> 1  
[Thread 0x16bb2f000] Path length: 60
[Thread 0x16bb2f000] Bellman-Ford found the path in 2188548 microseconds

Running experiment from file: 13k.txt
=================================
[Thread 0x16baa3000] Shortest-path for graph (v=13000, e=26000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16baa3000] Finding path using Dijkstra ...

[Thread 0x16bb2f000] Shortest-path for graph (v=13000, e=26000) | Initial vertex: 0 | Destination vertex: 1
[Thread 0x16bb2f000] Finding path using Bellman-Ford ...

[Thread 0x16baa3000] Shortest path: 0 -> 5656 -> 4358 -> 11276 -> 6377 -> 4887 -> 3947 -> 10767 -> 2111 -> 10668 -> 2084 -> 9093 -> 7136 -> 74 -> 10573 -> 1  
[Thread 0x16baa3000] Path length: 59
[Thread 0x16baa3000] Dijkstra found the path in 5034 microseconds

[Thread 0x16bb2f000] Shortest path: 0 -> 5656 -> 4358 -> 11276 -> 6377 -> 4887 -> 3947 -> 10767 -> 2111 -> 10668 -> 2084 -> 9093 -> 7136 -> 74 -> 10573 -> 1  
[Thread 0x16bb2f000] Path length: 59
[Thread 0x16bb2f000] Bellman-Ford found the path in 5784160 microseconds


Experiment Results:
Experiment   | File       | V      | E      | Dijkstra     | Bellman-Ford
-------------|------------|--------|--------|--------------|-------------
#1           | 1k.txt     | 1000   | 2000   | 261          | 35888
#2           | 2k.txt     | 2000   | 4000   | 449          | 139759
#3           | 3k.txt     | 3000   | 6000   | 1158         | 315381
#4           | 5k.txt     | 5000   | 10000  | 2141         | 860198
#5           | 8k.txt     | 8000   | 16000  | 3376         | 2188548
#6           | 13k.txt    | 13000  | 26000  | 5034         | 5784160

Notes:
1. Results for Dijkstra and Bellman-Ford are displayed in microseconds.
2. V := number of vertices, E := number of edges.
3. Graphs are directed and weighted (1-10).
4. All vertices have 2 outgoing edges.
```
The results confirmed theoretical expectations. Dijkstra's algorithm, with its efficient use of a priority queue, was significantly faster than Bellman-Ford's algorithm. The key difference lies in Bellman-Ford's iterative approach, which cycles through all graph edges in each iteration, resulting in high computational demands for larger graphs. On the other hand, Dijkstra's algorithm, by efficiently selecting and updating vertices based on their current shortest distance, substantially reduces the number of operations needed. It's important to note that Bellman-Ford is more suited for handling graphs with negative weights, which Dijkstra's algorithm cannot accommodate.

## 7. Additional Remarks

Note that both the exported version of graph or one to construct the graph from requires a `.txt` file of the following format:

```
<number_of_vertices>
<index_of_vertex_0> <count_of_outgoing_edges> <edge1_direction> <edge1_weight> <edge2_direction> <edge2_weight> ...
<index_of_vertex_1> <count_of_outgoing_edges> <edge1_direction> <edge1_weight> <edge2_direction> <edge2_weight> ...
...
```

So for example:

```
3
0 1 1 4
1 1 2 5
2 1 0 6
```

Will produce visually (though program provides no visuals):

![](/Users/m.urban/CLionProjects/shortest-path/readme-pictures/graph.png)