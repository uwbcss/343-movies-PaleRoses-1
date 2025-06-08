/**
 * @file bstree.h
 * @location header/bstree.h
 * @brief Binary Search Tree for sorted movie storage
 */

#ifndef BSTREE_H
#define BSTREE_H

#include <functional>
#include <iostream>

// BST for maintaining sorted collections
template <typename T> class BSTree {
  private:
    struct Node {
        T data;
        Node *left;
        Node *right;

        explicit Node(const T &item)
            : data(item), left(nullptr), right(nullptr) {}
    };

    Node *root;
    size_t nodeCount;

    // Recursive insertion
    Node *insertHelper(Node *node, const T &item,
                       std::function<bool(const T &, const T &)> compare) {
        if (node == nullptr) {
            nodeCount++;
            return new Node(item);
        }

        if (compare(item, node->data)) {
            node->left = insertHelper(node->left, item, compare);
        } else {
            node->right = insertHelper(node->right, item, compare);
        }

        return node;
    }

    // Find by key using extractor function
    template <typename K>
    T *findHelper(Node *node, const K &key,
                  std::function<K(const T &)> keyExtractor) const {
        if (node == nullptr) {
            return nullptr;
        }

        K nodeKey = keyExtractor(node->data);
        if (key == nodeKey) {
            return &(node->data);
        } else if (key < nodeKey) {
            return findHelper(node->left, key, keyExtractor);
        } else {
            return findHelper(node->right, key, keyExtractor);
        }
    }

    // In-order traversal
    void inOrderHelper(Node *node, std::function<void(const T &)> visit) const {
        if (node != nullptr) {
            inOrderHelper(node->left, visit);
            visit(node->data);
            inOrderHelper(node->right, visit);
        }
    }

    // Delete all nodes
    void deleteTree(Node *node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    // Find by custom predicate (for non-BST ordered searches)
    T *findByPredicateHelper(Node *node,
                             std::function<bool(const T &)> predicate) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (predicate(node->data)) {
            return &(node->data);
        }

        T *leftResult = findByPredicateHelper(node->left, predicate);
        if (leftResult != nullptr) {
            return leftResult;
        }

        return findByPredicateHelper(node->right, predicate);
    }

  public:
    BSTree() : root(nullptr), nodeCount(0) {}
    ~BSTree() { deleteTree(root); }

    // No copying
    BSTree(const BSTree &) = delete;
    BSTree &operator=(const BSTree &) = delete;

    // Insert with comparison function
    void insert(const T &item,
                std::function<bool(const T &, const T &)> compare) {
        root = insertHelper(root, item, compare);
    }

    // Find by key using extractor
    template <typename K>
    T *find(const K &key, std::function<K(const T &)> keyExtractor) const {
        return findHelper(root, key, keyExtractor);
    }

    // Find using custom predicate
    T *findByPredicate(std::function<bool(const T &)> predicate) const {
        return findByPredicateHelper(root, predicate);
    }

    // Visit all elements in sorted order
    void inOrderTraversal(std::function<void(const T &)> visit) const {
        inOrderHelper(root, visit);
    }

    bool empty() const { return root == nullptr; }
    size_t size() const { return nodeCount; }
};

#endif // BSTREE_H