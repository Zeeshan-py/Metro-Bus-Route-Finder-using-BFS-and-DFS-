#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::unordered_map;
using std::pair;
using std::vector;

class Graph {
private:
    // Stores each station with its directly connected stations and route weights.
    unordered_map<string, vector<pair<string, int>>> weightedAdjList;

    // Recursive helper used to perform DFS from a single station.
    void dfsHelper(const string& stationName,
                   unordered_map<string, bool>& visited,
                   vector<string>& traversalOrder) const;

public:
    // Removes all stations and routes from the graph.
    void clearGraph();

    // Adds a station to the metro map if it is not already present.
    bool addStation(const string& stationName);

    // Checks whether a station exists in the metro map.
    bool hasStation(const string& stationName) const;

    // Adds a bidirectional weighted route.
    // If allowCreateStations is true, missing stations are created during file loading.
    bool addWeightedRoute(const string& fromStation,
                          const string& toStation,
                          int weight,
                          bool showMessage = true,
                          bool allowCreateStations = false);

    // Loads routes from a text file where each route line contains two stations and a weight.
    bool loadRoutesFromFile(const string& fileName);

    // Saves the current metro graph to a text file using comma-separated routes.
    bool saveRoutesToFile(const string& fileName) const;

    // Finds the minimum-distance route between two stations using Dijkstra's Algorithm.
    pair<vector<string>, int> findShortestWeightedRoute(const string& startStation,
                                                        const string& destinationStation) const;

    // Returns a full DFS traversal of the metro graph, including disconnected parts.
    vector<string> depthFirstTraversal() const;

    // Displays the complete adjacency list representation of the metro map.
    void displayMap() const;

    // Displays a simple ASCII-style visualization of the metro graph.
    void displayAsciiMap() const;
};

#endif
