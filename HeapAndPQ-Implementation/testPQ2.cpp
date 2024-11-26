// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

/*
 * Compile this test against your .h files to make sure they compile. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly. If you do not call a
 * function from here, that template is not instantiated and that function is
 * NOT compiled! So for instance, if you don't add code here to call
 * updatePriorities, that function could later cause compiler errors that you
 * don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes). This will be a debug executable.
 *
 * Notice that testPairing tests the range-based constructor but main and
 * testPriorityQueue do not. Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues. You have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>

#include "BinaryPQ.h"
#include "Eecs281PQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"
#include "UnorderedPQ.h"

// A type for representing priority queue types at runtime
enum class PQType {
    Unordered,
    Sorted,
    Binary,
    Pairing,
};

// These can be pretty-printed :)
std::ostream& operator<<(std::ostream& ost, PQType pqType) {
    switch (pqType) {
    case PQType::Unordered:
        return ost << "Unordered";
    case PQType::Sorted:
        return ost << "Sorted";
    case PQType::Binary:
        return ost << "Binary";
    case PQType::Pairing:
        return ost << "Pairing";
    }

    return ost << "Unknown PQType";
}


// Compares two int const* on the integers they point to
struct IntPtrComp {
    bool operator()(int const* a, int const* b) const { return *a < *b; }
};


// Test the primitive operations on a priority queue: constructor, push, pop, top, size, empty.
template <template <typename...> typename PQ>
void testPrimitiveOperations() {
    std::cout << "Testing the primitive priority queue operations..." << std::endl;
    std::cout<<"hi2" << std::endl;

    PQ<int> pq {};
    std::cout<<"hi1" << std::endl;
    Eecs281PQ<int>& eecsPQ = pq;
    std::cout<<"hi" << std::endl;
    eecsPQ.push(3);
    eecsPQ.push(4);
    assert(eecsPQ.size() == 2);
    assert(eecsPQ.top() == 4);

    eecsPQ.pop();
    assert(eecsPQ.size() == 1);
    assert(eecsPQ.top() == 3);
    assert(not eecsPQ.empty());

    eecsPQ.pop();
    assert(eecsPQ.size() == 0);
    assert(eecsPQ.empty());

    std::cout << "pop size empty and top seem okay" << std::endl;

    // TODO: Add more testing here!
    eecsPQ.push(3);
    eecsPQ.push(6);
    eecsPQ.push(3);
    eecsPQ.push(4);
    eecsPQ.push(5);
    eecsPQ.push(2);
    eecsPQ.push(7);
    eecsPQ.push(4);
    eecsPQ.push(0);
    eecsPQ.pop();
    eecsPQ.pop();
    eecsPQ.pop();
    eecsPQ.push(7);

    PQ<int> pq2 {};
    Eecs281PQ<int>& eecsPQ2 = pq2;

    assert(pq2.size() == eecsPQ2.size());

    std::cout << "sizes are equal, operator=" << std::endl;

    std::priority_queue<int> actualPQ;

    for(int i=0;i<1000;i++){
        int num=std::rand()%1000;
        actualPQ.push(num);
        eecsPQ2.push(num);
        //std::cout<<actualPQ.top()<<","<<eecsPQ2.top()<<std::endl;
        assert(eecsPQ2.top()==actualPQ.top());
    }

    std::cout << "hello again" << std::endl;

    eecsPQ2.updatePriorities();

    /*for(int i=999;i>=0;i--){
        std::cout<<actualPQ.top()<<","<<eecsPQ2.top()<<std::endl;
        assert(eecsPQ2.top()==actualPQ.top());
        
        std::cout<< "end of this loop" << std::endl;
        eecsPQ2.pop();
        std::cout<< "end of this loop" << std::endl;
        actualPQ.pop();
        std::cout<< "end of this loop" << std::endl;
    }*/
        std::cout << "Testing BinaryPQ pop and top operations..." << std::endl;

    BinaryPQ<int> binaryPQ;
    std::priority_queue<int> stdPQ;  // Using the standard priority queue for validation

    // Push random numbers into the priority queues
    for (int i = 0; i < 1000; ++i) {
        int num = std::rand() % 1000;
        binaryPQ.push(num);
        stdPQ.push(num);
    }

    // Check top and pop operations
    while (!binaryPQ.empty() && !stdPQ.empty()) {
        // Check if tops are the same
        assert(binaryPQ.top() == stdPQ.top());
        //std::cout << "BinaryPQ top: " << binaryPQ.top() << ", std::priority_queue top: " << stdPQ.top() << std::endl;

        // Pop the top elements
        binaryPQ.pop();
        stdPQ.pop();
    }

    // Ensure both priority queues are empty
    assert(binaryPQ.empty() && stdPQ.empty());

    std::cout << "BinaryPQ pop and top operations succeeded!" << std::endl;

    std::cout << "testPrimitiveOperations succeeded!" << std::endl;
}

// Test the last public member function of Eecs281PQ, updatePriorities
template <template <typename...> typename PQ>

void testUpdatePriorities() {
    std::cout<< "hello update priorities" << std::endl;

    std::vector<int> data {1,5,4,3,6,32,5,6,34,45,45,32,5,7,6,54,3,324,5,6,44,3,5,6,77,7,65,4,3};

    PQ<int const*, IntPtrComp> pq {};
    Eecs281PQ<int const*, IntPtrComp>& eecsPQ = pq;

    // NOTE: If you add more data to the vector, don't push the pointers
    //   until AFTER the vector stops changing size! Think about why.
    for (auto& datum : data) {
        eecsPQ.push(&datum);
    }

    // Change some element in data (which is pointed to by an element in pq).
    // This new value should be higher than any other so its address will wind
    //   qt the top adter updatePriorities.
    auto& datum = data[5];
    datum = 1000;
    eecsPQ.updatePriorities();
    assert(*eecsPQ.top() == 1000);
    assert(eecsPQ.top() == &datum);
    while(!eecsPQ.empty()){
        eecsPQ.pop();
    }

    // TODO: Add more testing here as you see fit.
}

// Test that the priority queue uses its comparator properly.
// HiddenData can't be compared with operator<, so we use HiddenDataComp{} instead.
template <template <typename...> typename PQ>
void testHiddenData() {
    struct HiddenData {
        int data;
    };

    std::cout << "hello" << std::endl;

    struct HiddenDataComp {
        bool operator()(HiddenData const&, HiddenData const&) const {
            // TODO: Finish this comparator; when you do, give the parameters
            //   names in the line above
            return false;
        }
    };

    std::cout << "Testing with hidden data..." << std::endl;

    // TODO: Add code here to actually test with the HiddenData type.
    // Consider writing this code in the style of testPrimitiveOperations above.

    std::cout << "testHiddenData succeeded!" << std::endl;
}





// Test the pairing heap's range-based constructor, copy constructor,
//   copy-assignment operator, and destructor
// TODO: Test other operations specific to this PQ type.
void testPairing() {
    std::cout << "Testing Pairing Heap separately..." << std::endl;

    {
        std::vector<int> const vec {
            1,
            0,
        };

        std::cout << "Calling constructors" << std::endl;

        // Range-based constructor
        PairingPQ<int> pairing1 { vec.cbegin(), vec.cend() };
        assert(pairing1.size() == 2);
        // Copy constructor
        PairingPQ<int> pairing2 { pairing1 };
        assert(pairing2.size() == 2);

        // Copy-assignment operator
        PairingPQ<int> pairing3 {};
        pairing3 = pairing2;
        assert(pairing3.size() == 2);

        // A reference to a PairingPQ<T> is a reference to an Eecs281PQ<T>.
        // Yay for polymorphism! We can therefore write:
        Eecs281PQ<int>& pq1 = pairing1;
        Eecs281PQ<int>& pq2 = pairing2;
        Eecs281PQ<int>& pq3 = pairing3;

        pq1.push(3);
        pq2.pop();
        assert(pq1.size() == 3);
        assert(not pq1.empty());
        assert(pq1.top() == 3);
        pq2.push(pq3.top());
        assert(pq2.top() == pq3.top());

        std::cout << "Basic tests done." << std::endl;

        // TODO: Add more code to test addNode, updateElt, etc.


        // That { above creates a scope, and our pairing heaps will fall out of
        //   scope at the matching } below.
        std::cout << "Calling destructors" << std::endl;
    }

    std::cout << "testPairing succeeded!" << std::endl;
}


// Run all tests for a particular PQ type.
template <template <typename...> typename PQ>
void testPriorityQueue() {
    testPrimitiveOperations<PQ>();
    testHiddenData<PQ>();
    testUpdatePriorities<PQ>();
}

void testPairing2(){
    PairingPQ<int> pair {};
    //Eecs281PQ<int>& eecsPQ2 = pair;
    std::priority_queue<int> actualPQ;
    int iterations=1000;
    for(int i=0;i<iterations;i++){
        int num=std::rand()%1000;
        actualPQ.push(num);
        pair.push(num);
        //std::cout<<actualPQ.top()<<","<<eecsPQ2.top()<<std::endl;
        assert(pair.top()==actualPQ.top());
    }
    PairingPQ<int> pair2 {};
    pair2=pair;
    
    for(int i=iterations-1;i>=0;i--){
        assert(pair2.top()==pair.top());
        //std::cout<<pair.top()<<","<<pair2.top()<<std::endl;
        pair2.pop();
        pair.pop();
    }
}
void testSimpleUpdateElt() {
    std::cout << "Testing simple updateElt function for max-heap..." << std::endl;

    PairingPQ<int> pq;

    // Add some initial values.
    std::vector<PairingPQ<int>::Node*> nodes;
    for (int i = 1; i <= 5; ++i) {
        nodes.push_back(pq.addNode(i));
        std::cout << "After adding " << i << ", top: " << pq.top() << std::endl;
    }
    std::cout << "noooooo" << std::endl;
    // The heap should have the largest value at the top.
    assert(pq.top() == 5);

    // Update the root node to a smaller value (should remain root).
    pq.updateElt(nodes[4], 4);
    assert(pq.top() == 4);

    // Update a non-root node to a smaller value (shouldn't become root).
    pq.updateElt(nodes[2], 2);
    assert(pq.top() == 4);

    // Update a non-root node to become the largest value (should become root).
    pq.updateElt(nodes[1], 6);
    assert(pq.top() == 6);

    std::cout << "Simple updateElt function for max-heap passed all tests!" << std::endl;
}

void testUpdateElt() {
    std::cout << "Testing updateElt function for max-heap..." << std::endl;

    PairingPQ<int> pq;

    // Add some initial values.
    std::vector<PairingPQ<int>::Node*> nodes;
    for (int i = 1; i <= 10; ++i) {
        nodes.push_back(pq.addNode(i));
        std::cout << "Added value: " << i << ", current top: " << pq.top() << std::endl;
    }

    // Update the root node to a smaller value (should remain root).
    pq.updateElt(nodes[9], 9);
    std::cout << "After updating nodes[9] to 9, top: " << pq.top() << std::endl;

    // Update a non-root node to a smaller value (shouldn't become root).
    pq.updateElt(nodes[4], 3);
    std::cout << "After updating nodes[4] to 3, top: " << pq.top() << std::endl;

    assert(pq.top() == 9);

    // Update a non-root node to become the largest value (should become root).
    pq.updateElt(nodes[4], 11);
    assert(pq.top() == 11);

    // Update the root node to a larger value (should remain root).
    pq.updateElt(nodes[4], 12);
    assert(pq.top() == 12);

    // Try updating a node after popping a few elements.
    pq.pop();
    pq.pop();
    pq.updateElt(nodes[2], 13);
    assert(pq.top() == 13);

    // Update a node to the same value (shouldn't change anything).
    pq.updateElt(nodes[2], 13);
    assert(pq.top() == 13);

    std::cout << "updateElt function for max-heap passed all tests!" << std::endl;
}




void testPairingHeap() {
    // 1. Empty Heap Operations
    PairingPQ<int> emptyHeap;
    assert(emptyHeap.size() == 0);
    assert(emptyHeap.empty());

    // 2. Single Element Operations
    emptyHeap.push(5);
    assert(emptyHeap.top() == 5);
    emptyHeap.pop();
    assert(emptyHeap.empty());

    // 3. Duplicate Elements
    for (int i = 0; i < 5; ++i) {
        emptyHeap.push(10);
    }
    assert(emptyHeap.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(emptyHeap.top() == 10);
        emptyHeap.pop();
    }

    // 5. Update Priorities
    // Assuming you have a way to get a node pointer when you add an element
    // PairingPQ<int>::Node* node5 = emptyHeap.addNode(5);
    // PairingPQ<int>::Node* node10 = emptyHeap.addNode(10);
    // emptyHeap.updateElt(node5, 15);
    // assert(emptyHeap.top() == 10);
    // emptyHeap.pop();
    // assert(emptyHeap.top() == 15);

    // 7. Copy and Assignment
    PairingPQ<int> heapForCopy;
    heapForCopy.push(5);
    PairingPQ<int> copiedHeap = heapForCopy;
    assert(copiedHeap.top() == 5);

    // 8. Melding
    // Assuming you have a meld function
    // PairingPQ<int> heap1, heap2;
    // heap1.push(1); heap1.push(3);
    // heap2.push(2); heap2.push(4);
    // heap1.meld(heap2);
    // assert(heap1.size() == 4);
    // assert(heap1.top() == 1);

    // 12. Stress Test
    for (int i = 0; i < 10000; ++i) {
        emptyHeap.push(i);
        assert(emptyHeap.top() == i);
        emptyHeap.pop();
    }

    std::cout << "All tests passed!" << std::endl;
}



void testPairingUpdate(){
    PairingPQ<int> pair {};
    std::priority_queue<int> actualPQ;
    //int max=0;
    for(int i=0;i<1000000;i++){
        int num=std::rand()%1000;
        auto node=pair.addNode(num);
        if(num%10==5){
            int num_in=std::rand()%1000;
            if(num_in>=node->getElt()){
                //std::cout<<"Updated Elt "<<node->getElt()<<" to "<<num_in<<std::endl;
                pair.updateElt(node,num_in);
                actualPQ.push(num_in);
            }
            else{
                actualPQ.push(num);
            }
        }
        else{
            actualPQ.push(num);
        }
        //std::cout<<i<<","<<pair.top()<<std::endl;
        //assert(pair.top()==max);
    }
    int prev=pair.top();
    for(int i=1;i<1000000;i++){
        pair.pop();
        actualPQ.pop();
        assert(pair.top()<=prev);
        //std::cout<<pair.top()<<","<<actualPQ.top()<<std::endl;
        assert(pair.top()==actualPQ.top());
        prev=pair.top();
    }


    std::cout << "Test Pairing Update PASSED" << std::endl;
}
void testComplexUpdateElt() {
    std::cout << "Testing complex updateElt function for max-heap..." << std::endl;

    PairingPQ<int> pq;

    // Add some initial values.
    std::vector<PairingPQ<int>::Node*> nodes;
    for (int i = 1; i <= 50; ++i) { // Change here
        nodes.push_back(pq.addNode(i));
        std::cout << "Added value: " << i << ", current top: " << pq.top() << std::endl;
    }

    // ... [rest of the code remains unchanged]

    // Update multiple nodes in a sequence.
    for (int i = 0; i < 25; ++i) { // Change here
        pq.updateElt(nodes[static_cast<size_t>(i)], static_cast<int>(200 + i)); // Change here
        assert(pq.top() == 200 + i);
    }

    std::cout << "Complex updateElt function for max-heap passed all tests!" << std::endl;
}
void testBinaryPQUpdatePriorities() {
    std::cout << "Testing BinaryPQ updatePriorities operation..." << std::endl;

    BinaryPQ<int> binaryPQ;
    std::priority_queue<int> stdPQ;  // Using the standard priority queue for validation

    // Push random numbers into the priority queues
    for (int i = 0; i < 1000; ++i) {
        int num = std::rand() % 1000;
        binaryPQ.push(num);
        stdPQ.push(num);
    }

    // Perform some operations to make the BinaryPQ disorderly (e.g., alternating pops and pushes)
    for (int i = 0; i < 500; ++i) {
        binaryPQ.pop();
        stdPQ.pop();
        int num = std::rand() % 1000;
        binaryPQ.push(num);
        stdPQ.push(num);
    }

    // Now, call updatePriorities to restore the BinaryPQ
    binaryPQ.updatePriorities();

    // Check top operations to validate the correct order after calling updatePriorities
    while (!binaryPQ.empty() && !stdPQ.empty()) {
        // Check if tops are the same
        assert(binaryPQ.top() == stdPQ.top());
        
        // Pop the top elements
        binaryPQ.pop();
        stdPQ.pop();
    }

    // Ensure both priority queues are empty
    assert(binaryPQ.empty() && stdPQ.empty());

    std::cout << "BinaryPQ updatePriorities operation succeeded!" << std::endl;
}

void testBinaryPQPopAndTop() {
    std::cout << "Testing BinaryPQ pop and top operations..." << std::endl;

    BinaryPQ<int> binaryPQ;
    std::priority_queue<int> stdPQ;  // Using the standard priority queue for validation

    // Push random numbers into the priority queues
    for (int i = 0; i < 1000; ++i) {
        int num = std::rand() % 1000;
        binaryPQ.push(num);
        stdPQ.push(num);
    }

    // Check top and pop operations
    while (!binaryPQ.empty() && !stdPQ.empty()) {
        // Check if tops are the same
        assert(binaryPQ.top() == stdPQ.top());
        //std::cout << "BinaryPQ top: " << binaryPQ.top() << ", std::priority_queue top: " << stdPQ.top() << std::endl;

        // Pop the top elements
        binaryPQ.pop();
        stdPQ.pop();
    }

    // Ensure both priority queues are empty
    assert(binaryPQ.empty() && stdPQ.empty());

    std::cout << "BinaryPQ pop and top operations succeeded!" << std::endl;
}


// void testBinaryPQComp() {
//     std::cout << "Starting comprehensive BinaryPQ tests..." << std::endl;

//     // 1. Basic push and top
//     BinaryPQ<int> pq;
//     for (int i = 1; i <= 10; ++i) {
//         pq.push(i);
//         assert(pq.top() == i);
//     }

//     // 2. Basic pop
//     for (int i = 10; i >= 1; --i) {
//         assert(pq.top() == i);
//         pq.pop();
//     }

//     // 3. Push in reverse and check
//     for (int i = 10; i >= 1; --i) {
//         pq.push(i);
//         assert(pq.top() == 10);
//     }
//     // 2. Basic pop
//     for (int i = 10; i >= 1; --i) {
//         assert(pq.top() == i);
//         pq.pop();
//     }

//     // 4. Randomized push and check
//     pq.push(5);
//     pq.push(10);
//     pq.push(1);
//     pq.push(7);
//     pq.push(3);
//     assert(pq.top() == 10);


//     pq.pop();
//     assert(pq.top() == 7);
//     pq.pop();
//     assert(pq.top() == 5);
//     pq.pop();
//     assert(pq.top() == 3);

//     // 6. Check empty
//     BinaryPQ<int> emptyPQ;
//     assert(emptyPQ.empty());

//     // 7. Iterator constructor and updatePriorities
//     std::vector<int> vec = {5, 3, 8, 1, 9, 2};
//     BinaryPQ<int> iterPQ(vec.begin(), vec.end());
//     assert(iterPQ.top() == 9);
//     vec[0] = 10;
//     iterPQ.updatePriorities();
//     assert(iterPQ.top() == 10);

//     // 8. Copy constructor
//     BinaryPQ<int> copyPQ = iterPQ;
//     assert(copyPQ.top() == 10);

//     // 9. Large number of elements
//     BinaryPQ<int> largePQ;
//     for (int i = 1; i <= 1000; ++i) {
//         largePQ.push(i);
//         assert(largePQ.top() == i);
//     }
//     for (int i = 1000; i >= 1; --i) {
//         assert(largePQ.top() == i);
//         largePQ.pop();
//     }

//     std::cout << "All BinaryPQ tests passed!" << std::endl;
// }




// PairingPQ has some extra behavior we need to test in updateElement.
// This template specialization handles that without changing the nice uniform
//   interface of testPriorityQueue.
template <>
void testPriorityQueue<PairingPQ>() {
    testPrimitiveOperations<PairingPQ>();
    testHiddenData<PairingPQ>();
    testUpdatePriorities<PairingPQ>();
    std::cout<<"helllllooooo:"<<std::endl;
    testPairing();
    testPairing2();
    testPairingHeap();
    testSimpleUpdateElt();
    testUpdateElt();
    testComplexUpdateElt();
    testPairingUpdate();
}




int main() {
    std::vector<PQType> const types {
        PQType::Unordered,
        PQType::Sorted,
        PQType::Binary,
        PQType::Pairing,
    };

    std::cout << "PQ tester" << std::endl << std::endl;
    int idx { 0 };
    for (auto const& type : types) {
        std::cout << "  " << idx++ << ") " << type << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Select one: ";
    int choice {};
    std::cin >> choice;
    PQType const pqType = types.at((size_t)choice);

    std::cout << "Testing the " << pqType << " PQ...";
    

    // TODO: Add more cases to test other priority queue types.
    switch (pqType) {
    case PQType::Unordered:
        testPriorityQueue<UnorderedPQ>();
        break;
    case PQType::Sorted:
        testPriorityQueue<SortedPQ>();
        break;
    case PQType::Binary:
        testPriorityQueue<BinaryPQ>();
        testBinaryPQUpdatePriorities();
        break;
    case PQType::Pairing:
        testPriorityQueue<PairingPQ>();
        break;
    default:
        std::cout << "Unrecognized PQ type " << pqType << " in main." << std::endl
                  << "Perhaps you forgot to add tests for all four PQ types." << std::endl;
        return 1;
    }

    std::cout << "All tests succeeded!" << std::endl;
    return 0;
}
