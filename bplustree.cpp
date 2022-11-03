#include <bits/stdc++.h>
#include <cassert>
#include <cstddef>
using namespace std;
#define deb 1

#define ORDER 2
#define INITIAL_INDEX 10

class BPTreeNode {
    int indexes[ORDER];
    BPTreeNode *child_ptr[ORDER + 1];
    int _fill_count = 0;

  public:
    BPTreeNode();
    BPTreeNode(int index);
    bool is_root;
    bool is_leaf;
    void traverse(BPTreeNode *tn);
    int fill_count() { return _fill_count; };
    int is_filled() { return _fill_count < ORDER; };
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
    assert(ORDER > 1);
    assert(index > 0);
    this->is_leaf = false;

    this->indexes[0] = index;
    for (int i = 0; i < ORDER; i++) {
        this->indexes[i] = -1;
    }

    for (int i = 0; i < ORDER + 1; i++) {
        this->child_ptr[i] = NULL;
    }
}

class BPTree {
    int n;
    BPTreeNode *root;
    BPTree();

  public:
    void insert(int key);
};

BPTree::BPTree() {
    this->root = new BPTreeNode;
    this->root->is_root = true;
    this->root->is_leaf = false;
}

void BPTree::insert(int key) {
    assert(key > 0);

    BPTreeNode *x = root;
    assert(x != NULL);
    // check if root node is leaf
    if (x->is_leaf && !(x->is_filled())) {
    }
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