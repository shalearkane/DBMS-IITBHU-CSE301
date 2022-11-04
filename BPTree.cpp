#include <iostream>
#include <stdio.h>
using namespace std;

struct BPTreeNode {
    int *indexes;
    BPTreeNode **child_ptr;
    bool leaf;
    int n;
} *root = NULL, *np = NULL, *x = NULL;

BPTreeNode *init() {
    int i;
    np = new BPTreeNode;
    np->indexes = new int[5];
    np->child_ptr = new BPTreeNode *[6];
    np->leaf = true;
    np->n = 0;
    for (i = 0; i < 6; i++) {
        np->child_ptr[i] = NULL;
    }
    return np;
}
void traverse(BPTreeNode *p) {
    cout << endl;
    int i;
    for (i = 0; i < p->n; i++) {
        if (p->leaf == false) {
            traverse(p->child_ptr[i]);
        }
        cout << " " << p->indexes[i];
    }
    if (p->leaf == false) {
        traverse(p->child_ptr[i]);
    }
    cout << endl;
}

void sort(int *p, int n) {
    int i, j, temp;
    for (i = 0; i < n; i++) {
        for (j = i; j <= n; j++) {
            if (p[i] > p[j]) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}
int split_child(BPTreeNode *x, int i) {
    int j, mid;
    BPTreeNode *np1, *np3, *y;
    np3 = init();
    np3->leaf = true;
    if (i == -1) {
        mid = x->indexes[2];
        x->indexes[2] = 0;
        x->n--;
        np1 = init();
        np1->leaf = false;
        x->leaf = true;
        for (j = 3; j < 5; j++) {
            np3->indexes[j - 3] = x->indexes[j];
            np3->child_ptr[j - 3] = x->child_ptr[j];
            np3->n++;
            x->indexes[j] = 0;
            x->n--;
        }
        for (j = 0; j < 6; j++) {
            x->child_ptr[j] = NULL;
        }
        np1->indexes[0] = mid;
        np1->child_ptr[np1->n] = x;
        np1->child_ptr[np1->n + 1] = np3;
        np1->n++;
        root = np1;
    } else {
        y = x->child_ptr[i];
        mid = y->indexes[2];
        y->indexes[2] = 0;
        y->n--;
        for (j = 3; j < 5; j++) {
            np3->indexes[j - 3] = y->indexes[j];
            np3->n++;
            y->indexes[j] = 0;
            y->n--;
        }
        x->child_ptr[i + 1] = y;
        x->child_ptr[i + 1] = np3;
    }
    return mid;
}
void insert(int a) {
    int i, temp;
    x = root;
    if (x == NULL) {
        root = init();
        x = root;
    } else {
        if (x->leaf == true && x->n == 5) {
            temp = split_child(x, -1);
            x = root;
            for (i = 0; i < (x->n); i++) {
                if ((a > x->indexes[i]) && (a < x->indexes[i + 1])) {
                    i++;
                    break;
                } else if (a < x->indexes[0]) {
                    break;
                } else {
                    continue;
                }
            }
            x = x->child_ptr[i];
        } else {
            while (x->leaf == false) {
                for (i = 0; i < (x->n); i++) {
                    if ((a > x->indexes[i]) && (a < x->indexes[i + 1])) {
                        i++;
                        break;
                    } else if (a < x->indexes[0]) {
                        break;
                    } else {
                        continue;
                    }
                }
                if ((x->child_ptr[i])->n == 5) {
                    temp = split_child(x, i);
                    x->indexes[x->n] = temp;
                    x->n++;
                    continue;
                } else {
                    x = x->child_ptr[i];
                }
            }
        }
    }
    x->indexes[x->n] = a;
    sort(x->indexes, x->n);
    x->n++;
}

int main() {
    int i, n, t;
    cout << "enter the no of elements to be inserted\n";
    cin >> n;
    for (i = 0; i < n; i++) {
        cout << "enter the element\n";
        cin >> t;
        insert(t);
    }
    cout << "traversal of constructed tree\n";
    traverse(root);
}
