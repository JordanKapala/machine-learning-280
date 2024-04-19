#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <ostream>

TEST(test_constructor) {
    // create a tree with default constructor
    BinarySearchTree<int> int_tree;
}

TEST(test_copy_constructor) {
    // create a tree
    BinarySearchTree<int> tree;
    int values[] = {5, 3, 7, 2, 4, 6, 8};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 3);

    // create a tree from int tree
    BinarySearchTree<int> new_tree(tree);

    ASSERT_FALSE(new_tree.empty());
    ASSERT_TRUE(new_tree.size() == 7);
    ASSERT_TRUE(new_tree.height() == 3);

    // modify new tree
    new_tree.insert(9);

    // test that tree and new tree are correct
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 3);

    ASSERT_FALSE(new_tree.empty());
    ASSERT_TRUE(new_tree.size() == 8);
    ASSERT_TRUE(new_tree.height() == 4);
}

TEST(test_assignment) {
    // create a tree
    BinarySearchTree<int> tree;
    int values[] = {5, 3, 7, 2, 4, 6, 8};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 3);

    // create a tree from int tree
    BinarySearchTree<int> new_tree;

    // insert values into new tree
    for (int value : values) {
        new_tree.insert(value);
    }

    // use assignment operator
    new_tree = tree;

    ASSERT_FALSE(new_tree.empty());
    ASSERT_TRUE(new_tree.size() == 7);
    ASSERT_TRUE(new_tree.height() == 3);

    // modify new tree
    new_tree.insert(9);

    // test that tree and new tree are correct
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 3);

    ASSERT_FALSE(new_tree.empty());
    ASSERT_TRUE(new_tree.size() == 8);
    ASSERT_TRUE(new_tree.height() == 4);

    BinarySearchTree<int> new_tree2;

    new_tree2 = new_tree;

    ASSERT_FALSE(new_tree2.empty());
    ASSERT_TRUE(new_tree2.size() == 8);
    ASSERT_TRUE(new_tree2.height() == 4);
    ASSERT_TRUE(new_tree2.check_sorting_invariant());

    BinarySearchTree<int> empty;

    new_tree2 = empty;
    ASSERT_TRUE(new_tree2.empty());
    ASSERT_TRUE(new_tree2.size() == 0);
    ASSERT_TRUE(new_tree2.height() == 0);
    ASSERT_TRUE(new_tree2.check_sorting_invariant());
}

TEST(test_empty) {
    // create tree
    BinarySearchTree<int> tree;

    // test empty
    ASSERT_TRUE(tree.empty());

    // insert value
    tree.insert(1);

    // test not empty
    ASSERT_FALSE(tree.empty());
}

TEST(test_height) {
    // create a tree
    BinarySearchTree<int> tree;

    // insert value
    tree.insert(1);
    ASSERT_TRUE(tree.height() == 1);

    // insert value
    tree.insert(2);
    ASSERT_TRUE(tree.height() == 2);

    // insert value
    BinarySearchTree<int>::Iterator iter = tree.insert(3);
    ASSERT_TRUE(tree.height() == 3);

    *iter = -1;
    ASSERT_TRUE(tree.height() == 3 );
    ASSERT_FALSE(tree.check_sorting_invariant());

    tree.insert(-10);
    tree.insert(-5);
    tree.insert(-9);
    tree.insert(-8);
    ASSERT_TRUE(tree.height() == 5);
}

TEST(test_size) {
    // create a tree
    BinarySearchTree<int> tree;

    // insert value
    tree.insert(1);
    ASSERT_TRUE(tree.size() == 1);

    // insert value
    tree.insert(2);
    ASSERT_TRUE(tree.size() == 2);

    // insert value
    BinarySearchTree<int>::Iterator iter = tree.insert(3);
    ASSERT_TRUE(tree.size() == 3);

    *iter = -1;
    ASSERT_TRUE(tree.size() == 3);
}

TEST(test_traverse_inorder) {
    // create a tree
    BinarySearchTree<int> tree;
    int values[] = {5, 3, 7, 2, 4, 6, 8};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    // create stream object to compare
    std::ostringstream output;

    // get output from inorder function
    tree.traverse_inorder(output);

    // create string for comparison
    std::string output_str = output.str();
    std::string expected_output = "2 3 4 5 6 7 8 ";

    // compare output
    ASSERT_TRUE(output_str == expected_output);
}

TEST(test_traverse_preorder) {
    // create a tree
    BinarySearchTree<int> tree;
    int values[] = {5, 3, 7, 2, 4, 6, 8};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    // create stream object to compare
    std::ostringstream output;

    // get output from inorder function
    tree.traverse_preorder(output);

    // create string for comparison
    std::string output_str = output.str();
    std::string expected_output = "5 3 2 4 7 6 8 ";

    // compare output
    ASSERT_TRUE(output_str == expected_output);
}

TEST(test_sorting_invariant) {
    // create a tree
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter;
    int values[] = {5, 3, 7, 2, 4, 6};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
        ASSERT_TRUE(tree.check_sorting_invariant());
    }

    // insert value into tree
    iter = tree.insert(8);

    // test sorting invariant
    ASSERT_TRUE(tree.check_sorting_invariant());

    // modify element to break invariant
    *iter = 1;

    // test sorting invariant
    ASSERT_FALSE(tree.check_sorting_invariant());

    *iter = 8;
    ASSERT_TRUE(tree.check_sorting_invariant());

    BinarySearchTree<int> tree2;
    ASSERT_TRUE(tree2.check_sorting_invariant());

    tree2.insert(-10);
    ASSERT_TRUE(tree2.check_sorting_invariant());

    tree2.insert(-30);
    tree2.insert(-9);
    ASSERT_TRUE(tree2.check_sorting_invariant());

}

TEST(test_min_element) {
    // create a tree
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter;
    int values[] = {5, 3, 7, 4, 6, 8};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    // create iterator to min element
    iter = tree.min_element();
    ASSERT_TRUE(*iter == 3);

    // insert new min element
    tree.insert(2);

    // test min element
    iter = tree.min_element();
    ASSERT_TRUE(*iter == 2);
}

TEST(test_max_element) {
    // create a tree
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter;
    BinarySearchTree<int>::Iterator null_iter;
    int values[] = {5, 3, 7, 2, 4, 6};

    iter = tree.max_element();
    ASSERT_TRUE(iter == null_iter);
    iter = tree.min_element();
    ASSERT_TRUE(iter == null_iter);

    iter = tree.insert(1);
    ASSERT_TRUE(tree.min_element() == tree.max_element());
    iter = tree.min_element();
    ASSERT_TRUE(*iter == 1);

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    // create iterator to min element
    iter = tree.max_element();
    ASSERT_TRUE(*iter == 7);

    // insert new min element
    tree.insert(8);

    // test min element
    iter = tree.max_element();
    ASSERT_TRUE(*iter == 8);
}

TEST(test_min_greater_than) {
    // create a tree
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter;
    int values[] = {5, 3, 7, 2, 6, 8};

    // insert values into tree
    for (int value : values) {
        tree.insert(value);
    }

    // test min greater than lowest
    iter = tree.min_greater_than(1);
    ASSERT_TRUE(*iter == 2);

    // test min greater than highest
    iter = tree.min_greater_than(7);
    ASSERT_TRUE(*iter == 8);

    // test min greater than traverse height
    iter = tree.min_greater_than(5);
    ASSERT_TRUE(*iter == 6);

    // test min greater than
    iter = tree.min_greater_than(3);
    ASSERT_TRUE(*iter == 5);

    // insert 4
    tree.insert(4);

    // test min greater than
    iter = tree.min_greater_than(3);
    ASSERT_TRUE(*iter == 4);

    tree.insert(-1);

    iter = tree.min_greater_than(0);
    ASSERT_TRUE(*iter == 2);
    iter = tree.min_greater_than(-1);
    ASSERT_TRUE(*iter == 2);
    iter = tree.min_greater_than(-2);
    ASSERT_TRUE(*iter == -1);
}

TEST(test_insert) {
    // create tree
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter;

    // insert first value
    iter = tree.insert(5);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(*iter == 5);
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    ASSERT_TRUE(tree.check_sorting_invariant());

    // insert value to left
    iter = tree.insert(3);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(*iter == 3);
    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.height() == 2);
    ASSERT_TRUE(tree.check_sorting_invariant());

    // insert value to right
    iter = tree.insert(7);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(*iter == 7);
    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(tree.height() == 2);
    ASSERT_TRUE(tree.check_sorting_invariant());

    // insert value to left
    iter = tree.insert(2);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(*iter == 2);
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 3);
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_find) {
    // create tree
    BinarySearchTree<int> tree;
    BinarySearchTree<int>::Iterator iter;
    BinarySearchTree<int>::Iterator null_iter;
    int values[] = {5, 3, 7, 2, 4, 6, 8};

    // insert values
    for (int value : values) {
        tree.insert(value);
        ASSERT_TRUE(tree.check_sorting_invariant());
    }

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 3);

    // test find
    iter = tree.find(7);
    ASSERT_TRUE(*iter == 7);

    iter = tree.find(0);
    ASSERT_TRUE(iter == null_iter);

    iter = tree.find(7);
    *iter = 0;
    ASSERT_TRUE(*iter == 0);

    iter = tree.find(7);
    ASSERT_TRUE(iter == null_iter);

    iter = tree.find(-5);
    ASSERT_TRUE(iter == null_iter);
}

TEST(bst_public_test) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  std::cout << "cout << tree.to_string()" << std::endl;
  std::cout << tree.to_string() << std::endl << std::endl;

  std::cout << "cout << tree" << std::endl << "(uses iterators)" << std::endl;
  std::cout << tree << std::endl << std::endl;

  std::ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  std::cout << "preorder" << std::endl;
  std::cout << oss_preorder.str() << std::endl << std::endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  std::ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  std::cout << "inorder" << std::endl;
  std::cout << oss_inorder.str() << std::endl << std::endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST_MAIN()