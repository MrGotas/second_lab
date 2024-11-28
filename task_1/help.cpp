while (ss >> elem){
        int errors = correctVal(elem); //1

        allErrors += errors; //2

        if (allErrors > 1 || countTemp > 2){ //3
            cout << endl << "Incorrect XML" << endl;
            return 0; 
        }

        if (errors == 1){ //4
            if (countTemp == 0){
                temp1 = elem;
                countTemp++;
            }else{
                temp2 = elem;
                countTemp++;
            }

            cout << elem << " " << errors << " " << "temp1 " << temp1 << " temp2 " << temp2 << "     ";
            continue;
        }

        toStack(elem, stack, temp1, temp2, countTemp, errors);
        cout << elem << " " << errors << " " << "temp1 " << temp1 << " temp2 " << temp2 << "     ";

    }
    cout << endl;
    stack.print();




int correctVal(string& elem){
    int countMis = 0;
    size_t symbols = 0;
    int openS = 0;
    int closS = 0;
    int slash = 0;
    while(elem[symbols] != '\0'){
        if (elem[symbols] == '<'){
            openS++;
        }else if(elem[symbols] == '>'){
            closS++;
        }else if(elem[symbols] == '/'){
            slash++;
        }
        symbols ++;
    }
    
    if (symbols < 3){ // если количество меьнше допустимого
        countMis = 2;
        return countMis;
    }

    if (slash > 1 || openS !=1 || closS != 1){
        countMis ++;
        if (slash > 2){
            countMis = 2;
        }else if(openS > 2){
            countMis = 2;
        }else if(closS > 2){
            countMis = 2;
        }
    }

    if (slash == 1 && elem[1] != '/'){ ///////////////// сделать ращбор смысловой нагрузки запроса 
        countMis ++;
    }

    if (elem[0] != '<'){ // наличие открывающего символа
        countMis++;
        elem[0] = '<';
    }

    if (elem[symbols - 1] != '>'){ // наличие закрывающего символа
        countMis++;
        elem[symbols] = '>';
    }

    return countMis;
}

void correctTemp (string& temp1, string& temp2, int& errors, int countTemp, Stack& stack){
    if (countTemp == 1 && temp2 != "0"){
        bool slashTemp1 = false;
        string valueTemp1;
        for (size_t i = 0; temp1[i] != '\0'; ++i){
            if(temp1[i] == '/'){
                slashTemp1 = true;
                continue;
            }
            valueTemp1 += temp1[i];
        }

        if (slashTemp1 == true){
            int changes = 0;
            string topVal = stack.topEl();
            for (size_t i = 0; valueTemp1[i] != '\0'; ++i){
                if (valueTemp1[i] != topVal[i]){
                   changes++; 
                }
            }
            if (changes == 1){
                for (size_t i = 1; valueTemp1[i] != '\0'; ++i){
                    if (temp1[i + 1] != topVal[i]){
                        temp1[i + 1] = topVal[i];
                    }
                }
                temp2 = "0";
            }
        }
        errors++;
        return;
    }

    if (countTemp == 2){
        bool slashTemp2 = false;
        string valueTemp2;
        for (size_t i = 0; temp2[i] != '\0'; ++i){
            if (temp2[i] == '/'){
                slashTemp2 = true;
                continue;
            }
            valueTemp2 += temp2[i];
        }

        if (slashTemp2 == true){
            int changes = 0;
            for (size_t i = 0; valueTemp2[i] != '\0'; ++i){
                if (valueTemp2[i] != temp1[i]){
                    temp1[i] = valueTemp2[i];
                    changes++;
                }
            }
            errors += changes;
        }
    }
}

bool toStack (string elem, Stack& stack, string& temp1, string& temp2, int& countTemp, int& errors){
    bool slash = false;
    string value;
    for (size_t i = 0; elem[i] != '\0'; ++i){
        if (elem[i] == '/'){
            slash = true;
            continue;
        }
        value += elem[i];
    }

    if (stack.topEl() != value && slash == false){
        stack.push(elem);
    }else if (stack.topEl() == value && slash == true){
        stack.del();
    }else{
        if (countTemp == 0){
            temp1 = elem;
            countTemp++;
            return false;
        }else{
            temp2 = elem;
            countTemp ++;
            return false;
        }
        correctTemp(temp1, temp2, errors, countTemp, stack);
    }
    return true;
}
