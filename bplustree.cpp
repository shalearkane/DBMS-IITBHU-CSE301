#include <bits/stdc++.h>
#include <cassert>
#include <cstddef>
using namespace std;
#define deb 1

#define ORDER 2

class BPTreeNode {
    int data[ORDER];
    BPTreeNode *child_ptr[ORDER + 1];
    int fill_count = 0;

  public:
    BPTreeNode();
    bool is_root;
    bool is_leaf;
    void traverse(BPTreeNode *tn);
};

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
    this->root->is_leaf = true;
}

void BPTree::insert(int key) {
    BPTreeNode *x = root;
    assert(x != NULL);
}

BPTreeNode::BPTreeNode() {
    this->is_leaf = true;
    for (int i = 0; i < ORDER + 1; i++) {
        this->child_ptr[i] = NULL;
    }
}

void BPTreeNode::traverse(BPTreeNode *tn) {
    for (int i = 0; i < tn->fill_count; i++) {
        if (!tn->is_leaf) {
            traverse(tn->child_ptr[i]);
        }

        cout << tn->data[i];
    }

    if (!tn->is_leaf) {
        traverse(tn->child_ptr[tn->fill_count]);
    }

    cout << '\n';
}

int main() { return 0; }