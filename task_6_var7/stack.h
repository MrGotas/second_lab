#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <iostream>
#include <string>

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

    string topEl(){
        if (top != nullptr){
            return top->data;
        }else{
            return "";
        }
    }

    void del() {
        if (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void print() {
        Node* current = top;
        while (current != nullptr) {
            cout << current->data << endl;
            current = current->next;
        }
    }
};

#endif // STACK_H_INCLUDED