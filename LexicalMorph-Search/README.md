# **Lexical Morph Search**

## **Overview**
The Lexical Morph Search project determines whether a sequence of valid word transformations (morphs) exists between a given start and end word, using a provided dictionary. This project explores graph traversal, word transformation rules, and efficient pathfinding.

This project is particularly significant to me as it is my first without any starter code. Every file, function, and feature was designed and implemented from scratch, showcasing my ability to create an end-to-end system based solely on the project specifications.

As this project included additional formatting rules for input files to increase complexity, I have only included the output here to clarify the purpose and functionality of the project.

## **Skills Demonstrated**
- **Graph Theory and Traversal**: Modeled word transformations as graph edges and implemented traversal algorithms (stack-based DFS and queue-based BFS) to find valid transformation paths.
- **Algorithmic Design**: Created efficient rules for word transformations, including letter swaps, additions, deletions, and replacements.
- **Data Structures**: Leveraged custom data structures for dictionary entries and transformation tracking.
- **Command-Line Interface**: Developed a robust CLI to allow users to specify word pairs, modification rules, and search strategies.
- **Problem-Solving**: Built an efficient, modular system to handle complex requirements and edge cases, including validation and error handling.

## **Key Features**
- **Transformation Rules**:
  - Letter replacement (change).
  - Letter insertion or deletion (length).
  - Adjacent letter swaps (swap).
- **Search Strategies**:
  - Stack-based depth-first search (DFS).
  - Queue-based breadth-first search (BFS).
- **Detailed Output**:
  - In "word mode," outputs the sequence of words from start to end.
  - In "morph mode," outputs the specific transformations applied.
- **Input Parsing**:
  - Reads from a dictionary file and validates start and end words.

### **Example Output**

#### **Word Mode**
```plaintext
Words in morph: 5
start
strat
srat
rat
mat
