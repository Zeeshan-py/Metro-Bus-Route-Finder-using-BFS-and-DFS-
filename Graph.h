#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Graph {
private:
    // Stores each station and the stations directly connected to it.
    std::unordered_map<std::string, std::vector<std::string>> adjList;

    // Optional weighted adjacency list for future pathfinding upgrades.
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> weightedAdjList;

    // Recursive helper used to perform DFS from a single station.
    void dfsHelper(const std::string& stationName,
                   std::unordered_map<std::string, bool>& visited,
                   std::vector<std::string>& traversalOrder) const;

public:
    // Removes all stations and routes from the graph.
    void clearGraph();

    // Adds a station to the metro map if it is not already present.
    bool addStation(const std::string& stationName);

    // Checks whether a station exists in the metro map.
    bool hasStation(const std::string& stationName) const;

    // Adds a bidirectional route between two stations.
    // If allowCreateStations is true, missing stations are created during file loading.
    bool addRoute(const std::string& fromStation,
                  const std::string& toStation,
                  bool showMessage = true,
                  bool allowCreateStations = false);

    // Adds a bidirectional weighted route for future scalability.
    bool addWeightedRoute(const std::string& fromStation,
                          const std::string& toStation,
                          int weight,
                          bool showMessage = true,
                          bool allowCreateStations = false);

    // Loads routes from a text file where each line contains two station names.
    bool loadRoutesFromFile(const std::string& fileName);

    // Saves the current metro graph to a text file using comma-separated routes.
    bool saveRoutesToFile(const std::string& fileName) const;

    // Finds the shortest route between two stations using BFS.
    std::vector<std::string> findShortestRoute(const std::string& startStation,
                                               const std::string& destinationStation) const;

    // Returns a full DFS traversal of the metro graph, including disconnected parts.
    std::vector<std::string> depthFirstTraversal() const;

    // Displays the complete adjacency list representation of the metro map.
    void displayMap() const;

    // Displays a simple ASCII-style visualization of the metro graph.
    void displayAsciiMap() const;
};

#endif
