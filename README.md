# Metro Bus Route Finder using BFS and DFS

A beginner-friendly C++ DSA project that models the Lahore Metrobus system as a graph using an adjacency list.

## Features

- Add metro stations
- Connect stations with bidirectional routes
- Automatically load route data from `stations.txt` at startup
- Save graph data back to `stations.txt`
- Find the shortest route using BFS
- Display a full DFS traversal
- Handle invalid station input safely
- Prevent duplicate routes
- Show a simple ASCII-style metro visualization
- Measure BFS and DFS execution time with `clock()`
- Support optional weighted route data for future upgrades

## Data Structures Used

- `unordered_map<string, vector<string>>` for the adjacency list
- `queue` for BFS
- `unordered_map` for visited tracking and parent reconstruction
- recursion for DFS
- `stringstream` for parsing comma-separated file data
- `clock()` for simple runtime measurement

## File Handling

The project uses a simple text file named `stations.txt`.

Each line stores either:

- a route in the format `Station1,Station2`
- a weighted route in the format `Station1,Station2,Weight`
- or a standalone station name for isolated stations

## Complexity

- BFS time complexity: `O(V + E)`
- BFS space complexity: `O(V)`
- DFS time complexity: `O(V + E)`
- DFS space complexity: `O(V)`
- File save/load time complexity: `O(V + E)` for a full graph pass

## How to Run

1. Compile the project:

```bash
g++ main.cpp Graph.cpp -std=c++17 -o metro.exe
```

2. Run the program:

```bash
metro.exe
```

## Menu Options

1. Add Station
2. Connect Stations
3. Display Metro Map
4. Find Shortest Route (BFS)
5. Show DFS Traversal
6. Save Data
7. Reload Data
8. Exit

## Visual Output

The map output includes both:

- a normal adjacency-list view
- a simple ASCII-style station listing for easier viva explanation

## Sample Dataset

The project is seeded with Lahore Metrobus routes such as:

- Shahdara -> Salamatpura
- Salamatpura -> Niazi Chowk
- Niazi Chowk -> Timber Market
- Timber Market -> Azadi Chowk
- Azadi Chowk -> Railway Station
- Railway Station -> MAO College
- MAO College -> Canal
- Canal -> Ichra
- Ichra -> Shama
- Shama -> Kalma Chowk
- Kalma Chowk -> Qainchi
- Qainchi -> Chungi Amar Sidhu
- Chungi Amar Sidhu -> Attari Saroba
- Attari Saroba -> Gajjumata

Extra alternate connections are included to make BFS and DFS traversal more realistic.

# Metro-Bus-Route-Finder-using-BFS-and-DFS-
