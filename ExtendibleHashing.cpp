#include <bits/stdc++.h>
#include <cassert>
#include <string>
#include <utility>
using namespace std;

#define BIGPRIME 99991

class Bucket {
    const long unsigned int size;
    int depth;
    list<pair<const int, string>> values;
    bool updateKey(const int key, const string value);
    string getValueFromKey(const int key);

  public:
    Bucket(const int depth, const int size);
    bool insert(const int key, const string value);
    int getDepth() { return this->depth; };
    bool mergeFromBucket(Bucket *b) {
        for (pair<int, string> p : b->getValues()) {
            if (!this->insert(p.first, p.second))
                return false;
        }
        // reduce depth
        this->depth--;
        return true;
    };
    list<pair<const int, string>> getValues() { return this->values; };
    int findKey(const int key);
    bool remove(const int key);
};

Bucket::Bucket(const int depth, const int size) : size(size), depth(depth) {
    assert(this->depth > 0 && this->size > 0);
}

int Bucket::findKey(const int key) {
    int counter = 0;
    for (pair<const int, const string> p : this->values) {
        if (p.first == key)
            return counter;
        counter++;
    }

    return -1;
}

string Bucket::getValueFromKey(const int key) {
    for (pair<const int, const string> p : this->values) {
        if (p.first == key)
            return p.second;
    }

    return "\0";
}

bool Bucket::updateKey(const int key, const string value) {
    for (pair<const int, string> &p : this->values) {
        if (p.first == key) {
            p.second = value;
            return true;
        }
    }

    return false;
}

bool Bucket::insert(const int key, const string value) {
    cerr << "(b insert) key = " << key << ' ';
    cerr << "size = " << this->values.size() << '\n';

    if (findKey(key) >= 0) {
        this->updateKey(key, value);
        return true;
    }

    if (this->values.size() < size) {
        this->values.emplace_back(make_pair(key, value));
        return true;
    } else
        return false;
}

bool Bucket::remove(const int key) {
    if (findKey(key) >= 0) {
        this->values.remove({key, getValueFromKey(key)});
        return true;
    } else {
        return false;
    }
}

class Directory {
    const long unsigned int bucket_size;
    int global_depth;
    vector<Bucket *> buckets;
    int hashing_func(const int n) { return n % BIGPRIME; };
    void grow();
    void split(int bucket_no);
    int getBucketCount() { return 1 << global_depth; };
    int getPosFromKey(const int key) {
        return this->hashing_func(key) % this->getBucketCount();
    }
    void join();

  public:
    Directory(const int depth, const int bucket_size);
    void insert(const int key, const string value);
    void remove(const int key);

    void print();
};

Directory::Directory(const int depth, const int bucket_size)
    : bucket_size(bucket_size), global_depth(depth) {
    assert(this->global_depth > 0 && this->bucket_size > 0);

    this->buckets = vector<Bucket *>(this->getBucketCount(),
                                     new Bucket(depth, bucket_size));
}

void Directory::join() {
    // try to join all buckets
    const int half_count = this->getBucketCount() / 2;
    for (int i = 0; i < half_count; i++) {
        // check if two buckets can be merged
        if (this->buckets[i] != this->buckets[i + half_count] &&
            (this->buckets[i]->getValues().size() +
                 this->buckets[i + half_count]->getValues().size() <=
             this->bucket_size)) {
            cout << "(d_join) " << i << " " << i + half_count << '\n';
            // merge the second bucket into first
            this->buckets[i]->mergeFromBucket(this->buckets[i + half_count]);
            delete this->buckets[i + half_count];
            this->buckets[i + half_count] = this->buckets[i];
        }
    }
}

void Directory::split(const int bucket_no) {
    // get the bucket pos pointing to the same
    // its always 0xxx and 1xxx
    cerr << "(d_split) " << bucket_no << '\n';
    const int b_pos1 = bucket_no % (this->getBucketCount() / 2);
    const int b_pos2 = b_pos1 + (this->getBucketCount() / 2);

    Bucket *b = this->buckets[bucket_no];

    this->buckets[b_pos1] = new Bucket(b->getDepth() + 1, this->bucket_size);
    this->buckets[b_pos2] = new Bucket(b->getDepth() + 1, this->bucket_size);

    for (pair<const int, const string> p : b->getValues()) {
        this->insert(p.first, p.second);
    }

    delete b;
    cerr << "(d_split) done" << '\n';
}

void Directory::grow() {
    cerr << "(d_grow) to = " << this->global_depth + 1 << '\n';
    const vector<Bucket *> temp = this->buckets;
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

void Directory::remove(const int key) {
    const int pos = this->getPosFromKey(key);
    if (this->buckets[pos]->remove(key)) {
        cerr << "(d_remove) " << key << '\n';
    }
    this->join();
}

int main() {
    cout << "Extendible Hashing\n";
    // depth 1 and bucket size 2
    Directory d(1, 1);
    d.insert(3, "string value");
    d.insert(1, "something");
    d.insert(7, "soso");
    d.insert(3, "something else");
    d.remove(3);
    cout << endl;
    return 0;
}