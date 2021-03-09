#include "Restaurant.h"
#include "Dish.h"
#include "Table.h"
#include <fstream>
#include <string>
#include <iostream>
#include<vector>


using namespace std;


Restaurant::Restaurant(const string &configFilePath) {

    string line;
    ifstream myfile (configFilePath);

    int counterOfL=0;
    int counterOfDish=0;
    if (myfile.is_open()) {

        while (getline(myfile, line)) {

            if ((line[0] != '#' ) & (line.size() != 0)) {// if the line is not blank

                if (counterOfL == 0) {//number of lines
                    counterOfL = counterOfL + 1;
                }

                else if (counterOfL == 1) {//capacity of each table

                    string tCapacity = "";
                    int capacity = 0;

                    for (unsigned int i = 0; i <line.size(); i++) {
                        if (line[i] != ',') {
                            tCapacity = tCapacity + line[i];
                            capacity = stoi(tCapacity);
                            tCapacity = "";
                            Table* t=new Table(capacity);
                            tables.push_back(t);

                        }
                    }
                    counterOfL = counterOfL + 1;
                }

                else if (counterOfL == 2) {
                    int counterOfCom = 0;
                    string dishName = "";
                    string type = "";
                    string price = "";
                    for (unsigned int i = 0; i < line.size(); i++) {
                        if (line[i] != ',') {
                            if (counterOfCom == 0) {
                                dishName = dishName + line[i];
                            }
                            if (counterOfCom == 1) {
                                type = type + line[i];
                            }
                            if (counterOfCom == 2) {
                                price = price + line[i];
                            }
                        } else {
                            counterOfCom = counterOfCom + 1;

                        }

                    }


                    int dishPrice = stoi(price);
                    DishType type1;//check which type
                    if (type[0] == 'V') {
                        type1 = VEG;
                    }
                    if (type[0] == 'B') {
                        type1 = BVG;
                    }
                    if (type[0] == 'A') {
                        type1 = ALC;
                    }
                    if (type[0] == 'S') {
                        type1 = SPC;
                    }

                    menu.push_back(Dish(counterOfDish, dishName, dishPrice, type1));
                    counterOfDish = counterOfDish + 1;

                }

            }

        }
    }

            }

Restaurant::~Restaurant() {//destructor
    clear1();

}
Restaurant::Restaurant(const Restaurant &other):open(other.open) {//copy constructor
copy(other);
}

Restaurant& Restaurant::operator=(const Restaurant & other) {
    if(&other != this){
        clear1();
        copy(other);
        this->open = other.open;
    }
    return *this;
}
Restaurant :: Restaurant(Restaurant && other): open(other.open){
    steal(other);
}
Restaurant& Restaurant ::operator=(Restaurant && other){
    this->clear1();
    this->open = other.open;
    steal(other);
    return *this;

}

void Restaurant:: clear1() {
    this->menu.clear();

    for (unsigned int i = 0; i < tables.size(); i++) {
        delete (tables[i]);
    }
    tables.clear();
    for (unsigned int i = 0; i < actionsLog.size(); i++) {
        delete (actionsLog[i]);
    }

    actionsLog.clear();
}
void Restaurant :: copy(const Restaurant &other){
    for(unsigned int i = 0; i < other.menu.size(); i++){
        this->menu.push_back(other.menu[i]);
    }
    for(unsigned int i = 0;i< other.tables.size(); i++){
        Table* t=new Table(*(other.tables[i]));
        this->tables.push_back(t);

    }
    for(unsigned int i = 0; i < other.actionsLog.size(); i++){
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    }

}
void Restaurant :: steal( Restaurant & other){
    this->actionsLog=move(other.actionsLog);
    this->menu=move(other.menu);
    this->tables=move(this->tables);
    this->open=other.open;
}


void Restaurant::start() {
    open = true;
    cout << "Restaurant is now open!" << endl;
    string inputLine;
    string inputAction = "";
    string nameOfAction;
    int customerCounter = 0;


    while (inputLine != "closeall") {
        getline(cin, inputLine);//reciving input from the user and putting it in inputLine
        int index = inputLine.find(" ");
        nameOfAction = inputLine.substr(0, index); //getting the name of the action
        inputLine = inputLine.substr(index + 1); //delete the name of the action


        if (nameOfAction == "open") {
            int index2 = inputLine.find(" ");
            int tableId = stoi(inputLine.substr(0, index2));
            inputLine = inputLine.substr(index2 + 1);
            vector<Customer *> customersVec;
            string nameOfCustomer;
            string typeOfCustomer;
            int indexOfComma;
            int indexOfGap;

            while (inputLine.size() != 0) {
                indexOfComma = inputLine.find(',');
                indexOfGap = inputLine.find(" ");
                nameOfCustomer = inputLine.substr(0, indexOfComma);//getting the name
                if (indexOfGap != -1) {//we are not at the end of the line
                    typeOfCustomer = inputLine.substr(indexOfComma + 1, 3);//getting th type
                } else if (indexOfGap == -1) {
                    typeOfCustomer = inputLine.substr(indexOfComma + 1, inputLine.size());//getting th type

                }

                if (typeOfCustomer == "veg") {
                    customersVec.push_back(new VegetarianCustomer(nameOfCustomer, customerCounter));
                }
                if (typeOfCustomer == "alc") {
                    customersVec.push_back(new AlchoholicCustomer(nameOfCustomer, customerCounter));
                }
                if (typeOfCustomer == "chp") {
                    customersVec.push_back(new CheapCustomer(nameOfCustomer, customerCounter));
                }
                if (typeOfCustomer == "spc") {
                    customersVec.push_back(new SpicyCustomer(nameOfCustomer, customerCounter));
                }

                if (indexOfGap != -1) {
                    customerCounter = customerCounter + 1;
                    inputLine = inputLine.substr(indexOfGap + 1);//shrinking the line size
                } else {
                    customerCounter = customerCounter + 1;
                    inputLine = "";
                }


            }
            BaseAction *openTable = new OpenTable(tableId, customersVec);
            openTable->act(*this);
            actionsLog.push_back(openTable);
        }

        if (nameOfAction == "order") {
            string temp = inputLine.substr(0);
            int tableId = stoi(temp);
            BaseAction *order = new Order(tableId);
            order->act(*this);
            actionsLog.push_back(order);

        }

        if (nameOfAction == "close") {
            int tableId = stoi(inputLine.substr(0, inputLine.size()));
            BaseAction *close = new Close(tableId);
            close->act(*this);
            actionsLog.push_back(close);
        }
        if (nameOfAction == "move") {
            int srcTable;
            int dstTable;
            int customerId;
            int gap1 = inputLine.find(" ");
            srcTable = stoi(inputLine.substr(0, gap1));
            inputLine = inputLine.substr(gap1 + 1);
            int gap2 = inputLine.find(" ");
            dstTable = stoi(inputLine.substr(0, gap2));
            inputLine = inputLine.substr(gap2 + 1);
            customerId = stoi(inputLine);
            BaseAction *move = new MoveCustomer(srcTable, dstTable, customerId);
            move->act(*this);
            actionsLog.push_back(move);

        }
        if (nameOfAction == "closeall") {
            BaseAction *closeall = new CloseAll();
            closeall->act(*this);
            actionsLog.push_back(closeall);
        }
        if (nameOfAction == "menu") {
            BaseAction *menu = new PrintMenu();
            menu->act(*this);
            actionsLog.push_back(menu);
        }
        if (nameOfAction == "status") {
            int tableId = stoi(inputLine.substr(0, inputLine.size()));
            BaseAction *status = new PrintTableStatus(tableId);
            status->act(*this);
            actionsLog.push_back(status);
        }
        if (nameOfAction == "log") {
            BaseAction *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        }
        if (nameOfAction == "backup") {
            BaseAction *backup = new BackupRestaurant();
            backup->act(*this);
            actionsLog.push_back(backup);
        }
        if (nameOfAction == "restore") {
            BaseAction *restore = new RestoreResturant();
            restore->act(*this);
            actionsLog.push_back(restore);
        }

    }


}
int Restaurant::getNumOfTables()const {
    return  tables.size();
}
Table* Restaurant::getTable(int ind) {
    Table* t =  tables[ind];
    return t;
}
vector<Dish>& Restaurant::getMenu() {
    return menu;
}
vector<Table*>& Restaurant::getTables() {
    return tables;
}
const vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;
}

