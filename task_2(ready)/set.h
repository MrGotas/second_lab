#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

struct Set {
    struct KeyNode {
        string key;
        KeyNode* next;

        KeyNode(const string& k) : key(k), next(nullptr) {}
    };

    KeyNode** table;
    size_t tableSize;

    Set(size_t initialCapacity = 10) : tableSize(initialCapacity) {
        table = new KeyNode*[tableSize];
        for (size_t i = 0; i < tableSize; ++i) {
            table[i] = nullptr;
        }
    }

    ~Set() {
        for (size_t i = 0; i < tableSize; ++i) {
            KeyNode* current = table[i];
            while (current != nullptr) {
                KeyNode* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }

    size_t hashFunction(const string& key) const {
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % tableSize;
    }

    bool setAdd(const string& key) {
        size_t hash = hashFunction(key);
        KeyNode* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                return false; // Ключ уже существует
            }
            current = current->next;
        }
        KeyNode* newNode = new KeyNode(key);
        newNode->next = table[hash];
        table[hash] = newNode;
        return true;
    }

    bool set_at(const string& key) const {
        size_t hash = hashFunction(key);
        KeyNode* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false; // Ключ не найден
    }

    bool setDel(const string& key) {
        size_t hash = hashFunction(key);
        KeyNode* current = table[hash];
        KeyNode* prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[hash] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false; // Ключ не найден
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (size_t i = 0; i < tableSize; ++i) {
            KeyNode* current = table[i];
            while (current != nullptr) {
                file << current->key << endl;
                current = current->next;
            }
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            setAdd(line);
        }
        file.close();
    }
};

#endif // SET_H_INCLUDED