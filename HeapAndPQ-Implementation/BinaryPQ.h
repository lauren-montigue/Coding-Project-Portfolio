// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef BINARYPQ_H
#define BINARYPQ_H

#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function, or verify that it is already done
        data.push_back(TYPE{});
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{ start, end }{
        // TODO: Implement this function
        // Iterate over the range defined by start and end iterators
        data.insert(data.begin(), TYPE{});
        updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    // HEAPIFY I THINK
    virtual void updatePriorities() {
        // TODO: Implement this function.
        for (size_t i = size(); i >= 1; --i) {
            fixDown(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        // TODO: Implement this function.
        data.push_back(val);
        fixUp(size()); //originally had heapify but dont want to rebuild heap in O(n) time. fixUp can do in log(n), as required by this function
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the PQ is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        //IDIOT CHECK
        // If the heap is empty, do nothing
        if (empty()) return;

        data[1] = data.back();

        // Remove the last element
        data.pop_back();

        // Restore the heap property by fixing down from the root
        fixDown(1); // Use 1-based indexing
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function.

        // These lines are present only so that this provided file compiles.
        return data[1];
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return data.size() - 1; // TODO: Delete or change this line
    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        //QUESTION: is this calling my size that accounts for 1-based indexing?
        return (size() == 0); // TODO: Delete or change this line
    } // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    // TODO: Add any additional member functions you require here.
    //       For instance, you might add fixUp() and fixDown().

    //Pass in index of node k
    void fixDown(size_t index) {
        while (2 * index <= size()) {
            size_t larger_child = 2 * index; // left child
            if (larger_child < size() && this->compare(data[larger_child], data[larger_child + 1])) {
                ++larger_child; // right child is larger
            }
            if (this->compare(data[larger_child], data[index])) {
                break; // heap property is satisfied
            }
            std::swap(data[index], data[larger_child]);
            index = larger_child;
        }
    }

    void fixUp(size_t k) {
        while (k > 1 && this->compare(data[k / 2], data[k])) {
            std::swap(data[k], data[k / 2]);
            k /= 2;
        }
        
    }

}; // BinaryPQ


#endif // BINARYPQ_H
