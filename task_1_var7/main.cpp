#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Stack {
    // Определение узла списка
    struct Node {
        string data;
        Node* next;
        Node(const string& value) : data(value), next(nullptr) {}
    };

    Node* top;

    Stack() : top(nullptr) {}

    ~Stack() {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const string& value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    void del() {
        if (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    string topEl(){
        if (top != nullptr){
            return top->data;
        }else{
            return "";
        }
    }

    void print() const {
        Node* current = top;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

};

int correctVal(string& elem);

int main() {
    string userInput;

    getline(cin, userInput);
    stringstream words(userInput);
    string word;
    int countWords= 0;
    while(words >> word){
        countWords++;
    }

    string elem;
    stringstream ss(userInput);
    string temp1, temp2;
    int countTemp = 0;
    int allErrors = 0;
    Stack stack;

    while (ss >> elem){
        
        int errors = correctVal(elem);
        cout << elem << " " << errors << " ";

        allErrors += errors;
        if (allErrors > 1){
            cout << endl << "Incorrect XML";
        }
    }
    cout << endl;
}

int correctVal(string& elem){
    int countEr = 0;
    int openS = 0, closS = 0, slash = 0;
    size_t symbols = 0;
    while (elem[symbols] != '\0'){
        if (elem[symbols] == '<'){
            openS++;
        }else if(elem[symbols] == '>'){
            closS++;
        }else if(elem[symbols] == '/'){
            slash++;
        }
        symbols++;
    }

    if (symbols < 3){
        countEr = 2;
        return countEr;
    }

    if(slash > 1){
        countEr++;
        if (slash > 2){
            countEr = 2;
            return countEr;
        }
    }

    if (openS != 1){
        if (openS == 0){
            elem[0] = '<';
            countEr++;
        }
        if (openS > 2){
            countEr = 2;
            return countEr;
        }
    }

    if (closS != 1){
        if (closS == 0){
            elem[symbols - 1] = '>';
            countEr++;
        }
        if (closS > 2){
            countEr = 2;
            return countEr;
        }
    }

    return countEr;
}