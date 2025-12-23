#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <functional>
#include <type_traits>
#include <QString>
#include "linkedlist.h"

using namespace std;

template<typename K, typename V>
class HashTable {
private:
    struct KeyValuePair {
        K key;
        V value;

        KeyValuePair() = default;
        KeyValuePair(const K& k, const V& v) : key(k), value(v) {}
    };

    static const int TABLE_SIZE = 1009;
    LinkedList<KeyValuePair>* table;
    int count;


    int hashString(const string& key) const {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % TABLE_SIZE;
    }

    int hash(const K& key) const {
        if constexpr (is_same_v<K, string>) {
            return hashString(key);
        } else if constexpr (is_same_v<K, QString>) {
            return hashString(key.toStdString());
        } else if constexpr (is_integral_v<K>) {
            return static_cast<int>(key) % TABLE_SIZE;
        } else {

            return (reinterpret_cast<unsigned long>(&key)) % TABLE_SIZE;
        }
    }

public:
    HashTable() : count(0) {
        table = new LinkedList<KeyValuePair>[TABLE_SIZE];
    }

    ~HashTable() {
        delete[] table;
    }


    HashTable(const HashTable& other) : count(other.count) {
        table = new LinkedList<KeyValuePair>[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = other.table[i];
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            delete[] table;
            table = new LinkedList<KeyValuePair>[TABLE_SIZE];
            count = other.count;
            for (int i = 0; i < TABLE_SIZE; i++) {
                table[i] = other.table[i];
            }
        }
        return *this;
    }

    void insert(const K& key, const V& value) {
        int index = hash(key);


        bool found = false;
        for (int i = 0; i < table[index].getSize(); i++) {
            if (table[index].at(i).key == key) {
                table[index].at(i).value = value;
                found = true;
                break;
            }
        }


        if (!found) {
            table[index].append(KeyValuePair(key, value));
            count++;
        }
    }


    bool find(const K& key, V& value) const {
        int index = hash(key);

        for (int i = 0; i < table[index].getSize(); i++) {
            if (table[index].at(i).key == key) {
                value = table[index].at(i).value;
                return true;
            }
        }
        return false;
    }

    bool contains(const K& key) const {
        int index = hash(key);

        for (int i = 0; i < table[index].getSize(); i++) {
            if (table[index].at(i).key == key) {
                return true;
            }
        }
        return false;
    }


    bool remove(const K& key) {
        int index = hash(key);

        for (int i = 0; i < table[index].getSize(); i++) {
            if (table[index].at(i).key == key) {
                table[index].removeAt(i);
                count--;
                return true;
            }
        }
        return false;
    }


    LinkedList<K> getAllKeys() const {
        LinkedList<K> keys;
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (int j = 0; j < table[i].getSize(); j++) {
                keys.append(table[i].at(j).key);
            }
        }
        return keys;
    }


    LinkedList<V> getAllValues() const {
        LinkedList<V> values;
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (int j = 0; j < table[i].getSize(); j++) {
                values.append(table[i].at(j).value);
            }
        }
        return values;
    }

    int getSize() const { return count; }
    bool isEmpty() const { return count == 0; }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].clear();
        }
        count = 0;
    }


    void forEach(function<void(const K&, V&)> callback) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (int j = 0; j < table[i].getSize(); j++) {
                callback(table[i].at(j).key, table[i].at(j).value);
            }
        }
    }


    double getLoadFactor() const {
        return static_cast<double>(count) / TABLE_SIZE;
    }
};

#endif
