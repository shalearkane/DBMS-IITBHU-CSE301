#include <bits/stdc++.h>
#include <cassert>
#include <cstddef>
#include <string>
using namespace std;

#define DEBUG 1
int call_depth = 0;
int print_depth = 0;

#define ORDER 2
#define INITIAL_INDEX 10

void dbg_print(string cls, string func, string msg = "") {
    if (!DEBUG)
        return;
    for (int i = 0; i < call_depth - 1; i++) {
        cerr << "  ";
    }
    if (call_depth - 1)
        cerr << "└─";

    cerr << cls << '\t' << func;
    if (msg != "")
        cerr << " : " << msg;
    cerr << '\n';
}
class BPTreeNode {
    string cls = "Node";
    int _fill_count = 0;
    int indexes[ORDER];

  public:
    BPTreeNode(BPTreeNode *parent);
    BPTreeNode(const int index);

    BPTreeNode *child_ptr[ORDER + 1];
    BPTreeNode *parent_ptr = NULL;
    bool is_root;
    bool is_leaf;
    void traverse(BPTreeNode *tn);
    int fill_count() {
        assert(_fill_count <= ORDER);
        return _fill_count;
    };
    int is_filled() { return !(_fill_count < ORDER); };
    bool insert(int key) {
        if (this->_fill_count >= ORDER)
            return false;
        else {
            this->indexes[this->_fill_count] = key;
            this->_fill_count++;
            return true;
        }
    }

    void clear_indexes() {
        assert(this->is_leaf);
        for (int i = 0; i < ORDER; i++)
            indexes[i] = -1;
        this->_fill_count = 0;
    }

    void clear_indexes_of_parent(bool is_parent) {
        assert(is_parent);
        for (int i = 0; i < ORDER; i++)
            indexes[i] = -1;
        this->_fill_count = 0;
    }

    void shiftAfterIndexAndInsertChild(const int index, BPTreeNode *child) {
        // DEBUG
        call_depth++;
        const string msg = "index = " + to_string(index) +
                           ", value = " + to_string(child->getIndexes()[index]);
        dbg_print(this->cls, "shiftAfterIndexAndInsertChild", msg);
        // DEBUG

        assert(this->_fill_count <= ORDER - 1);

        for (int i = ORDER - 1; i > index; i--) {
            this->indexes[i] = this->indexes[i - 1];
            this->child_ptr[i + 1] = this->child_ptr[i];
        }
        this->indexes[index] = child->getIndexes()[0];
        this->child_ptr[index + 1] = child;

        this->_fill_count++;

        call_depth--;
    }

    int *getIndexes() { return indexes; };
};

class BPTree {
    string cls = "Tree";
    BPTreeNode *root;
    void insertIntoChildren(BPTreeNode *n, const int index, const int key);
    bool insertIntoLeaf(BPTreeNode *n, const int key);
    bool checkSplitSuccess(BPTreeNode *parent);
    void splitRecursionUpLeaf(BPTreeNode *parent, const int child_index,
                              const int additional);
    void splitRecursionUpInternalNode(BPTreeNode *parent,
                                      BPTreeNode *child_node);
    int getInsertPosition(BPTreeNode *n, const int key);

  public:
    BPTree();
    void print() { this->root->traverse(root); };
    void insert(int key);
};

BPTreeNode::BPTreeNode(BPTreeNode *parent) {
    assert(ORDER > 1);
    assert(parent != NULL);

    this->is_leaf = true;

    for (int i = 0; i < ORDER; i++) {
        this->indexes[i] = -1;
    }

    for (int i = 0; i < ORDER + 1; i++) {
        this->child_ptr[i] = NULL;
    }
}

BPTreeNode::BPTreeNode(const int index) {
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

int BPTree::getInsertPosition(BPTreeNode* n,const int key){
    int less_than = n->fill_count();

    // check where to insert
    for (int i = n->fill_count() - 1; i >= 0; i--) {
        cout << i << less_than;
        if (key < this->root->getIndexes()[i]) {
            less_than = i;
        } else {
            break;
        }
    }

    return less_than;
}

void BPTree::insert(const int key) {
    // DEBUG
    call_depth++;
    const string msg = "key = " + to_string(key);
    dbg_print(this->cls, "insert", msg);
    // DEBUG

    assert(key > 0);

    BPTreeNode *x = root;
    assert(x != NULL);
    // root node is definitely not leaf

    const int insertPosition = getInsertPosition(x, key);

    insertIntoChildren(this->root, insertPosition, key);

    call_depth--;
}

void BPTree::insertIntoChildren(BPTreeNode *n, const int index, const int key) {
    // DEBUG
    call_depth++;
    const string msg =
        "index = " + to_string(index) + " key = " + to_string(key);
    dbg_print(this->cls, "insertIntoChildren", msg);
    // DEBUG

    // if child pointer is null, create a new leaf
    if (n->child_ptr[index] == NULL) {
        n->child_ptr[index] = new BPTreeNode(n);
    }

    // if it is a leaf index, insert directly
    if (n->child_ptr[index]->is_leaf) {
        if (!insertIntoLeaf(n->child_ptr[index], key)) {
            // strategy strategy
            // strategy 1:
            // split the child node and try add a index to parent
            // if parent overflows, split parent and add index to its parent
            // recursively
            // strategy 2:
            // create a new level

            // check if first strategy will work
            if (checkSplitSuccess(n)) {
                splitRecursionUpLeaf(n, index, key);
            }
        }
    } else {
        // not a leaf index, then find where to insert
        const int insertPosition = getInsertPosition(n->child_ptr[index], key);
        insertIntoChildren(n->child_ptr[index], insertPosition, key);
    }

    call_depth--;
}

void BPTree::splitRecursionUpLeaf(BPTreeNode *parent,const int child_index,
                                  const int additional) {

    // DEBUG
    call_depth++;
    const string msg = "index = " + to_string(child_index) +
                       " additional = " + to_string(additional);
    dbg_print(this->cls, "splitRecursionUpLeaf", msg);
    // DEBUG

    BPTreeNode *new_node = new BPTreeNode(parent);

    vector<int> v;
    for (int i = 0; i < parent->child_ptr[child_index]->fill_count(); i++)
        v.push_back(parent->child_ptr[child_index]->getIndexes()[i]);

    parent->child_ptr[child_index]->clear_indexes();

    v.push_back(additional);
    sort(v.begin(), v.end());

    // strictly for leaf nodes
    int i = 0;
    for (; i < (v.size() / 2); i++) {
        parent->child_ptr[child_index]->insert(v[i]);
    }

    for (; i < v.size(); i++) {
        new_node->insert(v[i]);
    }

    splitRecursionUpInternalNode(parent, new_node);

    call_depth--;
}

void BPTree::splitRecursionUpInternalNode(BPTreeNode *parent,
                                          BPTreeNode *child_node) {
    // DEBUG
    call_depth++;
    const string msg =
        "parent = " + to_string(parent->getIndexes()[0]) + "-" +
        to_string(parent->getIndexes()[parent->fill_count() - 1]) +
        ", child_node = " + to_string(child_node->getIndexes()[0]) + "-" +
        to_string(child_node->getIndexes()[child_node->fill_count() - 1]);
    dbg_print(this->cls, "SplitRecursionUpInternalNode", msg);
    // DEBUG

    if (parent->is_filled()) {
        // split parent node
        BPTreeNode *gran_parent = parent->parent_ptr;
        BPTreeNode *sibling = new BPTreeNode(gran_parent->parent_ptr);

        vector<int> v(*parent->getIndexes());
        assert(v.size() == ORDER);

        parent->clear_indexes_of_parent(!(parent->is_leaf));

        // strictly for indexes within parent nodes
        int i = 0;
        for (; i < (v.size() / 2); i++) {
            parent->insert(v[i]);
        }

        // sibling gets least right side child_ptrs
        for (; i < v.size(); i++) {
            sibling->child_ptr[sibling->fill_count() + 1] =
                parent->child_ptr[i + 1];
            parent->child_ptr[i + 1] = NULL;

            sibling->insert(v[i]);
        }

        // since number of child nodes is one plus indexes
        sibling->child_ptr[sibling->fill_count()] = parent->child_ptr[i];
        parent->child_ptr[i] = NULL;

        splitRecursionUpInternalNode(gran_parent, sibling);

        call_depth--;
        return;
    }

    int index = child_node->getIndexes()[0];
    int insertPosition = getInsertPosition(parent, index);

    // push the elements after insertPosition
    parent->shiftAfterIndexAndInsertChild(insertPosition, child_node);

    call_depth--;
}

bool BPTree::checkSplitSuccess(BPTreeNode *parent) {
    // DEBUG
    call_depth++;
    dbg_print(this->cls, "checkSplitSuccess");
    // DEBUG

    if (parent == NULL)
        return false;
    if (!parent->is_filled())
        return true;
    return (parent->parent_ptr);

    call_depth--;
}

bool BPTree::insertIntoLeaf(BPTreeNode *n, const int key) {
    // DEBUG
    call_depth++;
    const string msg = "key = " + to_string(key);
    dbg_print(this->cls, "insertIntoLeaf", msg);
    // DEBUG

    assert(n != NULL);
    assert(n->is_leaf);

    call_depth--;
    return n->insert(key);
}

void BPTreeNode::traverse(BPTreeNode *n) {
    print_depth++;
    if (n == NULL) {
        print_depth--;
        return;
    }

    for (int i = 0; i < print_depth; i++)
        cout << "-";
    cout << "Indexes : ";

    for (int i = 0; i < ORDER; i++) {
        cout << n->getIndexes()[i] << ", ";
    }

    cout << '\n';
    for (int i = 0; i < ORDER + 1; i++) {
        traverse(n->child_ptr[i]);
    }

    print_depth--;
}

void menu() {
    cout << "Command       \tDescription\n"
         << "---------------------------\n"
         << "i <key>       \tinsert\n"
         << "d <key>       \tdelete\n"
         << "p             \tprint\n"
         << "q             \tquit\n";
}

int main() {
    cout << "B+ Tree\n";

    BPTree *bpt = new BPTree();
    menu();
    do {
        string choice;
        int key;
        string value;
        cout << "> ";
        cin >> choice;
        if (choice == "i") {
            cin >> key;
            bpt->insert(key);
            bpt->print();
        } else if (choice == "p") {
            bpt->print();
        } else if (choice == "d") {
            cin >> key;
            // d.remove(key);
        } else if (choice != "q") {
            cout << "\nInvalid option '" << choice << "'\n\n";
            menu();
        } else {
            break;
        }

    } while (true);
    cout << endl;
    return 0;
}