#include <bits/stdc++.h>
#include <utility>
using namespace std;

class Bucket {
    const long unsigned int size;
    int depth;
    vector<pair<const int, const string>> values;

  public:
    Bucket(const int depth, const int size);
    bool insert(const int key, const string value);
    int getDepth() { return this->depth; };
    vector<pair<const int, const string>> getValues() { return this->values; };
};

Bucket::Bucket(const int depth, const int size) : size(size), depth(depth) {}

bool Bucket::insert(const int key, const string value) {
    cerr << "(b insert) key = " << key << ' ';
    cerr << "size = " << this->values.size() << '\n';
    if (this->values.size() < size) {
        this->values.push_back({key, value});
        return true;
    } else
        return false;
}

class Directory {
    const long unsigned int bucket_size;
    int global_depth;
    vector<Bucket *> buckets;
    int hashing_func(const int n);
    void grow();
    void split(int bucket_no);

  public:
    Directory(const int depth, const int bucket_size);
    void insert(const int key, const string value);
    int getBucketCount() { return 1 << global_depth; };
    int getPosFromKey(const int key) {
        return this->hashing_func(key) % this->getBucketCount();
    }
    void print();
};

Directory::Directory(const int depth, const int bucket_size)
    : bucket_size(bucket_size), global_depth(depth) {
    this->buckets = vector<Bucket *>(this->getBucketCount(),
                                     new Bucket(depth, bucket_size));
}

int Directory::hashing_func(const int n) { return n % 99991; }

void Directory::split(const int bucket_no) {
    // get the bucket pos pointing to the same
    // its always 0xxx and 1xxx
    cerr << "(d_split) " << bucket_no << '\n';
    const int b_pos1 = bucket_no % (this->getBucketCount() / 2);
    const int b_pos2 = b_pos1 + (this->getBucketCount() / 2);
    Bucket *b = this->buckets[bucket_no];
    vector<pair<const int, const string>> v = b->getValues();

    this->buckets[b_pos1] = new Bucket(b->getDepth() + 1, this->bucket_size);
    this->buckets[b_pos2] = new Bucket(b->getDepth() + 1, this->bucket_size);
    for (pair<const int, const string> p : v) {
        this->insert(p.first, p.second);
    }

    delete b;
    cerr << "(d_split) done" << '\n';
}

void Directory::grow() {
    cerr << "(d_grow) to = " << this->global_depth+1 << '\n';
    vector<Bucket *> temp = this->buckets;
    this->buckets.insert(buckets.end(), temp.begin(), temp.end());
    this->global_depth++;
}

void Directory::insert(const int key, const string value) {
    cerr << "(d_insert) key = " << key << " ";

    const int pos = this->getPosFromKey(key);

    cerr << "pos = " << pos << '\n';

    if (!this->buckets[pos]->insert(key, value)) {
        // split the bucket or grow the directory
        if (this->buckets[pos]->getDepth() < this->global_depth) {
            // split the bucket
            this->split(pos);
        } else {
            // grow
            this->grow();
        }

        this->insert(key, value);
    }
}

int main() {
    cout << "Extendible Hashing\n";
    // depth 1 and bucket size 2
    Directory d(1, 1);
    d.insert(3, "string value");
    d.insert(1, "something");
    d.insert(7, "soso");
    cout << endl;
    return 0;
}