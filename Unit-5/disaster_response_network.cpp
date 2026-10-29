#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class DisasterResponseNetwork {
public:
    struct Road {
        std::string destination;
        double distanceKm;
    };

    struct Route {
        std::vector<std::string> locations;
        double distanceKm;
    };

    bool addLocation(const std::string& name) {
        if (name.empty()) {
            return false;
        }
        return roads_.emplace(name, std::vector<Road>{}).second;
    }

    bool addRoad(const std::string& first, const std::string& second,
                 double distanceKm) {
        if (first == second || !std::isfinite(distanceKm) ||
            distanceKm <= 0.0 ||
            roads_.find(first) == roads_.end() ||
            roads_.find(second) == roads_.end() ||
            hasRoad(first, second)) {
            return false;
        }

        roads_[first].push_back({second, distanceKm});
        roads_[second].push_back({first, distanceKm});
        return true;
    }

    bool addReliefCenter(const std::string& location) {
        if (roads_.find(location) == roads_.end()) {
            return false;
        }
        return reliefCenters_.insert(location).second;
    }

    const std::map<std::string, std::vector<Road>>& locations() const {
        return roads_;
    }

    const std::set<std::string>& reliefCenters() const {
        return reliefCenters_;
    }

    std::vector<std::string> reachableFrom(const std::string& start) const {
        std::vector<std::string> reachable;
        if (roads_.find(start) == roads_.end()) {
            return reachable;
        }

        std::set<std::string> visited;
        std::vector<std::string> pending{start};
        while (!pending.empty()) {
            std::string current = pending.back();
            pending.pop_back();
            if (!visited.insert(current).second) {
                continue;
            }

            reachable.push_back(current);
            for (const Road& road : roads_.at(current)) {
                if (visited.find(road.destination) == visited.end()) {
                    pending.push_back(road.destination);
                }
            }
        }

        std::sort(reachable.begin(), reachable.end());
        return reachable;
    }

    bool shortestRoute(const std::string& start, const std::string& destination,
                       Route& result) const {
        if (roads_.find(start) == roads_.end() ||
            roads_.find(destination) == roads_.end()) {
            return false;
        }

        using QueueEntry = std::pair<double, std::string>;
        std::priority_queue<QueueEntry, std::vector<QueueEntry>,
                            std::greater<QueueEntry>> pending;
        std::map<std::string, double> distances;
        std::map<std::string, std::string> previous;

        for (const auto& location : roads_) {
            distances[location.first] = std::numeric_limits<double>::infinity();
        }
        distances[start] = 0.0;
        pending.push({0.0, start});

        while (!pending.empty()) {
            const auto [distance, current] = pending.top();
            pending.pop();
            if (distance > distances[current]) {
                continue;
            }
            if (current == destination) {
                break;
            }

            for (const Road& road : roads_.at(current)) {
                const double candidate = distance + road.distanceKm;
                if (candidate < distances[road.destination]) {
                    distances[road.destination] = candidate;
                    previous[road.destination] = current;
                    pending.push({candidate, road.destination});
                }
            }
        }

        if (!std::isfinite(distances[destination])) {
            return false;
        }

        result.locations.clear();
        for (std::string current = destination;; current = previous.at(current)) {
            result.locations.push_back(current);
            if (current == start) {
                break;
            }
        }
        std::reverse(result.locations.begin(), result.locations.end());
        result.distanceKm = distances[destination];
        return true;
    }

private:
    bool hasRoad(const std::string& first, const std::string& second) const {
        const auto& adjacent = roads_.at(first);
        return std::any_of(adjacent.begin(), adjacent.end(),
                           [&second](const Road& road) {
                               return road.destination == second;
                           });
    }

    std::map<std::string, std::vector<Road>> roads_;
    std::set<std::string> reliefCenters_;
};

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

bool readPositiveDistance(double& distanceKm) {
    const std::string input = readLine("Road distance in km: ");
    std::istringstream parser(input);
    char extra;
    if (!(parser >> distanceKm) || parser >> extra ||
        !std::isfinite(distanceKm) || distanceKm <= 0.0) {
        std::cout << "Enter a valid distance greater than zero.\n";
        return false;
    }
    return true;
}

bool readIndex(const std::string& prompt, std::size_t count,
               std::size_t& selected) {
    const std::string input = readLine(prompt);
    std::istringstream parser(input);
    std::size_t index;
    char extra;
    if (!(parser >> index) || parser >> extra || index == 0 || index > count) {
        std::cout << "Enter a number from 1 to " << count << ".\n";
        return false;
    }
    selected = index - 1;
    return true;
}

void printLocations(const DisasterResponseNetwork& network) {
    std::cout << "\nLocations and roads:\n";
    for (const auto& location : network.locations()) {
        std::cout << "  " << location.first;
        if (network.reliefCenters().find(location.first) !=
            network.reliefCenters().end()) {
            std::cout << " [relief center]";
        }
        std::cout << '\n';
        for (const auto& road : location.second) {
            std::cout << "    -> " << road.destination << " ("
                      << std::fixed << std::setprecision(1) << road.distanceKm
                      << " km)\n";
        }
    }
}

void printMenu() {
    std::cout << "\n=== Disaster Response Network ===\n"
              << "1. Display locations and roads\n"
              << "2. Add location\n"
              << "3. Add two-way road\n"
              << "4. Mark relief center\n"
              << "5. Analyze connectivity (DFS)\n"
              << "6. Find shortest route to relief center (Dijkstra)\n"
              << "0. Exit\n";
}

int main() {
    DisasterResponseNetwork network;
    for (const char* location : {
             "Central Hospital", "East Bridge", "North Shelter",
             "Riverside", "South Depot", "West Clinic"}) {
        network.addLocation(location);
    }
    network.addRoad("Central Hospital", "East Bridge", 4.0);
    network.addRoad("Central Hospital", "Riverside", 6.5);
    network.addRoad("East Bridge", "North Shelter", 5.0);
    network.addRoad("East Bridge", "Riverside", 2.0);
    network.addRoad("Riverside", "South Depot", 4.5);
    network.addRoad("South Depot", "North Shelter", 3.0);
    network.addRoad("West Clinic", "South Depot", 7.0);
    network.addReliefCenter("North Shelter");
    network.addReliefCenter("West Clinic");

    std::cout << "Disaster Response Network\n"
              << "Road weights represent distance in kilometers. "
                 "A sample network is ready.\n";

    while (true) {
        printMenu();
        const std::string input = readLine("Choose an option: ");
        if (!std::cin) {
            std::cout << '\n';
            break;
        }
        std::istringstream parser(input);
        int choice;
        char extra;
        if (!(parser >> choice) || parser >> extra) {
            std::cout << "Enter a valid menu number.\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting.\n";
            break;
        }
        if (choice == 1) {
            printLocations(network);
        } else if (choice == 2) {
            const std::string name = readLine("Location name: ");
            std::cout << (network.addLocation(name)
                              ? "Location added.\n"
                              : "Could not add location; check the name is "
                                "non-empty and not already present.\n");
        } else if (choice == 3) {
            const std::string first = readLine("First location: ");
            const std::string second = readLine("Second location: ");
            double distanceKm;
            if (readPositiveDistance(distanceKm)) {
                std::cout << (network.addRoad(first, second, distanceKm)
                                  ? "Two-way road added.\n"
                                  : "Could not add road; locations must exist, "
                                    "be distinct, and not already be connected.\n");
            }
        } else if (choice == 4) {
            const std::string location = readLine("Relief center location: ");
            std::cout << (network.addReliefCenter(location)
                              ? "Relief center added.\n"
                              : "Location does not exist or is already a "
                                "relief center.\n");
        } else if (choice == 5) {
            const std::string start = readLine("Start location: ");
            if (network.locations().find(start) == network.locations().end()) {
                std::cout << "Unknown location.\n";
                continue;
            }
            const std::vector<std::string> reachable =
                network.reachableFrom(start);
            std::cout << "DFS from " << start << " can reach "
                      << reachable.size() << " of "
                      << network.locations().size() << " locations:\n";
            for (const std::string& location : reachable) {
                std::cout << "  " << location << '\n';
            }
            if (reachable.size() == network.locations().size()) {
                std::cout << "All locations are connected to the start.\n";
            } else {
                std::cout << "The network has locations outside this "
                             "connected component.\n";
            }
        } else if (choice == 6) {
            if (network.reliefCenters().empty()) {
                std::cout << "No relief centers are configured.\n";
                continue;
            }

            const std::string start = readLine("Start location: ");
            if (network.locations().find(start) == network.locations().end()) {
                std::cout << "Unknown location.\n";
                continue;
            }

            std::vector<std::string> centers(network.reliefCenters().begin(),
                                             network.reliefCenters().end());
            std::cout << "Relief centers:\n";
            for (std::size_t index = 0; index < centers.size(); ++index) {
                std::cout << "  " << index + 1 << ". " << centers[index]
                          << '\n';
            }
            std::size_t selected;
            if (!readIndex("Choose a relief center: ", centers.size(),
                           selected)) {
                continue;
            }

            DisasterResponseNetwork::Route route;
            if (!network.shortestRoute(start, centers[selected], route)) {
                std::cout << "No road route connects " << start << " to "
                          << centers[selected] << ".\n";
                continue;
            }
            std::cout << "Shortest route (" << std::fixed
                      << std::setprecision(1) << route.distanceKm << " km):\n  ";
            for (std::size_t index = 0; index < route.locations.size(); ++index) {
                if (index > 0) {
                    std::cout << " -> ";
                }
                std::cout << route.locations[index];
            }
            std::cout << '\n';
        } else {
            std::cout << "Unknown option.\n";
        }
    }

    return 0;
}
