# **MineEscape**

## **Overview**
MineEscape is a C++ project focused on finding the optimal escape route from a mine using weighted pathfinding algorithms. The mine is represented as a grid, with traversal costs determined by obstacles, traps, and power-ups like TNT. TNT adds a unique gameplay element through explosions that destroy surrounding blocks and can trigger chain reactions. The goal is to navigate efficiently from a start point to an end point while avoiding hazards and leveraging these dynamic elements.

The P2random files are given as starter code, but all other files are my own.

## **Skills Demonstrated**
- **Pathfinding Algorithms**: Implemented Dijkstra’s algorithm for shortest pathfinding.
- **Graph Representation**: Modeled the mine as a graph with weighted edges.
- **Interactive Gameplay Design**: Integrated TNT explosions, traps, and dynamic traversal mechanics.
- **Error Handling**: Managed edge cases such as dead ends and invalid input.

## **Key Features**
- **Weighted Graph Traversal**: Accounts for traps, obstacles, and power-ups like TNT to optimize escape routes.
- **Dynamic Gameplay**: TNT creates chain reactions, destroying blocks and reducing traversal costs, while traps increase costs and add strategic challenges.

## **Example Output**
```plaintext
Cleared: 5 at [2,3]
Cleared: 7 at [3,3]
Cleared: 3 at [4,4]
Cleared: 2 at [5,5]
TNT explosion at [6,6]!
Cleared by TNT: 8 at [6,5]
Cleared by TNT: 4 at [5,6]
Cleared: 6 at [4,6]
Cleared: 9 at [1,2]
Cleared: 1 at [3,1]
Cleared: 6 at [2,2]
Median difficulty of clearing rubble is: 5.10
Cleared 10 tiles containing 51 rubble and escaped.
