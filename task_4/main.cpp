// var 1
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Array{
    string* data;
    size_t size = 0;
    size_t capacity;

    Array(size_t initialCapacity = 5){
        size = 0;
        capacity = initialCapacity;
        data = new string[capacity];
    }

    ~Array() {
        delete[] data;
    }

    void resize() {
        size_t newCapacity = capacity * 2;
        string* newData = new string[newCapacity]; // новый массив
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data; 
        data = newData; //указатель на новый массив
        capacity = newCapacity;
    }

    void push(const string& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value; // заносим строку и увеличиваем размер
    }



    void output(){
        for (size_t i = 0; i < size; i++){
            cout << data[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    cout << "Enter values: ";
    string input;
    getline (cin, input);
    stringstream ss (input);
    string elem;

    Array mass;
    while (getline(ss, elem)){
        mass.push(elem);
    }

    mass.output();
}