# Metro Bus Route Finder using BFS and DFS

## Problem Statement

The goal of this project is to build a metro route finder for the Lahore Metrobus system using graph data structures in C++. Each metro station is represented as a node, and each route between two stations is represented as an edge.

The system allows the user to:

- add stations
- connect stations with routes
- load metro data from a text file
- save metro data permanently
- display the metro network
- find the shortest route using BFS
- traverse the graph using DFS

The project is designed as a beginner-friendly DSA application that demonstrates how real-world transport systems can be modeled using graphs.

## Algorithm Explanation

### 1. Graph Representation

The metro system is represented using an adjacency list:

- `unordered_map<string, vector<string>> adjList`

This structure stores each station as a key, and its directly connected stations as the value.

### 2. Breadth First Search (BFS)

BFS is used to find the shortest route between two stations in an unweighted graph.

How it works:

1. Start from the source station.
2. Insert it into a queue.
3. Mark it as visited.
4. Visit all neighboring stations level by level.
5. Store each station's parent to rebuild the shortest path.
6. When the destination is found, reconstruct and print the path using the parent map.

Why BFS is suitable:

- It explores the graph level by level.
- In an unweighted graph, the first time the destination is reached is the shortest path.

### 3. Depth First Search (DFS)

DFS is used to perform a complete traversal of the metro graph.

How it works:

1. Start from a station.
2. Mark it as visited.
3. Move recursively to an unvisited neighbor.
4. Continue until no further unvisited stations remain.
5. If disconnected parts exist, the traversal continues from the next unvisited station.

Why DFS is useful:

- It helps explore the graph deeply.
- It is useful for understanding connectivity and graph structure.

### 4. File Handling

The project uses `stations.txt` for persistent storage.

- `ifstream` is used to load routes automatically when the program starts.
- `ofstream` is used to save updated routes back into the file.
- `stringstream` is used to parse comma-separated route data.

This allows the metro dataset to remain available even after the program closes.

### 5. Route Validation and Safety

The project includes safety checks to make the application more reliable:

- invalid station names are rejected
- duplicate stations are prevented
- duplicate routes are prevented
- empty input is rejected
- file loading rebuilds the graph cleanly on restart

### 6. UI Structure

The program uses a menu-driven console interface with clear messages and formatted output. It also displays a simple ASCII-style metro view for easier understanding during viva and demonstration.

## Data Structures Used

- `unordered_map<string, vector<string>>` for adjacency list storage
- `queue<string>` for BFS
- `unordered_map<string, bool>` for visited tracking
- `unordered_map<string, string>` for BFS parent reconstruction
- recursion for DFS
- `set<string>` for unique file saving
- `stringstream` for parsing text file input

## Complexity Table

| Operation | Time Complexity | Space Complexity |
|----------|-----------------|------------------|
| Add Station | O(1) average | O(1) |
| Add Route | O(1) average | O(1) |
| Load Routes from File | O(V + E) | O(V + E) |
| Save Routes to File | O(V + E) | O(V + E) |
| BFS Shortest Path | O(V + E) | O(V) |
| DFS Traversal | O(V + E) | O(V) |
| Display Metro Map | O(V + E) | O(V) |

Where:

- `V` = number of stations
- `E` = number of routes

## Features Implemented

- adjacency list graph representation
- bidirectional metro routes
- Lahore Metrobus dataset integration
- automatic loading from `stations.txt`
- saving updated routes to file
- BFS shortest path finding
- DFS full traversal
- invalid input handling
- duplicate route prevention
- console route and map visualization
- execution time measurement for BFS and DFS
- optional weighted-route support for future expansion

## Conclusion

This project successfully models the Lahore Metrobus system as a graph and demonstrates two important graph algorithms: BFS and DFS. It also includes file persistence, input validation, and a clean console interface, making it suitable for DSA submission and viva demonstration.
