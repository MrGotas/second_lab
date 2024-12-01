#include <iostream>
#include <sstream>

using namespace std;

struct Set {
    int* data;   
    size_t size = 0; // размер
    size_t capacity; // ёмкость

    Set(size_t initialCapacity = 5) {
        size = 0;
        capacity = initialCapacity; // Устанавливаем начальную емкость
        data = new int[capacity]; // Выделяем память под массив
    }

    ~Set() {
        delete[] data;
    }

    void resize() {
        size_t newCapacity = capacity * 2;
        int* newData = new int[newCapacity]; // новый массив
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data; 
        data = newData; //указатель на новый массив
        capacity = newCapacity;
    }

    bool push(const int& value) {
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == value) {
                return false; // Элемент уже существует
            }
        }
        if (size >= capacity) {
            resize();
        }

        data[size++] = value;
        return true;
    }

    size_t sizeM() const {
        return size; // Возвращаем текущий размер
    }

    bool get(size_t index, int& result) {
        if (index < 0 || index >= size) {
            cout << "ERROR: Index out of range." << endl;
            return false;
        }
        result = data[index];
        return true;
    }

    void del(size_t index) {
        if (index < 0 || index >= size) {
            cout << "ERROR: Index out of range." << endl;
            return;
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    void print() const {
        for (size_t i = 0; i < size; ++i) {
            cout << data[i];
            if (i < size - 1) {
                cout << ", ";
            }
        }
    }
};
void partition(Set& values, int subsetSum);
bool findSubset(Set& values, int targetSum, Set& subset);

int main() {
    cout << "Enter nums in set: ";
    string userInput;
    getline(cin, userInput);

    stringstream ss(userInput);
    int val;
    Set values;
    while (ss >> val) {
        if (!values.push(val)) {
            cerr << "The value " << val << " already exists." << endl;
            return 1;
        }
    }

    int sum;
    cout << "Enter sum: ";
    cin >> sum;
    int totalSum = 0;
    for (size_t i = 0; i < values.sizeM(); i++) {
        int temp;
        values.get(i, temp);
        totalSum += temp;
    }

    if (sum == 0 || totalSum % sum != 0) {
        cerr << "Cannot partition the set into subsets with equal sum." << endl;
        return 1;
    }

    partition(values, sum);
    cout << endl;

    return 0;
}

void partition(Set& values, int subsetSum) {
    while (values.sizeM() > 0) {
        Set subset;
        if (!findSubset(values, subsetSum, subset)) {
            cout << "Cannot partition the set into subsets with equal sum." << endl;
            return;
        }

        cout << "{";
        subset.print();
        cout << "} ";
    }
}

bool findSubset(Set& values, int targetSum, Set& subset) {
    int currentSum = 0;
    for (size_t i = 0; i < subset.sizeM(); i++) {
        int temp;
        subset.get(i, temp);
        currentSum += temp;
    }

    if (currentSum == targetSum) {
        return true;
    }

    for (size_t i = 0; i < values.sizeM(); i++) {
        int num;
        values.get(i, num);
        if (currentSum + num <= targetSum) {
            subset.push(num);
            values.del(i);

            if (findSubset(values, targetSum, subset)) {
                return true;
            }

            subset.del(subset.sizeM() - 1); // Удаляем последний элемент
            values.push(num);
        }
    }

    return false;
}