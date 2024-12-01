#include <iostream>
#include <string>
#include <sstream>
#include "stack.h"
#include "massive.h"

using namespace std;

struct HashTable {
    struct KeyValuePair {
        string key;
        string value;
        KeyValuePair* next;

        KeyValuePair(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
    };

    KeyValuePair** table;
    size_t tableSize;

    HashTable(size_t initialCapacity = 10) : tableSize(initialCapacity) {
        table = new KeyValuePair*[tableSize];
        for (size_t i = 0; i < tableSize; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < tableSize; ++i) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                KeyValuePair* next = current->next;
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

    bool push(const string& key, const string& value) {
        size_t hash = hashFunction(key);
        KeyValuePair* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                cerr << "Ошибка. Такой потомок уже есть" << endl;
                return false;
            }
            current = current->next;
        }
        KeyValuePair* newPair = new KeyValuePair(key, value);
        newPair->next = table[hash];
        table[hash] = newPair;
        return true;
    }

    int countKeys(string val){
        for (size_t i = 0; i < tableSize; ++i){
            KeyValuePair* current = table[i];
            while(current != nullptr){
                if (current->key == val){
                    return 1;
                }
                current = current->next;
            }
        }
        return 0;
    }

    void findVal(int& countDes, Stack& tempSt){
        string val = tempSt.topEl();
        tempSt.del();
        for (size_t i = 0; i < tableSize; ++i){
            KeyValuePair* current = table[i];
            while(current != nullptr){
                if (current -> value == val){
                    tempSt.push(current->key);
                    countDes++;
                }
                current = current->next;
            }
        }
        if (tempSt.isEmpty()){
            return;
        }else{
            findVal(countDes, tempSt);
        }
    }

    bool del(const string& key) {//----------------------
        size_t hash = hashFunction(key);
        KeyValuePair* current = table[hash];
        KeyValuePair* prev = nullptr;
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

    void print() {
        for (size_t i = 0; i < tableSize; ++i) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                cout << current->key << ";" << current->value << " ";
                current = current->next;
            }
        }
    }
};

int main(){
    int countElem;
    string countElemStr;
    cout << "Введите кол-во: ";
    getline(cin, countElemStr);
    countElem = stoi(countElemStr);

    if (countElem < 1){
        cerr << "Некорректное значение" << endl;
        return 1;
    }

    HashTable peoples;

    while (countElem != 0){
        string relatives;
        getline(cin, relatives);

        stringstream ss (relatives);
        string key, val;
        ss >> key;
        ss >> val;
        if (!peoples.push(key, val)){
            return 1;
        }
        countElem--;
    }

    StrArray outAr;
    Stack tempSt;
    for (size_t i = 0; i < peoples.tableSize; ++i) {
        HashTable::KeyValuePair* current = peoples.table[i];
        while (current != nullptr) {
            string key = current->key;
            int countDes = 0;
            tempSt.push(key);
            peoples.findVal(countDes, tempSt);
            outAr.push(key + " " + to_string(countDes));
            tempSt.print();
            current = current->next;
        }
    }
    // прородитель рода
    bool exit = false;
    for (size_t i = 0; i < peoples.tableSize; ++i) {
        HashTable::KeyValuePair* current = peoples.table[i];
        if (exit){
            break;
        }
        while (current != nullptr) {
            string val = current->value;
            if (peoples.countKeys(val) == 0){
                int countDes = 0;
                tempSt.push(val);
                peoples.findVal(countDes, tempSt);
                outAr.push(val + " " + to_string(countDes));
                exit = true;
                break;
            }
            current = current->next;
        }
    }

    outAr.sortAr();
    cout << endl << "Вывод: " << endl;
    outAr.print();

    return 0;
}