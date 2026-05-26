#include "Graph.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <set>

namespace {
using namespace std;

bool containsNeighbor(const vector<string>& neighbors, const string& stationName) {
    return find(neighbors.begin(), neighbors.end(), stationName) != neighbors.end();
}

bool containsWeightedNeighbor(const vector<pair<string, int>>& neighbors, const string& stationName, int& weight) {
    for (const auto& neighbor : neighbors) {
        if (neighbor.first == stationName) {
            weight = neighbor.second;
            return true;
        }
    }

    return false;
}

template <typename T>
void sortStations(vector<T>& items) {
    sort(items.begin(), items.end());
}

string trim(const string& text) {
    const string whitespace = " \t\r\n";
    const size_t first = text.find_first_not_of(whitespace);

    if (first == string::npos) {
        return "";
    }

    const size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

bool parseWeight(const string& text, int& weight) {
    if (text.empty()) {
        return false;
    }

    try {
        size_t consumed = 0;
        const int parsed = stoi(text, &consumed);
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

bool Graph::addStation(const string& stationName) {
    if (stationName.empty()) {
        cout << "Station name cannot be empty.\n";
        return false;
    }

    if (hasStation(stationName)) {
        cout << "Station already exists.\n";
        return false;
    }

    adjList[stationName];
    weightedAdjList[stationName];
    cout << "Station added successfully.\n";
    return true;
}

bool Graph::hasStation(const string& stationName) const {
    return adjList.find(stationName) != adjList.end();
}

bool Graph::addRoute(const string& fromStation, const string& toStation, bool showMessage, bool allowCreateStations) {
    return addWeightedRoute(fromStation, toStation, 1, showMessage, allowCreateStations);
}

bool Graph::addWeightedRoute(const string& fromStation, const string& toStation, int weight, bool showMessage, bool allowCreateStations) {
    if (fromStation.empty() || toStation.empty()) {
        cout << "Station names cannot be empty.\n";
        return false;
    }

    if (fromStation == toStation) {
        cout << "A station cannot be connected to itself.\n";
        return false;
    }

    if (weight <= 0) {
        cout << "Weight must be a positive number.\n";
        return false;
    }

    if (allowCreateStations) {
        // File loading can rebuild the graph from raw routes, so missing stations are created here.
        adjList[fromStation];
        adjList[toStation];
        weightedAdjList[fromStation];
        weightedAdjList[toStation];
    } else if (!hasStation(fromStation) || !hasStation(toStation)) {
        cout << "Invalid station name. Please add both stations before connecting them.\n";
        return false;
    }

    bool routeExists = containsNeighbor(adjList[fromStation], toStation);
    int existingWeight = 0;
    bool weightedRouteExists = containsWeightedNeighbor(weightedAdjList[fromStation], toStation, existingWeight);

    // Add the destination station to the source station's neighbor list.
    if (!containsNeighbor(adjList[fromStation], toStation)) {
        adjList[fromStation].push_back(toStation);
    }

    // Add the source station to the destination station's neighbor list.
    if (!containsNeighbor(adjList[toStation], fromStation)) {
        adjList[toStation].push_back(fromStation);
    }

    // Keep the weighted graph in sync for future shortest-path upgrades.
    if (weightedRouteExists) {
        for (auto& neighbor : weightedAdjList[fromStation]) {
            if (neighbor.first == toStation) {
                neighbor.second = weight;
                break;
            }
        }

        for (auto& neighbor : weightedAdjList[toStation]) {
            if (neighbor.first == fromStation) {
                neighbor.second = weight;
                break;
            }
        }
    } else {
        weightedAdjList[fromStation].push_back({toStation, weight});
        weightedAdjList[toStation].push_back({fromStation, weight});
    }

    if (showMessage) {
        if (routeExists && weightedRouteExists && existingWeight == weight) {
            cout << "Route already exists.\n";
        } else if (routeExists) {
            cout << "Route updated successfully.\n";
        } else {
            cout << "Route added successfully.\n";
        }
    }

    return true;
}

bool Graph::loadRoutesFromFile(const string& fileName) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Could not open file: " << fileName << '\n';
        return false;
    }

    clearGraph();

    string line;
    int loadedRoutes = 0;
    int loadedStations = 0;
    int skippedLines = 0;

    while (getline(inputFile, line)) {
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        stringstream lineStream(line);
        string from;
        string to;

        if (!getline(lineStream, from, ',')) {
            continue;
        }

        if (!getline(lineStream, to, ',')) {
            from = trim(from);
            if (!from.empty()) {
                adjList[from];
                weightedAdjList[from];
                loadedStations++;
            }
            continue;
        }

        from = trim(from);
        to = trim(to);

        string weightText;
        int weight = 1;

        if (getline(lineStream, weightText, ',')) {
            weightText = trim(weightText);
            if (!parseWeight(weightText, weight)) {
                cout << "Skipped invalid weight line: " << line << '\n';
                skippedLines++;
                continue;
            }

            string extraText;
            if (getline(lineStream, extraText, ',') && !trim(extraText).empty()) {
                cout << "Skipped invalid extra-field line: " << line << '\n';
                skippedLines++;
                continue;
            }
        }

        if (from.empty() || to.empty()) {
            skippedLines++;
            continue;
        }

        if (addWeightedRoute(from, to, weight, false, true)) {
            loadedRoutes++;
        } else {
            skippedLines++;
        }
    }

    cout << "Loaded " << loadedRoutes << " route connections and " << loadedStations << " standalone stations from file.\n";
    if (skippedLines > 0) {
        cout << "Skipped " << skippedLines << " invalid file line(s).\n";
    }
    return true;
}

bool Graph::saveRoutesToFile(const string& fileName) const {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cout << "Could not save data to file: " << fileName << '\n';
        return false;
    }

    set<string> uniqueLines;

    for (const auto& stationEntry : adjList) {
        const string& stationName = stationEntry.first;

        if (stationEntry.second.empty()) {
            uniqueLines.insert(stationName);
            continue;
        }

        for (const string& neighbor : stationEntry.second) {
            if (stationName < neighbor) {
                int weight = 1;
                auto weightedStationIterator = weightedAdjList.find(stationName);
                if (weightedStationIterator != weightedAdjList.end()) {
                    containsWeightedNeighbor(weightedStationIterator->second, neighbor, weight);
                }

                if (weight == 1) {
                    uniqueLines.insert(stationName + "," + neighbor);
                } else {
                    uniqueLines.insert(stationName + "," + neighbor + "," + to_string(weight));
                }
            }
        }
    }

    for (const string& line : uniqueLines) {
        outputFile << line << '\n';
    }

    cout << "Metro data saved to " << fileName << " successfully.\n";
    return true;
}

// BFS time complexity: O(V + E), because each station and route can be processed once.
// BFS space complexity: O(V), because the queue, visited map, and parent map can grow with stations.
std::vector<string> Graph::findShortestRoute(const string& startStation, const string& destinationStation) const {
    vector<string> emptyRoute;

    if (!hasStation(startStation) || !hasStation(destinationStation)) {
        return emptyRoute;
    }

    queue<string> stationsToVisit;
    unordered_map<string, bool> visited;
    unordered_map<string, string> parent;

    stationsToVisit.push(startStation);
    visited[startStation] = true;

    while (!stationsToVisit.empty()) {
        string currentStation = stationsToVisit.front();
        stationsToVisit.pop();

        if (currentStation == destinationStation) {
            break;
        }

        auto stationIterator = adjList.find(currentStation);
        if (stationIterator == adjList.end()) {
            continue;
        }

        for (const string& neighbor : stationIterator->second) {
            if (visited.find(neighbor) == visited.end()) {
                visited[neighbor] = true;
                parent[neighbor] = currentStation;
                stationsToVisit.push(neighbor);
            }
        }
    }

    if (visited.find(destinationStation) == visited.end()) {
        return emptyRoute;
    }

    vector<string> route;
    string current = destinationStation;

    while (true) {
        route.push_back(current);

        if (current == startStation) {
            break;
        }

        current = parent[current];
    }

    reverse(route.begin(), route.end());
    return route;
}

void Graph::dfsHelper(const string& stationName, unordered_map<string, bool>& visited, vector<string>& traversalOrder) const {
    visited[stationName] = true;
    traversalOrder.push_back(stationName);

    auto stationIterator = adjList.find(stationName);
    if (stationIterator == adjList.end()) {
        return;
    }

    for (const string& neighbor : stationIterator->second) {
        if (visited.find(neighbor) == visited.end()) {
            dfsHelper(neighbor, visited, traversalOrder);
        }
    }
}

// DFS time complexity: O(V + E), because each station and route is visited once in the recursive walk.
// DFS space complexity: O(V), because the visited map and recursion stack can grow with the number of stations.
std::vector<string> Graph::depthFirstTraversal() const {
    vector<string> traversal;

    if (adjList.empty()) {
        return traversal;
    }

    unordered_map<string, bool> visited;

    for (const auto& station : adjList) {
        if (visited.find(station.first) == visited.end()) {
            dfsHelper(station.first, visited, traversal);
        }
    }

    return traversal;
}

void Graph::displayMap() const {
    if (adjList.empty()) {
        cout << "Metro map is empty. Add stations first.\n";
        return;
    }

    cout << "\nMetro Map (Adjacency List)\n";
    cout << "--------------------------\n";

    vector<string> stations;
    stations.reserve(adjList.size());

    for (const auto& station : adjList) {
        stations.push_back(station.first);
    }

    sortStations(stations);

    for (const string& stationName : stations) {
        cout << stationName << " -> ";

        const auto& neighbors = adjList.at(stationName);

        for (const string& neighbor : neighbors) {
            cout << neighbor << " ";
        }

        cout << '\n';
    }

    cout << "\nASCII View\n";
    cout << "----------\n";

    for (const string& stationName : stations) {
        cout << "[" << stationName << "]\n";

        const auto& neighbors = adjList.at(stationName);

        if (neighbors.empty()) {
            cout << "   |-- (no direct routes)\n";
            continue;
        }

        for (const string& neighbor : neighbors) {
            cout << "   |-- " << neighbor << '\n';
        }
    }
}

void Graph::displayAsciiMap() const {
    displayMap();
}
