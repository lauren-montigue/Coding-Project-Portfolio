// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

//basic idea: pairing heap that produces pairing pq
//all held together with nodes. every node has a child pointer and a sibling pointer
//then one more pointer of your choice: parent or previous.
//node never has higher priority than its parent
//no vector or deque of child pointers
//root pointer that points to a node. root cannot have sibling or parent
//add member variables yourself - root, number of nodes (dont call it size, there will be a member function called size)
//once you add member variables, modify all constructors (default, range based, copy)
//to be valid, all three constructors must initialize all to nullptr and zero
//parent or previous (distinciton at 30:00) important for updateElt
//meld and anyone that calls it - check for proper use of parent or previous
//DONT: use recursion. lol wasnt going to. never think of it.
//push is simple - just calls addNode, which depends on meld
//meld is given two node pointers and return another node pointer
//meld says im putting these two pairing heaps together
//parameters are essentially the roots so they must have no parent/prev and no sibling
//push - create NEW new Node . then root can be that. s
//push. create new Node with 81 and call meld with root and new Node
//now meld has to figure out which of these should be root? calls this->compare
//if (this->compare(a->elt,b->elt)). if returns true, second should be root
//now the two nodes have to recognize each other as parent and child. then return pointer to bigger one.
//add node will change root, NOT meld
//who could call meld with a nullptr in theory? make sure this function deals w nullptr so meld doesnt have to
//do that check and it can be faster
//can only update elt with a higher priority, NOT lower
//when you pop, there could be multiple siblings so you need to choose highest priority sibling to be new root
//how do we do this? delete node and point to direct child
//make a deque of node pointers
//now determine new root and PAIR
//cant just keep melding. horrible efficiency.
//now do multi pass. easier to code. 
//if you wanted to do two pass. could meld two then the next two then maybe you end up with one extra. right to left 
//to put in and left to right to take out. thats the two passes
//now lets look at multi pass
//take two things, meld, and push to the back. then the next thing will be the end. circular until you get everything in your meld
//


#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            // to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            // position.  There are two versions, getElt() and a dereference
            // operator, use whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            // TODO: Add one extra pointer (parent or previous) as desired.
            Node *parent;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp },
        root{ nullptr },
        numNodes{ 0 } {
        // No additional implementation needed
    }


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp },
        root{ nullptr },
        numNodes{ 0 } {
        // TODO: Implement this function.
        //This is how tutorial says to do it
        for (auto it = start; it != end; ++it) {
            push(*it);
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    // I think I'm creating a shallow copy here
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare },
        root{ nullptr }, // Initialize root to nullptr (NOT to other.root - this would make a shallow copy)
        numNodes{ 0 } // Initialize numNodes with zero because push increments
        {
        if (!other.root) {
            return; // If the other pairing heap is empty, no need to copy anything
        }
        
        std::deque<Node*> nodesToCopy; // Deque acting as a stack to track nodes to copy
        
        // Create a new root node
        //std::cout << size() << std::endl;

        nodesToCopy.push_back(other.root); // Push the root of the other pairing heap        
        
        // Perform iterative traversal to copy nodes
        while (!nodesToCopy.empty()) {
            Node* current = nodesToCopy.front(); // Get the top node from the deque
            nodesToCopy.pop_front(); // Pop the top node

            //Node* copy;

            push(current->elt);

            // Copy child nodes and siblings
            if (current->child) {
                // Create a copy of the child node
                nodesToCopy.push_back(current->child); // Push the child node for further copying
            }
            if (current->sibling) {
                // Create a copy of the sibling node
                nodesToCopy.push_back(current->sibling); // Push the sibling node for further copying
            }
            
        }

        //first check if other.root exists
        //then make a deque and push back from other info
        //while deque is not empty, new node equal to front of deque
        
    }// PairingPQ() 


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        // lecture.
        PairingPQ temp(rhs);

        //swap this objects data and the length with those from temp
        std::swap(numNodes, temp.numNodes);
        std::swap(root, temp.root);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        if (root) {
            // Deque to keep track of nodes to delete
            std::deque<Node*> nodesToDelete;
            nodesToDelete.push_front(root);

            while (!nodesToDelete.empty()) {
                Node* current = nodesToDelete.front();
                nodesToDelete.pop_front();

                // Push child nodes and siblings onto the deque
                if (current->child) {
                    nodesToDelete.push_front(current->child);
                    current->child = nullptr;
                }
                if (current->sibling) {
                    nodesToDelete.push_front(current->sibling);
                    current->sibling = nullptr;
                }

                // Delete the current node
                delete current;
            }
            
            root = nullptr; // Set the root to nullptr after deleting all nodes
        }
    }// ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        // walk through, add child and sibling, do something with current
        // never push parent or previous - inf loop
        // basic approach is like copy constructor and destructor
        // going to have to tear apart one node at a time and rebuild
        // root is nullptr
        // cannot delete nodes and push new ones
        // updatePriorities and updateElts CANNOT delete. only pop and destructor
        // root point to root. keep looping and adding things nearby

        if (!root) {
            return; // If the heap is empty, there's nothing to update
        }

        // Create a deque to store nodes to be updated
        std::deque<Node*> nodesToUpdate;
        nodesToUpdate.push_back(root);

        // Iterate through the nodes to update priorities
        while (!nodesToUpdate.empty()) {
            Node* current = nodesToUpdate.front();
            nodesToUpdate.pop_front();

            // Add child nodes and siblings to the deque for further processing
            if (current->child) {
                nodesToUpdate.push_back(current->child);
            }
            if (current->sibling) {
                nodesToUpdate.push_back(current->sibling);
            }

            // Detach the current node from its parent's children list
            if (current->parent) {
                // Find the previous sibling of the current node
                Node* prev_sibling = nullptr;
                Node* sibling = current->parent->child;
                while (sibling != current) {
                    prev_sibling = sibling;
                    sibling = sibling->sibling;
                }

                // Detach the current node from its parent's children list
                if (prev_sibling) {
                    prev_sibling->sibling = current->sibling;
                } else {
                    current->parent->child = current->sibling;
                }

                // Set the current node's parent pointer to nullptr
                current->parent = nullptr;
            }

            // Reinsert the current node into the pairing heap by melding it with the root
            root = meld(root, current);
        }
        } // updatePriorities()


        // Description: Add a new element to the pairing heap. This is already
        //              done. You should implement push functionality entirely
        //              in the addNode() function, and this function calls
        //              addNode().
        // Runtime: O(1)
        virtual void push(const TYPE &val) {
            addNode(val);
        } // push()


        // Description: Remove the most extreme (defined by 'compare') element
        //              from the pairing heap.
        // Note: We will not run tests on your code that would require it to pop
        // an element when the pairing heap is empty. Though you are welcome to
        // if you are familiar with them, you do not need to use exceptions in
        // this project.
        // Runtime: Amortized O(log(n))
        virtual void pop() {

            if (empty())
            {
                return;
            }

            else if (!root->child)
            {
                // Delete the old root node
                delete root;

                root = nullptr;

                // Update the count of nodes
                --numNodes;
            }
            else{

                Node* newRoot = root->child;

                std::deque<Node*> siblings;
                siblings.push_back(newRoot);
                Node* curr = newRoot->sibling;
                while (curr) {
                    siblings.push_back(curr);
                    curr = curr->sibling;
                }

                while (siblings.size() > 1) {
                    Node* sibling1 = siblings.front();
                    siblings.pop_front();
                    Node* sibling2 = siblings.front();
                    siblings.pop_front();

                    sibling1->parent = nullptr;
                    sibling1->sibling = nullptr;

                    sibling2->parent = nullptr;
                    sibling2->sibling = nullptr;

                    Node * together = meld(sibling1, sibling2);
                    siblings.push_back(together);
                }

                // Delete the old root node
                delete root;
    
                // Set the new root
                root = siblings.front();
                root->parent = nullptr;
                root->sibling = nullptr;

                siblings.pop_front();

                // Update the count of nodes
                --numNodes;

            }

                
        }
    


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function

        //Check if it's empty
        /*if (empty())
        {
            //error
        }
        else*/
            return root->elt;    // TODO: Delete or change this line
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function
        return numNodes; // TODO: Delete or change this line
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function
        return (numNodes == 0); // TODO: Delete or change this line
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        // Update the value of the node
        node->elt = new_value;

        // If the updated node is not the root, detach it from its parent's children list
        if (node->parent) {
            // Find the previous sibling of the updated node
            Node* prev_sibling = nullptr;
            Node* sibling = node->parent->child;
            while (sibling != node) {
                prev_sibling = sibling;
                sibling = sibling->sibling;
            }

            // Detach the updated node from its parent's children list
            if (prev_sibling) {
                prev_sibling->sibling = node->sibling;
            } else {
                node->parent->child = node->sibling;
            }
        }

        // Reinsert the updated node into the pairing heap by melding it with the root
        // Can I call meld?? watch video again
        root = meld(root, node);
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node* addNode(const TYPE &val) {
        // Create a new node with the given value
        Node* newNode = new Node(val);
        
        // Meld the newly created node with the existing root
        //is this O(1)??

        if (root == nullptr)
        {
            root = newNode;
        }
        else
            root = meld(root, newNode);

        // Increment the count of nodes
        ++numNodes;

        // Return a pointer to the newly added node
        return newNode;
    } // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers).

    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.

    Node* root;
    int numNodes;

    Node* meld(Node* rootA, Node* rootB) {
        //make sure to check whether one is nullptr, just not here.
        //paoletti said handle that in whatever would call it with a nullptr...
        //in add node do this

        //ALSO make sure that they have no parent or previous

        // Determine new root based on priority comparison
        if (this->compare(rootA->elt, rootB->elt)) {
            // rootB has higher priority, so rootA becomes a child of rootB
            if (rootB->child)
            {
                rootA->sibling = rootB->child;
            }
            rootA->parent = rootB;  // Set rootA's parent pointer to rootB
            rootB->child = rootA;
            return rootB;
        } else {
            if (rootB->child)
            {
                rootA->sibling = rootB->child;
            }
            // rootA has higher priority, so rootB becomes a child of rootA
            rootB->parent = rootA;  // Set rootB's parent pointer to rootA
            rootB->sibling = rootA->child;
            rootA->child = rootB;
            return rootA;
        }
    }

    

};


#endif // PAIRINGPQ_H
