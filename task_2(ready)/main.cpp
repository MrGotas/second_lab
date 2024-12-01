#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "set.h"

using namespace std;
namespace fs = filesystem;

bool emptyFile (string fileName);

int main(int argc, char* argv[]){
    if (argc != 6){
        cerr << "Unknown command." << endl;
        return 1;
    }
    string temp1 = argv[1];
    string temp2 = argv[3];
    if (temp1 != "--file" || temp2 != "--query"){
        cerr << "1.Unknown command." << endl;
        return 1;
    }

    string fileName = "files/";
    fileName += argv[2];
    string action;
    action = argv[4];

    Set mySet;
    if (fs::exists(fileName)){
        mySet.loadFromFile(fileName);
    }else{
        ofstream file(fileName);
        file.close();
    }

    string val = argv[5];

    if (action == "SETADD"){
        if (!mySet.setAdd(val)){
            cerr << "Val already exists." << endl;
            return 1;
        }

    }else if(emptyFile(fileName)){
        cerr << "Empty file" << endl;
        return 1;

    }else if (action == "SETDEL"){
        if (!mySet.setDel(val)){
            cerr << "Unknown val" << endl;
            return 1;
        }

    }else if (action == "SET_AT"){
        if (mySet.set_at(val)){
            cout << "Value exists" << endl;
            return 0;
        }else{
            cout << "Value does not exists" << endl;
            return 0;
        }

    }else{
        cerr << "1.Unknown command." << endl;
        return 1;
    }

    mySet.saveToFile(fileName);
    
    if (emptyFile(fileName)){
        fs::remove(fileName);
    }
    return 0;
}

bool emptyFile (string fileName) {
    ifstream file (fileName);

    string temp;
    if(getline(file, fileName)){
        file.close();
        return false;
    }else{
        file.close();
        return true;
    }
}