#include "Graph.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <set>

namespace {
bool containsNeighbor(const std::vector<std::string>& neighbors, const std::string& station) {
    return std::find(neighbors.begin(), neighbors.end(), station) != neighbors.end();
}

bool containsWeightedNeighbor(const std::vector<std::pair<std::string, int>>& neighbors, const std::string& station, int& weight) {
    for (const auto& neighbor : neighbors) {
        if (neighbor.first == station) {
            weight = neighbor.second;
            return true;
        }
    }

    return false;
}

template <typename T>
void sortStations(std::vector<T>& items) {
    std::sort(items.begin(), items.end());
}

std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = text.find_first_not_of(whitespace);

    if (first == std::string::npos) {
        return "";
    }

    const std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

bool parseWeight(const std::string& text, int& weight) {
    if (text.empty()) {
        return false;
    }

    try {
        std::size_t consumed = 0;
        const int parsed = std::stoi(text, &consumed);
        if (consumed != text.size() || parsed <= 0) {
            return false;
        }

        weight = parsed;
        return true;
    } catch (...) {
        return false;
    }
}
}

void Graph::clearGraph() {
    adjList.clear();
    weightedAdjList.clear();
}

void Graph::addStation(const std::string& station) {
    if (station.empty()) {
        std::cout << "Station name cannot be empty.\n";
        return;
    }

    if (hasStation(station)) {
        std::cout << "Station already exists.\n";
        return;
    }

    // Accessing the key creates the station with an empty list if it does not exist.
    adjList[station];
    std::cout << "Station added successfully.\n";
}

bool Graph::hasStation(const std::string& station) const {
    return adjList.find(station) != adjList.end();
}

void Graph::addRoute(const std::string& from, const std::string& to, bool showMessage, bool allowCreateStations) {
    addWeightedRoute(from, to, 1, showMessage, allowCreateStations);
}

void Graph::addWeightedRoute(const std::string& from, const std::string& to, int weight, bool showMessage, bool allowCreateStations) {
    if (from.empty() || to.empty()) {
        std::cout << "Station names cannot be empty.\n";
        return;
    }

    if (weight <= 0) {
        std::cout << "Weight must be a positive number.\n";
        return;
    }

    if (allowCreateStations) {
        // File loading can rebuild the graph from raw routes, so missing stations are created here.
        adjList[from];
        adjList[to];
        weightedAdjList[from];
        weightedAdjList[to];
    } else if (!hasStation(from) || !hasStation(to)) {
        std::cout << "Invalid station name. Please add both stations before connecting them.\n";
        return;
    }

    bool routeExists = containsNeighbor(adjList[from], to);
    int existingWeight = 0;
    bool weightedRouteExists = containsWeightedNeighbor(weightedAdjList[from], to, existingWeight);

    // Add the destination station to the source station's neighbor list.
    if (!containsNeighbor(adjList[from], to)) {
        adjList[from].push_back(to);
    }

    // Add the source station to the destination station's neighbor list.
    if (!containsNeighbor(adjList[to], from)) {
        adjList[to].push_back(from);
    }

    // Keep the weighted graph in sync for future shortest-path upgrades.
    if (weightedRouteExists) {
        for (auto& neighbor : weightedAdjList[from]) {
            if (neighbor.first == to) {
                neighbor.second = weight;
                break;
            }
        }

        for (auto& neighbor : weightedAdjList[to]) {
            if (neighbor.first == from) {
                neighbor.second = weight;
                break;
            }
        }
    } else {
        weightedAdjList[from].push_back({to, weight});
        weightedAdjList[to].push_back({from, weight});
    }

    if (showMessage) {
        if (routeExists && weightedRouteExists && existingWeight == weight) {
            std::cout << "Route already exists.\n";
        } else if (routeExists) {
            std::cout << "Route updated successfully.\n";
        } else {
            std::cout << "Route added successfully.\n";
        }
    }
}

void Graph::loadRoutesFromFile(const std::string& fileName) {
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        std::cout << "Could not open file: " << fileName << '\n';
        return;
    }

    clearGraph();

    std::string line;
    int loadedRoutes = 0;
    int loadedStations = 0;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream lineStream(line);
        std::string from;
        std::string to;

        if (!std::getline(lineStream, from, ',')) {
            continue;
        }

        if (!std::getline(lineStream, to, ',')) {
            from = trim(from);
            if (!from.empty()) {
                addStation(from);
                loadedStations++;
            }
            continue;
        }

        from = trim(from);
        to = trim(to);

        std::string weightText;
        int weight = 1;

        if (std::getline(lineStream, weightText, ',')) {
            weightText = trim(weightText);
            parseWeight(weightText, weight);
        }

        if (from.empty() || to.empty()) {
            continue;
        }

        addWeightedRoute(from, to, weight, false, true);
        loadedRoutes++;
    }

    std::cout << "Loaded " << loadedRoutes << " route connections and " << loadedStations << " standalone stations from file.\n";
}

void Graph::saveRoutesToFile(const std::string& fileName) const {
    std::ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        std::cout << "Could not save data to file: " << fileName << '\n';
        return;
    }

    std::set<std::string> uniqueLines;

    for (const auto& stationEntry : adjList) {
        const std::string& station = stationEntry.first;

        if (stationEntry.second.empty()) {
            uniqueLines.insert(station);
            continue;
        }

        for (const std::string& neighbor : stationEntry.second) {
            if (station < neighbor) {
                int weight = 1;
                auto weightedStationIterator = weightedAdjList.find(station);
                if (weightedStationIterator != weightedAdjList.end()) {
                    containsWeightedNeighbor(weightedStationIterator->second, neighbor, weight);
                }

                if (weight == 1) {
                    uniqueLines.insert(station + "," + neighbor);
                } else {
                    uniqueLines.insert(station + "," + neighbor + "," + std::to_string(weight));
                }
            }
        }
    }

    for (const std::string& line : uniqueLines) {
        outputFile << line << '\n';
    }

    std::cout << "Metro data saved to " << fileName << " successfully.\n";
}

// BFS time complexity: O(V + E), because each station and route can be processed once.
// BFS space complexity: O(V), because the queue, visited map, and parent map can grow with stations.
std::vector<std::string> Graph::findShortestRoute(const std::string& start, const std::string& destination) const {
    std::vector<std::string> emptyRoute;

    if (!hasStation(start) || !hasStation(destination)) {
        return emptyRoute;
    }

    std::queue<std::string> stationsToVisit;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, std::string> parent;

    stationsToVisit.push(start);
    visited[start] = true;

    while (!stationsToVisit.empty()) {
        std::string currentStation = stationsToVisit.front();
        stationsToVisit.pop();

        if (currentStation == destination) {
            break;
        }

        auto stationIterator = adjList.find(currentStation);
        if (stationIterator == adjList.end()) {
            continue;
        }

        for (const std::string& neighbor : stationIterator->second) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = currentStation;
                stationsToVisit.push(neighbor);
            }
        }
    }

    if (!visited[destination]) {
        return emptyRoute;
    }

    std::vector<std::string> route;
    std::string current = destination;

    while (true) {
        route.push_back(current);

        if (current == start) {
            break;
        }

        current = parent[current];
    }

    std::reverse(route.begin(), route.end());
    return route;
}

void Graph::dfsHelper(const std::string& station, std::unordered_map<std::string, bool>& visited, std::vector<std::string>& traversal) const {
    visited[station] = true;
    traversal.push_back(station);

    auto stationIterator = adjList.find(station);
    if (stationIterator == adjList.end()) {
        return;
    }

    for (const std::string& neighbor : stationIterator->second) {
        if (!visited[neighbor]) {
            dfsHelper(neighbor, visited, traversal);
        }
    }
}

// DFS time complexity: O(V + E), because each station and route is visited once in the recursive walk.
// DFS space complexity: O(V), because the visited map and recursion stack can grow with the number of stations.
std::vector<std::string> Graph::depthFirstTraversal() const {
    std::vector<std::string> traversal;

    if (adjList.empty()) {
        return traversal;
    }

    std::unordered_map<std::string, bool> visited;

    for (const auto& station : adjList) {
        if (!visited[station.first]) {
            dfsHelper(station.first, visited, traversal);
        }
    }

    return traversal;
}

void Graph::displayMap() const {
    if (adjList.empty()) {
        std::cout << "Metro map is empty. Add stations first.\n";
        return;
    }

    std::cout << "\nMetro Map (Adjacency List)\n";
    std::cout << "--------------------------\n";

    std::vector<std::string> stations;
    stations.reserve(adjList.size());

    for (const auto& station : adjList) {
        stations.push_back(station.first);
    }

    sortStations(stations);

    for (const std::string& stationName : stations) {
        std::cout << stationName << " -> ";

        const auto& neighbors = adjList.at(stationName);

        for (const std::string& neighbor : neighbors) {
            std::cout << neighbor << " ";
        }

        std::cout << '\n';
    }

    std::cout << "\nASCII View\n";
    std::cout << "----------\n";

    for (const std::string& stationName : stations) {
        std::cout << "[" << stationName << "]\n";

        const auto& neighbors = adjList.at(stationName);

        if (neighbors.empty()) {
            std::cout << "   |-- (no direct routes)\n";
            continue;
        }

        for (const std::string& neighbor : neighbors) {
            std::cout << "   |-- " << neighbor << '\n';
        }
    }
}

void Graph::displayAsciiMap() const {
    displayMap();
}