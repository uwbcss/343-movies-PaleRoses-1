/**
 * @location header/hashtable.h
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <functional>
#include <list>
#include <string>
#include <vector>

// Hash table with separate chaining
template <typename K, typename V> class HashTable {
  private:
    using Bucket = std::list<std::pair<K, V>>;

    std::vector<Bucket> table;
    size_t numElements;
    size_t tableSize;
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    size_t hash(const K &key) const { return std::hash<K>{}(key) % tableSize; }

    // Resize when load factor exceeded
    void resize() {
        size_t newSize = tableSize * 2;
        std::vector<Bucket> newTable(newSize);

        for (const auto &bucket : table) {
            for (const auto &pair : bucket) {
                size_t newIndex = std::hash<K>{}(pair.first) % newSize;
                newTable[newIndex].push_back(pair);
            }
        }

        table = std::move(newTable);
        tableSize = newSize;
    }

  public:
    explicit HashTable(size_t initialSize = 101)
        : table(initialSize), numElements(0), tableSize(initialSize) {}

    // Insert key-value pair, returns false if key exists
    bool insert(const K &key, const V &value) {
        size_t index = hash(key);

        for (const auto &pair : table[index]) {
            if (pair.first == key) {
                return false;
            }
        }

        table[index].emplace_back(key, value);
        numElements++;

        if (static_cast<double>(numElements) / tableSize > MAX_LOAD_FACTOR) {
            resize();
        }

        return true;
    }

    // Find value by key, returns nullptr if not found
    V *find(const K &key) {
        size_t index = hash(key);

        for (auto &pair : table[index]) {
            if (pair.first == key) {
                return &pair.second;
            }
        }

        return nullptr;
    }

    const V *find(const K &key) const {
        size_t index = hash(key);

        for (const auto &pair : table[index]) {
            if (pair.first == key) {
                return &pair.second;
            }
        }

        return nullptr;
    }

    bool contains(const K &key) const { return find(key) != nullptr; }

    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
};

#endif // HASHTABLE_H