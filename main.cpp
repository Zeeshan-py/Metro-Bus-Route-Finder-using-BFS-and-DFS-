#include <iostream>
#include <ctime>
#include <limits>
#include <string>
#include <vector>

#include "Graph.h"

namespace {
const std::string kDataFile = "stations.txt";
}

void printRoute(const std::vector<std::string>& route) {
    if (route.empty()) {
        std::cout << "No route found between the selected stations.\n";
        return;
    }

    std::cout << "Shortest route (" << route.size() << " stations): ";

    for (std::size_t i = 0; i < route.size(); ++i) {
        std::cout << route[i];

        if (i + 1 < route.size()) {
            std::cout << " -> ";
        }
    }

    std::cout << '\n';
}

void printBanner() {
    std::cout << "\n========================================\n";
    std::cout << "   METRO BUS ROUTE FINDER - LAHORE\n";
    std::cout << "========================================\n";
}

void printElapsedTime(const std::string& label, clock_t startTime, clock_t endTime) {
    const double elapsedMilliseconds = 1000.0 * static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
    std::cout << label << " execution time: " << elapsedMilliseconds << " ms\n";
}

void printTraversal(const std::vector<std::string>& traversal) {
    if (traversal.empty()) {
        std::cout << "Metro map is empty. Add stations first.\n";
        return;
    }

    std::cout << "DFS traversal: ";

    for (std::size_t i = 0; i < traversal.size(); ++i) {
        std::cout << traversal[i];

        if (i + 1 < traversal.size()) {
            std::cout << " -> ";
        }
    }

    std::cout << '\n';
}

bool promptForStation(const std::string& prompt, std::string& station) {
    std::cout << prompt;
    std::getline(std::cin, station);

    if (station.empty()) {
        std::cout << "Station name cannot be empty.\n";
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
        std::cout << "4. Find Shortest Route (BFS)\n";
        std::cout << "5. Show DFS Traversal\n";
        std::cout << "6. Save Data\n";
        std::cout << "7. Reload Data\n";
        std::cout << "8. Exit\n";
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
                metroGraph.addStation(station);
                metroGraph.saveRoutesToFile(kDataFile);
                std::cout << "Metro station saved.\n";
            }
            break;
        }
        case 2: {
            std::string from;
            std::string to;

            if (!promptForStation("Enter starting station: ", from)) {
                break;
            }

            if (!promptForStation("Enter destination station: ", to)) {
                break;
            }

            metroGraph.addRoute(from, to);
            metroGraph.saveRoutesToFile(kDataFile);
            std::cout << "Metro route saved.\n";
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
            printRoute(metroGraph.findShortestRoute(start, destination));
            const clock_t endTime = clock();
            printElapsedTime("BFS", startTime, endTime);
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
            metroGraph.saveRoutesToFile(kDataFile);
            break;
        case 7:
            metroGraph.loadRoutesFromFile(kDataFile);
            std::cout << "Metro data reloaded from file.\n";
            break;
        case 8:
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }

    } while (choice != 8);

    return 0;
}