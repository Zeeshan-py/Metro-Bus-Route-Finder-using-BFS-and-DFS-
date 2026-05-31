#include <iostream>
#include <ctime>
#include <limits>
#include <string>
#include <vector>

#include "Graph.h"

namespace {
using namespace std;

const string kDataFile = "stations.txt";

string trim(const string& text) {
    const string whitespace = " \t\r\n";
    const size_t first = text.find_first_not_of(whitespace);

    if (first == string::npos) {
        return "";
    }

    const size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}
}

void printWeightedRoute(const pair<vector<string>, int>& result) {
    const vector<string>& route = result.first;
    const int totalMeters = result.second;

    if (route.empty()) {
        cout << "No route found between the selected stations.\n";
        return;
    }

    cout << "Shortest weighted route (" << route.size() << " stations, "
         << totalMeters << " meters): ";

    for (size_t i = 0; i < route.size(); ++i) {
        cout << route[i];

        if (i + 1 < route.size()) {
            cout << " -> ";
        }
    }

    cout << '\n';
}

void printBanner() {
    cout << "\n========================================\n";
    cout << "   METRO BUS ROUTE FINDER - LAHORE\n";
    cout << "========================================\n";
}

void printElapsedTime(const std::string& label, clock_t startTime, clock_t endTime) {
    const double elapsedMilliseconds = 1000.0 * static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
    cout << label << " execution time: " << elapsedMilliseconds << " ms\n";
}

void printTraversal(const std::vector<std::string>& traversal) {
    if (traversal.empty()) {
        cout << "Metro map is empty. Add stations first.\n";
        return;
    }

    cout << "DFS traversal: ";

    for (size_t i = 0; i < traversal.size(); ++i) {
        cout << traversal[i];

        if (i + 1 < traversal.size()) {
            cout << " -> ";
        }
    }

    cout << '\n';
}

bool promptForStation(const std::string& prompt, std::string& station) {
    cout << prompt;
    std::getline(cin, station);
    station = trim(station);

    if (station.empty()) {
        cout << "Station name cannot be empty.\n";
        return false;
    }

    return true;
}

bool promptForWeight(int& weight) {
    cout << "Enter route weight in meters: ";
    cin >> weight;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid weight. Please enter a positive distance in meters.\n";
        return false;
    }

    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (weight <= 0) {
        cout << "Distance must be greater than 0 meters.\n";
        return false;
    }

    return true;
}

int main() {
    Graph metroGraph;
    int choice = 0;

    metroGraph.loadRoutesFromFile(kDataFile);

    do {
        printBanner();
        std::cout << "1. Add Station\n";
        std::cout << "2. Connect Stations\n";
        std::cout << "3. Display Metro Map\n";
        std::cout << "4. Find Shortest Weighted Route\n";
        std::cout << "5. Show DFS Traversal\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        // Clear bad input so the menu does not get stuck.
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            std::string station;
            if (promptForStation("Enter station name: ", station)) {
                if (metroGraph.addStation(station) && metroGraph.saveRoutesToFile(kDataFile)) {
                    std::cout << "Metro station saved.\n";
                }
            }
            break;
        }
        case 2: {
            std::string from;
            std::string to;
            int weight = 0;

            if (!promptForStation("Enter starting station: ", from)) {
                break;
            }

            if (!promptForStation("Enter destination station: ", to)) {
                break;
            }

            if (from == to) {
                std::cout << "A station cannot be connected to itself.\n";
                break;
            }

            if (!metroGraph.hasStation(from) || !metroGraph.hasStation(to)) {
                std::cout << "Invalid station name. Please add both stations before connecting them.\n";
                break;
            }

            if (!promptForWeight(weight)) {
                break;
            }

            if (metroGraph.addWeightedRoute(from, to, weight) && metroGraph.saveRoutesToFile(kDataFile)) {
                std::cout << "Metro route saved.\n";
            }
            break;
        }
        case 3:
            metroGraph.displayMap();
            break;
        case 4:
        {
            std::string start;
            std::string destination;

            if (!promptForStation("Enter start station: ", start)) {
                break;
            }

            if (!promptForStation("Enter destination station: ", destination)) {
                break;
            }

            if (!metroGraph.hasStation(start) || !metroGraph.hasStation(destination)) {
                std::cout << "Invalid station name. Please check the metro map and try again.\n";
                break;
            }

            const clock_t startTime = clock();
            printWeightedRoute(metroGraph.findShortestWeightedRoute(start, destination));
            const clock_t endTime = clock();
            printElapsedTime("Dijkstra", startTime, endTime);
            break;
        }
        case 5: {
            const clock_t startTime = clock();
            printTraversal(metroGraph.depthFirstTraversal());
            const clock_t endTime = clock();
            printElapsedTime("DFS", startTime, endTime);
            break;
        }
        case 6:
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }

    } while (choice != 6);

    return 0;
}
