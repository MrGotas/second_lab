#include <iostream>
#include <string>
#include <sstream>
#include "massive.h"

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

int correctQuotes(string& elem);
string withoutSlash(string elem);
void temps(Stack& stack, string& lastAdd, string& elem, string& temp1, string& temp2, string& preVal, StrArray& array, int& errors);
void corTemps(Stack& stack, string& preVal, string& temp1, string& temp2, int& errors);
void corArr(string& temp1, string& temp2, StrArray& array);
int to_Stack(StrArray& array);

int main() {
    string userInput;
    getline(cin, userInput);

    Stack stack;
    StrArray array;
    string elem, temp1 = "", temp2 = "", preVal = "";
    stringstream elems(userInput);
    int errors = 0;
    while (elems >> elem){
        if (elem.size() < 3){ // проверка размера
            cerr << "Incorrect XML" << endl;
            return 1;
        }

        errors += correctQuotes(elem); // проверка корректности кавычек
        if (errors > 1){
            cerr << "Incorrect XML." << endl;
            return 1;
        }

        if (elem.size() == 3 && elem[1] == '/'){ // </>
            string lastAdd = stack.topEl();
            array.push(elem + " 1");
            temps(stack, lastAdd, elem, temp1, temp2, preVal, array, errors);
            continue;
        }

        if (stack.topEl() == ""){ //стек пустой
            if (elem[1] == '/'){ // </a>
                string lastAdd = stack.topEl();
                array.push(elem + " 1");
                temps(stack, lastAdd, elem, temp1, temp2, preVal, array, errors);
                continue;
            }else{ // <a>
                stack.push(elem);
                array.push(elem);
                continue;
            }
        }else{ // если что-то есть
            if (elem[1] != '/'){ // <a> <b>
                stack.push(elem);
                array.push(elem);
                continue;
            }
            if (withoutSlash(elem) == stack.topEl()){ // <a> </a>
                stack.del();
                array.push(elem);
                continue;
            }else{
                string lastAdd = stack.topEl(); // <a> </b>
                array.push(elem + " 1");
                temps(stack, lastAdd, elem, temp1, temp2, preVal, array, errors);
                continue;
            }
        }
        cout << errors << endl;
    }

    //cout << "Fin Preval " << preVal << " temp1 " << temp1 << " temp2 " << temp2 << endl;
    if (temp1 != "" && preVal == "" && temp2 == ""){
        cerr << "Unknown value." << endl;
        return 1;
    }else if (temp2 == "" && temp1 != "" && preVal != ""){
        if (temp1.size() - 1 != preVal.size()){
            if (withoutSlash(preVal) == preVal){ // <a> <b> <xb> </a>
                string temp = preVal;
                temp[1] = '/';
                stack.del();
                if (withoutSlash(temp) != stack.topEl()){
                    errors = 2;
                }else{
                    for (size_t i = 0; i < array.sizeM(); i++){
                        string elem = array.get(i);
                        if (elem == preVal){
                            array.replace(i, temp);
                            continue;
                        }
                        stringstream ss (elem);
                        int countWor = 0;
                        string word;
                        while (ss >> word){
                            countWor++;
                        }
                        if (countWor != 1){
                            array.replace(i, temp1);
                            break;
                        }
                    }
                    errors ++;
                }
            }else{
               errors = 2; 
            }
        }else{
            int posInPreVal = 1;
            for (size_t i = 2; i < temp1.size() - 1; i++){
                if (temp1[i] != preVal[posInPreVal]){
                    errors++;
                    temp1[i] = preVal[posInPreVal];
                    posInPreVal++;
                    continue;
                }
                posInPreVal++;
            }
            for (size_t i = 0; i < array.sizeM(); i++){
                string elem = array.get(i);
                stringstream ss (elem);
                int countWor = 0;
                string word;
                while (ss >> word){
                    countWor++;
                }
                if (countWor != 1){
                    array.replace(i, temp1);
                    break;
                }
            }
            stack.del();
        }
    }
    /*cout << "Mass " << endl;
    array.print(errors);*/
    errors += to_Stack(array);

    if (errors > 1){
        cerr << "Incorrect XML" << endl;
        return 1;
    }

    //stack.print();
    array.print(errors);
    return 0;
}

int correctQuotes(string& elem){
    int errors = 0;
    if (elem[0] != '<'){
        errors++;
        elem[0] = '<';
    }
    if (elem[elem.size() - 1] != '>'){
        errors++;
        elem[elem.size() - 1] = '>';
    }

    return errors;
}

string withoutSlash(string elem){
    string temp;
    for (size_t i = 0; i < elem.size(); i++){
        if (i == 1 && elem[i] == '/'){
            continue;
        }
        temp += elem[i];
    }

    return temp;
}

void temps(Stack& stack, string& lastAdd, string& elem, string& temp1, string& temp2, string& preVal, StrArray& array, int& errors){
    if (temp1 == ""){
        preVal = lastAdd;
        temp1 = elem;
    }else{
        temp2 = elem;
        //cout << "Preval " << preVal << " temp1 " << temp1 << " temp2 " << temp2 << endl;
        corTemps(stack, preVal, temp1, temp2, errors);
        if (errors > 1) return;
        corArr(temp1, temp2, array);
    }
}

void corTemps(Stack& stack, string& preVal, string& temp1, string& temp2, int& errors){
    // <a> <b> </c> </a> || <a> <b> </a> <c> </c> </ab>
    if (preVal.size() < temp2.size() - 1){
        if (temp2.size() - 1 != temp1.size()){
            errors = 2;
            return;
        }
        string temp = withoutSlash(temp2);
        for (size_t i = 0; i < temp.size(); i++){
            if (temp[i] != temp1[i]){
                temp1[i] = temp[i];
                errors++;
            }
        }
    }else{
        if (temp1.size() - 1 != preVal.size()){
            errors = 2;
            return;
        }
        if (temp1[1] != '/'){
            errors++;
            temp1[1] = '/';
        }
        int posInPreVal = 1;
        for (size_t i = 2; i < temp1.size() - 1; i++){
            if (temp1[i] != preVal[posInPreVal]){
                errors++;
                temp1[i] = preVal[posInPreVal];
                posInPreVal++;
                continue;
            }
            posInPreVal++;
        }
        stack.del();
        if (withoutSlash(temp2) == stack.topEl()){
            stack.del();
        }else if (temp2[1] == '/'){
            errors++;
        }else{
            stack.push(temp2);
        }
    }
}

void corArr(string& temp1, string& temp2, StrArray& array){
    int correctVal = 0;
    for (size_t i = 0; i < array.sizeM(); i++){
        string elem = array.get(i);
        stringstream ss (elem);
        int countWor = 0;
        string word;
        while (ss >> word){
            countWor++;
        }
        if (countWor != 1){
            if (correctVal == 0){
                array.replace(i, temp1);
                correctVal++;
            }else{
                array.replace(i, temp2);
                correctVal;
            }
        }
    }
}

int to_Stack(StrArray& array){
    int errors = 0;
    Stack stack1;
    for (size_t i = 0; i < array.sizeM(); i++){
        string elem = array.get(i);
        if (elem.size() == 3 && elem[1] == '/'){ // </>
            errors++;
            continue;
        }

        if (stack1.topEl() == ""){ //стек пустой
            if (elem[1] == '/'){ // </a>
                errors++;
                continue;
            }else{ // <a>
                stack1.push(elem);
                continue;
            }
        }else{ // если что-то есть
            if (elem[1] != '/'){ // <a> <b>
                stack1.push(elem);
                continue;
            }
            if (elem[1] == '/' && withoutSlash(elem) == stack1.topEl()){ // <a> </a>
                stack1.del();
                continue;
            }else{
                errors++;
                continue;
            }
        }
    }
    return errors;
}