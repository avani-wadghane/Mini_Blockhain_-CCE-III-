# Disaster Response Network

## Problem Statement

During a disaster, rescue teams need to know whether locations are reachable
and which road route has the shortest distance to a relief center. Model the
road map as an undirected, weighted graph:

- Each location is a vertex.
- Each two-way road is an undirected edge.
- The road distance in kilometers is the edge weight.

The program must support:

1. Adding locations and positive-weight two-way roads.
2. Marking existing locations as relief centers.
3. Finding all locations reachable from a selected starting location and
   determining whether the network is connected from that location.
4. Finding the shortest-distance route from a selected location to a selected
   relief center, or reporting when no route exists.

Road weights must be nonnegative, which is also required by Dijkstra's
algorithm.

## Graph Representation

This C++17 command-line program uses an adjacency list: every location stores
its neighboring locations and the corresponding road distances. The program
stores locations in a `map` and relief centers in a `set`. A sample network
with two relief centers is loaded on startup; it can be extended through the
menu.

## Features

- Add locations and two-way roads with positive distance weights.
- Mark any existing location as a relief center.
- Run depth-first search (DFS) from a location to list its reachable
  locations and determine whether every location is in that connected
  component.
- Use Dijkstra's algorithm to find the shortest-distance route from a
  selected location to a selected relief center. If none exists, the program
  reports that no route is available.

## Build and run

From this directory, compile with a C++17 compiler:

```text
g++ -std=c++17 -Wall -Wextra -pedantic disaster_response_network.cpp -o disaster_response_network
```

On Windows with MinGW, run `disaster_response_network.exe`; on Linux or
macOS, run `./disaster_response_network`.

## Algorithms

### 1. Depth-First Search (DFS)

#### Algorithm

1. Create an empty visited set and a stack containing the starting location.
2. Remove one location from the stack.
3. If the location is already visited, ignore it and continue.
4. Otherwise, mark it visited and add it to the result.
5. Push every unvisited neighbor onto the stack.
6. Repeat until the stack is empty.
7. Compare the number of reached locations with the total number of locations.
   If they are equal, the selected location can reach the entire network;
   otherwise, the network is disconnected with respect to that location.

#### Explanation

DFS explores one road branch completely before returning to another branch.
Because the visited set prevents a location from being processed more than
once, every location and road in the reachable component is examined only a
bounded number of times.

### 2. Dijkstra's Shortest-Path Algorithm

#### Algorithm

1. Initialize every location's distance to infinity and the start location's
distance to zero.
2. Store the locations in a min-priority queue ordered by their current
   distance from the start.
3. Remove the location having the smallest distance.
4. Skip the entry if its distance is stale and different from the stored
   distance.
5. For each neighbor, calculate
   `candidateDistance = currentDistance + roadDistance`.
6. If this candidate is smaller than the neighbor's stored distance, update
   the stored distance, save the current location as the neighbor's previous
   location, and add the neighbor to the queue.
7. Stop when the selected relief center is removed from the queue.
8. If the relief center's distance is still infinity, no route exists.
   Otherwise, follow the previous-location links backward from the relief
   center and reverse them to obtain the route.

#### Explanation

Dijkstra's algorithm greedily expands the closest unvisited location. Each
successful relaxation improves the best-known distance to a location. A
min-priority queue ensures that locations are considered in increasing
distance order. Once the selected relief center is finalized, every road path
to it has been considered and the recorded predecessor chain gives the
minimum-distance route. This works only because all road weights are positive,
and therefore nonnegative.


## Worked Example

The sample network contains the relief centers `North Shelter` and
`West Clinic`. From `Central Hospital` to `West Clinic`, one possible route is
`Central Hospital -> East Bridge -> Riverside -> South Depot -> West Clinic`,
whose distance is `4.0 + 2.0 + 4.5 + 7.0 = 17.5 km`. Dijkstra's algorithm
evaluates this route and alternatives, then prints the minimum-distance route
and its total distance. If a starting location has no path to the selected
center, the program reports that no road route connects them.
