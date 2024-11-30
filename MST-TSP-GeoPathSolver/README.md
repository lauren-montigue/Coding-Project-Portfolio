# **MST-TSP OptTSP**

## **Overview**
The MST-TSP OptTSP project implements algorithms to solve variants of the Traveling Salesperson Problem (TSP) using graph theory and optimization techniques. The project calculates a Minimum Spanning Tree (MST) to approximate TSP solutions and further refines these results using branch-and-bound optimization.

This project highlights my ability to work with advanced graph algorithms and implement performance-optimized solutions for computationally intensive problems.

## **Skills Demonstrated**
- **Graph Algorithms**: Implemented Minimum Spanning Tree (MST) construction to approximate TSP.
- **Optimization Techniques**: Designed and implemented OptTSP to refine initial solutions using pruning and recursion.
- **Algorithm Analysis**: Compared performance and solution accuracy of MST-TSP and OptTSP.
- **Performance Engineering**: Optimized recursive algorithms and graph traversals to handle large input sizes efficiently.
- **Data Structures**: Utilized adjacency matrices, vectors, and sets for efficient graph representation and traversal.

## **Key Features**
- **MST-TSP Approximation**:
  - Constructs a Minimum Spanning Tree (MST) from the input graph.
  - Traverses the MST to approximate a TSP solution using a pre-order walk.
- **OptTSP Optimization**:
  - Refines the TSP solution by recursively exploring alternative routes.
  - Includes pruning techniques to eliminate suboptimal paths early.
- **Input Flexibility**:
  - Supports graphs of various sizes and structures.
  - Reads weighted adjacency matrices for graph representation.
- **Performance Insights**:
  - Outputs runtime and quality comparisons for MST-TSP and OptTSP solutions.

## **Example Output**
### **Input**
```plaintext
Graph (Adjacency Matrix):
0  2  9 10
2  0  6  4
9  6  0  8
10 4  8  0
