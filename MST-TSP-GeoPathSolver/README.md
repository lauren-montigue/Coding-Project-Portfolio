# **MST-TSP OptTSP**

## **Overview**
The MST-TSP OptTSP project explores three key algorithms to solve and optimize the Traveling Salesperson Problem (TSP), a fundamental challenge in graph theory:

1. **MST**: Uses a Minimum Spanning Tree (MST) to approximate TSP solutions.
2. **FASTTSP**: Constructs a near-optimal TSP solution using the arbitrary insertion method.
3. **OPTTSP**: Improves TSP solutions from FASTTSP with a combination of permutation generation and pruning strategies.

This project showcases my understanding of graph algorithms, heuristic methods, and optimization techniques to tackle computationally complex problems.

## **Skills Demonstrated**
- **Graph Algorithm Implementation**: Designed Prim's Algorithm for MST construction and applied it to approximate TSP solutions.
- **Heuristic Development**: Utilized Arbitrary Insertion to quickly generate near-optimal TSP paths in FASTTSP.
- **Optimization via Pruning**: Developed a recursive solution for OptTSP that leverages promising paths and discards non-optimal candidates based on dynamic bounds.
- **Recursive Problem-Solving**: Implemented efficient backtracking and path evaluation to refine solutions.
- **Mathematical Computing**: Calculated Euclidean distances, dynamic costs, and lower bounds to evaluate path viability.

## **Key Features**
- **MST-TSP Approximation**:
  - Constructs a Minimum Spanning Tree (MST) using Prim's Algorithm.
  - Approximates TSP solutions with a pre-order traversal of the MST.
- **FASTTSP (Arbitrary Insertion)**:
  - Greedy insertion heuristic minimizes the incremental cost of adding vertices to the TSP path.
  - Produces a near-optimal path with lower computational overhead.
- **OptTSP Optimization**:
  - Permutation generation refines the path order to optimize TSP solutions.
  - Prunes suboptimal paths early by dynamically calculating lower bounds.
  - Combines MST-derived bounds with arm-length calculations to improve efficiency.
- **Output and Metrics**:
  - Provides the TSP path and total cost for each algorithm.
  - Measures algorithmic performance through runtime and accuracy.

## **Example Output**

### **Input**
```plaintext
Vertices:
0: (0, 0), CampusType: kCampusA
1: (0, 3), CampusType: kCampusB
2: (4, 0), CampusType: kBorder
3: (4, 3), CampusType: kCampusA
