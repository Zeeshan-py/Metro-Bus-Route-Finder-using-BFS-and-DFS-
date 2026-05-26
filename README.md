# Metro Bus Route Finder using BFS and DFS

A C++ DSA project that models the Lahore Metro Bus System as an undirected weighted graph. Each station is a vertex, and each route between two stations is a bidirectional edge. The project demonstrates graph representation, BFS shortest-path traversal, DFS traversal, file handling, STL containers, recursion, and object-oriented programming.

## Project Objective

The objective of this project is to build a console-based route finder for a metro bus network. The system allows users to add stations, connect stations, save/load route data, display the graph, find the shortest route by hop count using BFS, and perform a full DFS traversal of the metro network.

This project is suitable for a DSA course submission because it connects a real-world transport problem with core graph concepts.

## Core Concepts Used

- C++
- Object-Oriented Programming
- Undirected graph
- Weighted graph storage
- Adjacency list representation
- Breadth First Search
- Depth First Search
- Recursion
- File handling
- STL containers
- Runtime measurement with `clock()`

## File Structure

| File | Purpose |
|---|---|
| `main.cpp` | Menu-driven console interface and user input handling |
| `Graph.h` | Graph class declaration |
| `Graph.cpp` | Graph class implementation, BFS, DFS, file handling, display logic |
| `stations.txt` | Persistent metro station and route data |
| `README.md` | Complete project report and documentation |

The compiled executable is intentionally not included in the repository. Build outputs such as `metro.exe` should be regenerated locally.

## Graph Representation

The project stores the metro network using one weighted adjacency list:

```cpp
std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> weightedAdjList;
```

Each key is a station name. Each value is a list of connected stations with their route weight.

Example:

```text
Shahdara -> Niazi Chowk(1)
Niazi Chowk -> Shahdara(1), Timber Market(1)
```

The graph is undirected. When a route is added from station `A` to station `B`, the program stores both:

```text
A -> B
B -> A
```

## Sample Metro Graph

```mermaid
graph LR
    Shahdara --- Niazi_Chowk
    Niazi_Chowk --- Timber_Market
    Timber_Market --- Azadi_Chowk
    Azadi_Chowk --- Bhatti_Chowk
    Bhatti_Chowk --- Katchery
    Katchery --- Civil_Secretariat
    Civil_Secretariat --- MAO_College
    MAO_College --- Janazgah
    Janazgah --- Qartaba_Chowk
    Qartaba_Chowk --- Shama
    Shama --- Ichhra
    Ichhra --- Canal
    Canal --- Qaddafi_Stadium
    Qaddafi_Stadium --- Kalma_Chowk
    Kalma_Chowk --- Model_Town
    Model_Town --- Naseerabad
    Naseerabad --- Ittefaq_Hospital
    Ittefaq_Hospital --- Qainchi
    Qainchi --- Ghazi_Chowk
    Ghazi_Chowk --- Chungi_Amar_Sidhu
    Chungi_Amar_Sidhu --- Kamahan
    Kamahan --- Attari_Saroba
    Attari_Saroba --- Nishtar
    Nishtar --- Youhanabad
    Youhanabad --- Dulu_Khurd
    Dulu_Khurd --- Gajjumata
```

## Features

- Add metro stations
- Connect stations with bidirectional weighted routes
- Prevent duplicate stations
- Prevent duplicate routes
- Reject self-routes such as `A -> A`
- Store weighted route data
- Load metro data from `stations.txt`
- Save updated graph data to `stations.txt`
- Find shortest route by hop count using BFS
- Perform full DFS traversal, including disconnected components
- Display adjacency-list map
- Display simple ASCII-style map
- Handle missing and invalid station names safely
- Trim user input to avoid accidental duplicate names with spaces
- Validate route weights while loading file data
- Measure BFS and DFS execution time

## Menu Options

```text
1. Add Station
2. Connect Stations
3. Display Metro Map
4. Find Shortest Route (BFS)
5. Show DFS Traversal
6. Save Data
7. Reload Data
8. Exit
```

## File Format

The project uses `stations.txt` as persistent storage.

Supported formats:

```text
Station1,Station2,Weight
Standalone Station
```

Examples:

```text
Shahdara,Niazi Chowk,1
Niazi Chowk,Timber Market,1
Isolated Station
```

Older two-column route lines such as `Station1,Station2` can still be loaded and are treated as weight `1`. New saves use the three-column weighted format. Invalid weights such as `0`, negative values, or non-numeric values are skipped during loading.

## BFS Explanation

BFS is used to find the shortest route between two stations by number of edges, also called hop count.

Steps:

1. Start from the source station.
2. Push the source station into a queue.
3. Mark it as visited.
4. Visit neighboring stations level by level.
5. Store each station's parent.
6. Stop when the destination is found.
7. Reconstruct the path using the parent map.

Important note: BFS reads neighbors from the weighted graph, but it does not compare weight values. It finds the route with the fewest station-to-station hops. For weighted shortest distance or weighted travel time, Dijkstra's Algorithm should be added.

## DFS Explanation

DFS is used to traverse the graph deeply using recursion.

Steps:

1. Start from an unvisited station.
2. Mark it as visited.
3. Recursively visit each unvisited neighbor.
4. Continue until no unvisited neighbor remains.
5. If disconnected components exist, start DFS again from the next unvisited station.

The implementation covers disconnected graphs by looping over every station in the weighted adjacency list.

## Validation and Safety

The project includes the following checks:

- Empty station names are rejected.
- Whitespace around user input is removed.
- Duplicate stations are rejected.
- Duplicate routes are not inserted again.
- Existing weighted routes can be updated.
- Self-routes are rejected.
- Missing stations are rejected during manual route creation.
- Invalid file weights are skipped.
- BFS safely handles missing, isolated, same-source-destination, and unreachable station cases.
- DFS uses visited tracking to avoid infinite recursion in cyclic graphs.

## Complexity Analysis

Let:

- `V` = number of stations
- `E` = number of routes
- `d` = degree of a station

| Operation | Time Complexity | Space Complexity | Notes |
|---|---:|---:|---|
| Add station | O(1) average | O(1) | Uses `unordered_map` |
| Check station | O(1) average | O(1) | Uses `unordered_map::find` |
| Add weighted route | O(d) | O(1) | Vector scan prevents duplicate neighbors |
| BFS shortest route | O(V + E) | O(V) | Finds shortest path by hop count |
| DFS traversal | O(V + E) | O(V) | Includes recursion stack |
| Load file | O(E * d) practical | O(V + E) | Each route checks for duplicates |
| Save file | O(E log E) | O(E) | Uses `set` to keep saved routes unique |
| Display map | O(V log V + E) | O(V) | Stations are sorted before display |

## How to Compile and Run

Compile:

```bash
g++ main.cpp Graph.cpp -std=c++17 -Wall -Wextra -pedantic -o metro.exe
```

Run on Windows:

```bash
metro.exe
```

Run on Linux/macOS:

```bash
./metro
```

## Example BFS Output

```text
Shortest route (27 stations): Shahdara -> Niazi Chowk -> Timber Market -> Azadi Chowk -> Bhatti Chowk -> Katchery -> Civil Secretariat -> MAO College -> Janazgah -> Qartaba Chowk -> Shama -> Ichhra -> Canal -> Qaddafi Stadium -> Kalma Chowk -> Model Town -> Naseerabad -> Ittefaq Hospital -> Qainchi -> Ghazi Chowk -> Chungi Amar Sidhu -> Kamahan -> Attari Saroba -> Nishtar -> Youhanabad -> Dulu Khurd -> Gajjumata
BFS execution time: 1 ms
```

## Example DFS Output

```text
DFS traversal: Shahdara -> Niazi Chowk -> Timber Market -> Azadi Chowk -> Bhatti Chowk -> Katchery -> Civil Secretariat -> MAO College -> Janazgah -> Qartaba Chowk -> Shama -> Ichhra -> Canal -> Qaddafi Stadium -> Kalma Chowk -> Model Town -> Naseerabad -> Ittefaq Hospital -> Qainchi -> Ghazi Chowk -> Chungi Amar Sidhu -> Kamahan -> Attari Saroba -> Nishtar -> Youhanabad -> Dulu Khurd -> Gajjumata
DFS execution time: 0 ms
```

## Strengths

- Clear graph-based modeling of a real transport system.
- Correct undirected route insertion.
- Correct BFS shortest-path logic for minimum hop count.
- Correct DFS traversal with disconnected graph coverage.
- Persistent storage through file handling.
- Duplicate route prevention.
- Single weighted adjacency list used across the graph.
- Modular structure with separate header and implementation files.

## Current Limitations

- BFS does not calculate weighted shortest paths.
- DFS is recursive, so extremely large graphs could risk stack overflow.
- Route insertion uses vectors, so duplicate checks are linear in station degree.
- There is no route deletion feature.
- There is no case-insensitive station matching.
- The console UI is simple and text-based.

## Future Improvements

- Add Dijkstra's Algorithm for shortest route by distance, time, or fare.
- Add Prim's or Kruskal's Algorithm to demonstrate minimum spanning tree concepts.
- Add route deletion and station deletion.
- Add case-insensitive search.
- Add station suggestions for misspelled names.
- Store latitude/longitude data for real map visualization.
- Replace vector neighbor lists with `unordered_set` for faster duplicate checks.
- Add unit tests for BFS, DFS, file parsing, duplicate routes, and invalid input.

## Final Summary

This project successfully demonstrates how a metro bus route network can be represented using graph data structures in C++. It implements station and route management, persistent file storage, BFS shortest-path search, DFS traversal, duplicate prevention, and basic validation. The current implementation is strong for a DSA semester project and can be extended into a more advanced route planner by adding Dijkstra's Algorithm for weighted shortest paths.
