#include <bits/stdc++.h>
#include <cassert>
#include <cstddef>
using namespace std;
#define deb 1

#define ORDER 2
#define INITIAL_INDEX 10

class BPTreeNode {
    int _fill_count = 0;
    int indexes[ORDER];

  public:
    BPTreeNode();
    BPTreeNode(int index);

    BPTreeNode *child_ptr[ORDER + 1];
    BPTreeNode *parent_ptr = NULL;
    bool is_root;
    bool is_leaf;
    void traverse(BPTreeNode *tn);
    int fill_count() { return _fill_count; };
    int is_filled() { return _fill_count < ORDER; };
    bool insert(int key) {
        if (this->_fill_count >= ORDER)
            return false;
        else {
            this->indexes[this->_fill_count] = key;
            this->_fill_count++;
            return true;
        }
    }

    int *getIndexes() { return indexes; };
};

class BPTree {
    int n;
    BPTreeNode *root;
    BPTree();
    void insert(BPTreeNode *n, int index, int key);
    bool insertIntoLeaf(BPTreeNode *n, int key);

  public:
    void insert(int key);
};

BPTreeNode::BPTreeNode() {
    assert(ORDER > 1);

    this->is_leaf = true;

    for (int i = 0; i < ORDER; i++) {
        this->indexes[i] = -1;
    }

    for (int i = 0; i < ORDER + 1; i++) {
        this->child_ptr[i] = NULL;
    }
}

BPTreeNode::BPTreeNode(int index) {
    // this set the initial index
    // not for leaf nodes
    assert(ORDER > 1);
    assert(index > 0);
    this->is_leaf = false;

    // if index is -1, then it means it is not populated yet
    for (int i = 1; i < ORDER; i++) {
        this->indexes[i] = -1;
    }
    this->indexes[0] = index;
    this->_fill_count = 1;

    for (int i = 0; i < ORDER + 1; i++) {
        this->child_ptr[i] = NULL;
    }
}

BPTree::BPTree() {
    this->root = new BPTreeNode(INITIAL_INDEX);
    this->root->is_root = true;
}

void BPTree::insert(int key) {
    assert(key > 0);

    BPTreeNode *x = root;
    assert(x != NULL);
    // root node is definitely not leaf
    int less_than = 0;
    for (; less_than < this->root->fill_count(); less_than++) {
        if (key < this->root->getIndexes()[less_than])
            continue;
        else
            break;
    }
    insert(this->root, less_than, key);
}

void BPTree::insert(BPTreeNode *n, int index, int key) {
    // if child index is null, create a new leaf
    if (n->child_ptr[index] == NULL) {
        n->child_ptr[index] = new BPTreeNode();
        n->child_ptr[index]->is_leaf = true;
    }
}

bool BPTree::insertIntoLeaf(BPTreeNode *n, int key) {
    assert(n != NULL);
    n->insert(key);
}

void BPTreeNode::traverse(BPTreeNode *tn) {
    for (int i = 0; i < tn->_fill_count; i++) {
        if (!tn->is_leaf) {
            traverse(tn->child_ptr[i]);
        }

        cout << tn->indexes[i];
    }

    if (!tn->is_leaf) {
        traverse(tn->child_ptr[tn->_fill_count]);
    }

    cout << '\n';
}

int main() { return 0; }