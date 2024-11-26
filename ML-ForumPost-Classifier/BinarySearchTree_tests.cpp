// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

TEST(checking_lots_of_things) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "");
    ASSERT_EQUAL(oss_preorder.str(), oss_inorder.str());

    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    tree.insert(1);
    tree.insert(3);
    tree.insert(6);

    //from variables in debugger, tree is set up correctly.

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree.size() == 6); 
    ASSERT_TRUE(tree.height() == 3);

    ASSERT_TRUE(*tree.begin() == 1);

    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 6);

    ASSERT_TRUE(tree.max_element() == tree.min_greater_than(5));
    ASSERT_TRUE(tree.min_element() == tree.min_greater_than(0));

    ASSERT_TRUE(*tree.min_greater_than(0) == 1);
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree.min_greater_than(3) == 4);
    ASSERT_EQUAL(tree.min_greater_than(6), tree.end());  
    
    ASSERT_EQUAL(tree.find(7), tree.end());
    ASSERT_EQUAL(tree.find(0), tree.end());
    ASSERT_EQUAL(*tree.find(1), 1);
    ASSERT_EQUAL(tree.find(1), tree.begin());
    ASSERT_EQUAL(tree.find(1), tree.begin()); 
    ASSERT_TRUE(tree.find(4) != tree.end());
    ASSERT_TRUE(tree.find(4) != tree.end());
    ASSERT_TRUE(tree.find(3) != tree.end()); 

    ostringstream oss_preorder2;
    tree.traverse_preorder(oss_preorder2);
    ASSERT_EQUAL(oss_preorder2.str(), "4 2 1 3 5 6 ");

    ostringstream oss_inorder2;
    tree.traverse_inorder(oss_inorder2);
    ASSERT_EQUAL(oss_inorder2.str(), "1 2 3 4 5 6 ");

    tree.insert(7);

    tree.traverse_preorder(oss_preorder2);
    ASSERT_EQUAL(oss_preorder2.str(), "4 2 1 3 5 6 4 2 1 3 5 6 7 ");

    tree.traverse_inorder(oss_inorder2);
    ASSERT_EQUAL(oss_inorder2.str(), "1 2 3 4 5 6 1 2 3 4 5 6 7 ");

    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 4);
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 7);
    ASSERT_TRUE(tree.find(7) != tree.end()); 

    cout << tree.to_string() << endl;
    cout << tree << endl;

    //testing iterator operator overloads
    ASSERT_EQUAL(*(++tree.find(1)), 2);
    ASSERT_EQUAL(*(++tree.find(2)), 3);
    ASSERT_EQUAL(*(++tree.find(3)), 4);
    ASSERT_EQUAL(*(++tree.find(4)), 5);
    ASSERT_EQUAL(*(++tree.find(5)), 6);
    ASSERT_EQUAL(*(++tree.find(6)), 7);
    ASSERT_TRUE(++tree.find(7) == tree.end());
}

TEST(test_height) {
    BinarySearchTree<int> tree; // when longest height is not the leftmost branch

    tree.insert(5);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);

    ASSERT_EQUAL(tree.height(), 4);
}

TEST(test_traversal) {
    // This is based on an example from Beaumont's lecture so 99% sure its right
    BinarySearchTree<int> tree;

    tree.insert(3);
    tree.insert(2);
    tree.insert(1);
    tree.insert(7);
    tree.insert(4);
    tree.insert(6);
    tree.insert(5);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "3 2 1 7 4 6 5 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "1 2 3 4 5 6 7 ");
}

TEST(node_size_one) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    tree.insert(3);

    ostringstream oss_preorder1;
    tree.traverse_preorder(oss_preorder1);
    ASSERT_EQUAL(oss_preorder1.str(), "3 ");

    ostringstream oss_inorder1;
    tree.traverse_inorder(oss_inorder1);
    ASSERT_EQUAL(oss_inorder1.str(), "3 ");
    ASSERT_EQUAL(oss_preorder1.str(), oss_inorder1.str());

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    ASSERT_TRUE(*tree.min_greater_than(1) == 3);
    ASSERT_TRUE(*tree.begin() == 3);
    ASSERT_TRUE(*tree.min_element() == 3);
    ASSERT_TRUE(*tree.max_element() == 3);

    ASSERT_EQUAL(tree.find(3), tree.begin());
    ASSERT_EQUAL(tree.find(5), tree.end());
    ASSERT_EQUAL(*tree.find(3), 3);
    
    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "3 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "3 ");

    cout << tree.to_string() << endl;
    cout << tree << endl;

    ASSERT_TRUE(++tree.begin() == tree.end());
}

TEST(left_stick_tree) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    tree.insert(4);
    tree.insert(3);
    tree.insert(2);
    tree.insert(1);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 4);
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_EQUAL(tree.min_greater_than(4), tree.end());
    ASSERT_TRUE(*tree.begin() == 1);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 4);

    ASSERT_EQUAL(tree.find(1), tree.begin());
    ASSERT_EQUAL(tree.find(5), tree.end());
    ASSERT_EQUAL(*tree.find(3), 3);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "4 3 2 1 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "1 2 3 4 ");

    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << tree.to_string() << endl;
    cout << tree << endl;

    ASSERT_EQUAL(*(++tree.find(1)), 2);
    ASSERT_EQUAL(*(++tree.find(2)), 3);
    ASSERT_EQUAL(*(++tree.find(3)), 4);
    ASSERT_TRUE(++tree.find(4) == tree.end());
}

TEST(right_stick_tree) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 4);
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_EQUAL(tree.min_greater_than(4), tree.end());
    ASSERT_TRUE(*tree.begin() == 1);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 4);

    ASSERT_EQUAL(tree.find(1), tree.begin());
    ASSERT_EQUAL(tree.find(5), tree.end());
    ASSERT_EQUAL(*tree.find(3), 3);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "1 2 3 4 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "1 2 3 4 ");

    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << tree.to_string() << endl;
    cout << tree << endl;

    ASSERT_EQUAL(*(++tree.find(1)), 2);
    ASSERT_EQUAL(*(++tree.find(2)), 3);
    ASSERT_EQUAL(*(++tree.find(3)), 4);
    ASSERT_TRUE(++tree.find(4) == tree.end());
}

TEST(empty_tree) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.height() == 0);
    ASSERT_EQUAL(tree.min_greater_than(1), tree.end());
    ASSERT_EQUAL(tree.begin(), tree.end());
    ASSERT_EQUAL(tree.min_element(), tree.end());
    ASSERT_EQUAL(tree.max_element(), tree.end());
    ASSERT_EQUAL(tree.find(1), tree.end());

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "");

    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << tree.to_string() << endl;
    cout << tree << endl;
}

TEST(big_three) {
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter = tree.begin();

    ASSERT_TRUE(iter == tree.end());

    tree.insert(9);
    iter = tree.begin();
    ASSERT_TRUE(iter != tree.end());
    ASSERT_TRUE(*iter == 9);

    BinarySearchTree<int> tree2;
    
    tree2.insert(4);
    tree2.insert(2);
    tree2.insert(5);
    tree2.insert(1);
    tree2.insert(3);
    tree2.insert(6);

    tree = tree2;

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree.size() == 6); 
    ASSERT_TRUE(tree.height() == 3);
    ASSERT_TRUE(*tree.begin() == 1);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 6);

    BinarySearchTree<int>::Iterator it = tree.find(6);
    BinarySearchTree<int>::Iterator it2 = tree2.find(6);
    *it2 = 7;
    ASSERT_EQUAL(*it2, 7);
    ASSERT_EQUAL(*it, 6);

    *it = 8;
    ASSERT_EQUAL(*it2, 7);
    ASSERT_EQUAL(*it, 8);

    BinarySearchTree<int> empty_tree;
    tree = empty_tree;

    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.begin() == tree.end());
    ASSERT_TRUE(tree.check_sorting_invariant());
}

// change something in original tree and check that the second tree is unchanged 

TEST(copy_constructor)
{
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    tree.insert(4);
    tree.insert(2);
    tree.insert(5);
    tree.insert(1);
    tree.insert(3);
    tree.insert(6);
    
    const BinarySearchTree<int> tree2 = tree;
    BinarySearchTree<int> tree3(tree2);

    ASSERT_FALSE(tree3.empty());
    ASSERT_TRUE(tree3.check_sorting_invariant());
    ASSERT_TRUE(tree3.size() == 6); 
    ASSERT_TRUE(tree3.height() == 3);

    ASSERT_TRUE(*tree3.begin() == 1);

    ASSERT_TRUE(*tree3.min_element() == 1);
    ASSERT_TRUE(*tree3.max_element() == 6);

    ASSERT_TRUE(*tree.min_greater_than(0) == 1);
    ASSERT_TRUE(*tree3.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree3.min_greater_than(3) == 4);
    ASSERT_EQUAL(tree3.min_greater_than(6), tree.end());  
    
    ASSERT_EQUAL(tree3.find(7), tree.end());
    ASSERT_EQUAL(tree3.find(0), tree.end());
    ASSERT_EQUAL(*tree3.find(1), 1);
    ASSERT_EQUAL(*tree3.find(1), *tree.begin());
    ASSERT_TRUE(tree3.find(4) != tree.end());
    ASSERT_TRUE(tree3.find(4) != tree.end());
    ASSERT_TRUE(tree3.find(3) != tree.end()); 

    tree3.insert(7);

    ASSERT_TRUE(tree3.size() == 7);
    ASSERT_TRUE(tree3.height() == 4);
    ASSERT_TRUE(tree3.check_sorting_invariant());
    ASSERT_TRUE(*tree3.max_element() == 7);
    ASSERT_TRUE(tree3.find(7) != tree.end()); 

    cout << tree3.to_string() << endl;
    cout << tree3 << endl;

    //testing iterator operator overloads
    ASSERT_EQUAL(*(++tree3.find(1)), 2);
    ASSERT_EQUAL(*(++tree3.find(2)), 3);
    ASSERT_EQUAL(*(++tree3.find(3)), 4);
    ASSERT_EQUAL(*(++tree3.find(4)), 5);
    ASSERT_EQUAL(*(++tree3.find(5)), 6);
    ASSERT_EQUAL(*(++tree3.find(6)), 7);
    ASSERT_TRUE(++tree3.find(7) == tree3.end());

    ostringstream oss_preorder;
    tree3.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "4 2 1 3 5 6 7 ");

    ostringstream oss_inorder;
    tree3.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "1 2 3 4 5 6 7 ");

    //destructor called automatically
}

TEST(random_vals) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());

    tree.insert(2);
    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(6);
    tree.insert(7);
    tree.insert(3);
    tree.insert(1);
    tree.insert(0);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 9);
    ASSERT_TRUE(tree.height() == 5);
    ASSERT_TRUE(*tree.min_greater_than(0) == 1);
    ASSERT_TRUE(*tree.min_greater_than(2) == 3);
    ASSERT_TRUE(*tree.min_greater_than(7) == 9);
    ASSERT_EQUAL(tree.min_greater_than(10), tree.end());
    ASSERT_TRUE(*tree.begin() == 0);
    ASSERT_TRUE(*tree.min_element() == 0);
    ASSERT_TRUE(*tree.max_element() == 10);

    ASSERT_EQUAL(tree.find(0), tree.begin());
    ASSERT_EQUAL(tree.find(5), tree.end());
    ASSERT_EQUAL(*tree.find(3), 3);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "2 1 0 9 4 3 6 7 10 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "0 1 2 3 4 6 7 9 10 ");

    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << tree.to_string() << endl;
    cout << tree << endl;

    ASSERT_EQUAL(*(++tree.find(0)), 1);
    ASSERT_EQUAL(*(++tree.find(1)), 2);
    ASSERT_EQUAL(*(++tree.find(2)), 3);
    ASSERT_EQUAL(*(++tree.find(3)), 4);
    ASSERT_EQUAL(*(++tree.find(4)), 6);
    ASSERT_EQUAL(*(++tree.find(6)), 7);
    ASSERT_EQUAL(*(++tree.find(7)), 9);
    ASSERT_EQUAL(*(++tree.find(9)), 10);
    ASSERT_TRUE(++tree.find(10) == tree.end());
}

TEST(right_child_only) {
    BinarySearchTree<int> tree;
    tree.insert(2);
    tree.insert(5);
    tree.insert(4);
    tree.insert(7);

    ASSERT_TRUE(*tree.min_greater_than(0) == 2);
    ASSERT_TRUE(*tree.min_greater_than(4) == 5);
    ASSERT_EQUAL(tree.min_greater_than(7), tree.end());
    ASSERT_TRUE(*tree.begin() == 2);
    ASSERT_TRUE(*tree.min_element() == 2);
    ASSERT_TRUE(*tree.max_element() == 7);

    ASSERT_EQUAL(tree.find(2), tree.begin());
    ASSERT_EQUAL(tree.find(3), tree.end());

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "2 5 4 7 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "2 4 5 7 ");

    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << tree.to_string() << endl;
    cout << tree << endl;
}

TEST(left_child_only) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(1);
    tree.insert(0);
    tree.insert(2);
    tree.insert(3);

    ASSERT_EQUAL(tree.min_greater_than(5), tree.end());
    ASSERT_TRUE(*tree.begin() == 0);
    ASSERT_TRUE(*tree.min_element() == 0);
    ASSERT_TRUE(*tree.max_element() == 5);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_EQUAL(oss_preorder.str(), "5 1 0 2 3 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), "0 1 2 3 5 ");

    ASSERT_TRUE(tree.check_sorting_invariant());

    cout << tree.to_string() << endl;
    cout << tree << endl;

    ASSERT_EQUAL(*(++tree.find(3)), 5);
    ASSERT_TRUE(++tree.find(5) == tree.end());
}

TEST(break_invariant) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(1);
    tree.insert(9);

    BinarySearchTree<int>::Iterator it = tree.begin();
    BinarySearchTree<int>::Iterator it_root = tree.find(5);
    
    *(tree.find(1)) = 7;

    ASSERT_FALSE(tree.check_sorting_invariant());
    ASSERT_TRUE(*it == 7);

    *(tree.find(5)) = 8;
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*it_root == 8);
}

// create a custom class and comparator

TEST_MAIN()