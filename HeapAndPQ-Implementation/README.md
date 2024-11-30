# **Priority Queue and Heap Implementation**

## **Overview**
This project implements three types of priority queues —**binary heap-based**, **sorted array-based**, and **pairing heap-based**. A large aspect of this project was developing a deep understanding of how these data structures operate under the hood. 

For the **pairing heap-based priority queue**, I worked from a detailed document outlining its theoretical functionality and had to translate that into an efficient implementation. This task required designing and coding directly from the conceptual description of the data structure, focusing both on performance and accuracy.

While some starter code (e.g., project scaffolding and helper classes) was provided, I wrote the core implementations of the priority queue functions in the following files:
- **`SortedPQ.h`**: Sorted array-based priority queue.
- **`BinaryPQ.h`**: Binary heap-based priority queue.
- **`PairingPQ.h`**: Pairing heap-based priority queue.

All other files in this project were provided as part of the starter code.

## **Skills Demonstrated**
- **Conceptual Understanding**: Gained a deep knowledge of priority queue operations and their underlying data structures, including binary heaps, sorted arrays, and pairing heaps.
- **Efficient Implementation**: Translated theoretical descriptions of pairing heaps into an optimized and functional implementation, focusing on performance and memory management.
- **Dynamic Data Handling**: Managed complex pointer-based relationships for the pairing heap, ensuring efficient memory usage and robust performance.

## **Key Features**
- **Binary Heap-Based Priority Queue**:
  - Efficient insertion (`push`) and deletion (`pop`) with logarithmic runtime.
  - Reordering with `updatePriorities` for O(n) complexity.
- **Sorted Array-Based Priority Queue**:
  - Maintains a sorted array for direct access to the highest-priority element with O(1) runtime using `top`.
  - Reordering with sorting algorithms for O(n log n) complexity.
- **Pairing Heap-Based Priority Queue**:
  - Implements a dynamic, node-based structure with child-sibling relationships.
  - Supports efficient `meld` operations for combining heaps.
  - Handles pointer-based priority updates with `updateElt`.

## **Example Usage**
```cpp
#include "BinaryPQ.h"
#include "SortedPQ.h"
#include "PairingPQ.h"
#include <iostream>
#include <vector>

int main() {
    // Binary Heap-Based Priority Queue Example
    BinaryPQ<int> binaryPQ;
    binaryPQ.push(15);
    binaryPQ.push(10);
    binaryPQ.push(20);
    std::cout << "BinaryPQ Top Element: " << binaryPQ.top() << std::endl;
    binaryPQ.pop();
    std::cout << "BinaryPQ Top After Pop: " << binaryPQ.top() << std::endl;

    // Sorted Array-Based Priority Queue Example
    SortedPQ<int> sortedPQ;
    sortedPQ.push(30);
    sortedPQ.push(25);
    sortedPQ.push(35);
    std::cout << "SortedPQ Top Element: " << sortedPQ.top() << std::endl;
    sortedPQ.pop();
    std::cout << "SortedPQ Top After Pop: " << sortedPQ.top() << std::endl;

    // Pairing Heap-Based Priority Queue Example
    PairingPQ<int> pairingPQ;
    pairingPQ.push(50);
    pairingPQ.push(40);
    pairingPQ.push(60);
    std::cout << "PairingPQ Top Element: " << pairingPQ.top() << std::endl;
    pairingPQ.pop();
    std::cout << "PairingPQ Top After Pop: " << pairingPQ.top() << std::endl;

    return 0;
}
