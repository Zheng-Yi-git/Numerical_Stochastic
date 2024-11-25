#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include <functional>
#include <iostream>

// Abstract base class defining the interface for hash functions
// K: The type of keys to be hashed
template <typename K>
class Hasher {
public:
    // Pure virtual function that computes a hash value for a given key
    // Returns: size_t hash value for the key
    virtual size_t operator()(const K& key) const = 0;
};

// Abstract base class defining the interface for key equality comparison
// K: The type of keys to be compared
template <typename K>
class EqualityPredicate {
public:
    // Pure virtual function that compares two keys for equality
    // Returns: true if keys are equal, false otherwise
    virtual bool operator()(const K& a, const K& b) const = 0;
};

// Default implementation of Hasher using C++ standard library's hash function
template <typename K>
class DefaultHasher : public Hasher<K> {
public:
    // Computes hash using std::hash
    // Returns: size_t hash value for the key
    size_t operator()(const K& key) const override {
        return std::hash<K>()(key);
    }
};

// Default implementation of EqualityPredicate using C++ equality operator
template <typename K>
class DefaultEqualityPredicate : public EqualityPredicate<K> {
public:
    // Compares keys using == operator
    // Returns: true if keys are equal, false otherwise
    bool operator()(const K& a, const K& b) const override {
        return a == b;
    }
};

// Hash table implementation using separate chaining for collision resolution
// K: Key type
// V: Value type
template <typename K, typename V>
class Hashtable {
public:
    // Constructor
    // Parameters:
    //   size: Initial number of buckets (default: 101)
    //   hasher: Custom hash function (default: nullptr, uses DefaultHasher)
    //   equalityPredicate: Custom equality comparison (default: nullptr, uses DefaultEqualityPredicate)
    Hashtable(size_t size = 101, Hasher<K>* hasher = nullptr, EqualityPredicate<K>* equalityPredicate = nullptr)
        : buckets(size), hasher(hasher), equalityPredicate(equalityPredicate) {
        if (!this->hasher) {
            this->hasher = new DefaultHasher<K>();
        }
        if (!this->equalityPredicate) {
            this->equalityPredicate = new DefaultEqualityPredicate<K>();
        }
    }

    ~Hashtable() {
        //delete hasher;
        //delete equalityPredicate;
    }

    // Inserts or updates a key-value pair in the hash table
    // If the key exists, updates the value; otherwise, inserts a new pair
    void insert(const K& key, const V& value) {
        size_t index = (*hasher)(key) % buckets.size();
        for (auto& pair : buckets[index]) {
            if ((*equalityPredicate)(pair.first, key)) {
                pair.second = value;
                return;
            }
        }
        buckets[index].emplace_back(key, value);
    }

    // Searches for a value associated with the given key
    // Parameters:
    //   key: The key to search for
    //   value: Reference to store the found value
    // Returns: true if key found, false otherwise
    bool find(const K& key, V& value) const {
        size_t index = (*hasher)(key) % buckets.size();
        for (const auto& pair : buckets[index]) {
            if ((*equalityPredicate)(pair.first, key)) {
                value = pair.second;
                return true;
            }
        }
        return false;
    }

    // Removes a key-value pair from the hash table
    // If the key doesn't exist, no action is taken
    void remove(const K& key) {
        size_t index = (*hasher)(key) % buckets.size();
        buckets[index].remove_if([this, &key](const std::pair<K, V>& pair) {
            return (*equalityPredicate)(pair.first, key);
        });
    }

private:
    std::vector<std::list<std::pair<K, V>>> buckets;  // Vector of linked lists for separate chaining
    Hasher<K>* hasher;                                // Pointer to hash function object
    EqualityPredicate<K>* equalityPredicate;          // Pointer to equality comparison object
};

#endif // HASHTABLE_H
