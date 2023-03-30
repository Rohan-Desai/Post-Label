// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

TEST(test_stub) {
    // Add your tests here
    ASSERT_TRUE(true);
}
TEST(test_empty) {
    BinarySearchTree<int> b;
    ASSERT_TRUE(b.empty());
    b.insert(1);
    b.insert(2);
    b.insert(3);
    ASSERT_FALSE(b.empty());
}

TEST(test_size_height) {
    BinarySearchTree<int> b;
    ASSERT_EQUAL(b.size(), 0);
    ASSERT_EQUAL(b.height(), 0);
    b.insert(5);
    b.insert(4);
    b.insert(6);
    b.insert(3);
    b.insert(7);
    b.insert(2);
    b.insert(8);
    b.insert(1);
    b.insert(9);
    ASSERT_EQUAL(b.size(), 9);
    ASSERT_EQUAL(b.height(), 5);
    b.insert(10);
    b.insert(0);
    ASSERT_EQUAL(b.size(), 11);
    ASSERT_EQUAL(b.height(), 6);
}

TEST(test_copy) {
    BinarySearchTree<int> b;
    b.insert(100);
    b.insert(150);
    b.insert(50);
    b.insert(125);
    b.insert(75);
    b.insert(135);
    b.insert(65);

    BinarySearchTree<int> c (b);
    BinarySearchTree<int>::Iterator cit = c.begin();

    for(BinarySearchTree<int>::Iterator bit = b.begin(); bit != b.end(); ++bit){
        ASSERT_TRUE(*bit == *cit);
        ++cit;
    }

    BinarySearchTree<int> d = b;
    BinarySearchTree<int>::Iterator dit = d.begin();

    for(BinarySearchTree<int>::Iterator bit = b.begin(); bit != b.end(); ++bit){
        ASSERT_TRUE(*bit == *dit);
        ++dit;
    }

}

TEST(test_find){
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(5);
    b.insert(15);
    b.insert(12);
    b.insert(6);
    b.insert(2);
    ASSERT_TRUE(*b.find(2) == 2);
    ASSERT_TRUE(*b.find(10) == 10);
    ASSERT_TRUE(*b.find(12) == 12);
    ASSERT_TRUE(b.find(100) == b.end());
}

TEST(test_min_max_element){
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(5);
    b.insert(15);
    b.insert(12);
    b.insert(6);
    b.insert(2);
    ASSERT_TRUE(*b.min_element() == 2);
    ASSERT_TRUE(*b.max_element() == 15);
    b.insert(1);
    ASSERT_TRUE(*b.min_element() == 1);
    ASSERT_TRUE(*b.max_element() == 15);
    b.insert(100);
    ASSERT_TRUE(*b.min_element() == 1);
    ASSERT_TRUE(*b.max_element() == 100);

}

TEST(test_check_sorting_invariant){
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(5);
    b.insert(15);
    b.insert(12);
    b.insert(6);
    b.insert(2);
    b.insert(100);
    ASSERT_TRUE(b.check_sorting_invariant());
    *b.begin() = 25;
    ASSERT_FALSE(b.check_sorting_invariant());
}

TEST(test_inorder_traversal){
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(5);
    b.insert(15);
    b.insert(12);
    b.insert(6);
    b.insert(2);
    b.insert(100);
    ostringstream oss_inorder;
    ostringstream oss_inorders;
    b.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "2 5 6 10 12 15 100 ");
    b.insert(8);
    b.insert(101);
    b.traverse_inorder(oss_inorders);
    ASSERT_TRUE(oss_inorders.str() == "2 5 6 8 10 12 15 100 101 ");
}

TEST(test_preorder_traversal){
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(2);
    b.insert(5);
    b.insert(12);
    b.insert(15);
    b.insert(6);
    b.insert(100);
    ostringstream oss_preorder;
    ostringstream oss_preorders;
    b.traverse_preorder(oss_preorder);
    ASSERT_TRUE(oss_preorder.str() == "10 2 5 6 12 15 100 ");
    b.insert(8);
    b.insert(101);
    b.traverse_preorder(oss_preorders);
    ASSERT_TRUE(oss_preorders.str() == "10 2 5 6 8 12 15 100 101 ");
}

TEST(test_min_greater_than){
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(5);
    b.insert(15);
    b.insert(12);
    b.insert(6);
    b.insert(2);
    b.insert(100);
    ASSERT_TRUE(*b.min_greater_than(9) == 10);
    ASSERT_TRUE(*b.min_greater_than(10) == 12);
    ASSERT_TRUE(*b.min_greater_than(25) == 100);
    ASSERT_TRUE(*b.min_greater_than(0) == 2);
    b.insert(1);
    b.insert(101);
    ASSERT_TRUE(*b.min_greater_than(100) == 101);
    ASSERT_TRUE(*b.min_greater_than(0) == 1);

    BinarySearchTree<int> c;
    c.insert(6);
    c.insert(4);
    c.insert(3);
    c.insert(9);
    c.insert(5);
    c.insert(8);
    c.insert(7);
    ASSERT_TRUE(*c.min_greater_than(1) == 3);
    ASSERT_TRUE(*c.min_greater_than(2) == 3);
    ASSERT_TRUE(*c.min_greater_than(3) == 4);
    ASSERT_TRUE(*c.min_greater_than(4) == 5);
    ASSERT_TRUE(*c.min_greater_than(5) == 6);
    ASSERT_TRUE(*c.min_greater_than(6) == 7);
    ASSERT_TRUE(*c.min_greater_than(7) == 8);
    ASSERT_TRUE(*c.min_greater_than(8) == 9);

    BinarySearchTree<int> d;
    d.insert(8);
    d.insert(5);
    d.insert(6);
    d.insert(7);
    d.insert(10);
    d.insert(9);
    d.insert(11);
    ASSERT_TRUE(*d.min_greater_than(3) == 5);
    ASSERT_TRUE(*d.min_greater_than(4) == 5);
    ASSERT_TRUE(*d.min_greater_than(5) == 6);
    ASSERT_TRUE(*d.min_greater_than(6) == 7);
    ASSERT_TRUE(*d.min_greater_than(7) == 8);
    ASSERT_TRUE(*d.min_greater_than(8) == 9);
    ASSERT_TRUE(*d.min_greater_than(9) == 10);
    ASSERT_TRUE(*d.min_greater_than(10) == 11);
}
TEST_MAIN()
