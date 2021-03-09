#include "Action.h"
#include "Restaurant.h"
#include <iostream>

using namespace std;
extern Restaurant* backup;


BaseAction::BaseAction() {
    status = PENDING;
    errorMsg = " ";
}
ActionStatus BaseAction::getStatus() const {
    return status;
}

BaseAction::~BaseAction() = default;// no need to implement//??

void BaseAction::complete() {
    status=COMPLETED;
}
void BaseAction::error(string errorMsg) {
    cout<<"Error: "+ errorMsg <<endl;
    status=ERROR;
}
string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::copyStatus(const BaseAction &action) {
    this->status=action.status;
    this->errorMsg=action.errorMsg;
}


OpenTable::OpenTable(int id, vector<Customer*> &customersList) : BaseAction(),tableId(id),customers(customersList),customerS("") {//this how we need to do at the vectors
    responsibleOfMEMO = true;
    for (unsigned int i = 0; i < customers.size(); i++) {
        customerS = customerS + customers[i]->getName() + "," + customers[i]->getType() + " ";
    }
}
OpenTable::~OpenTable() {// destructor
    if(responsibleOfMEMO) {
        clear1();
    }
}
OpenTable::OpenTable(OpenTable &other): tableId(other.tableId) {
    copy(other);
}
OpenTable& OpenTable ::operator=(OpenTable &other) {
    if(&other != this) {
        clear1();
        copy(other);
    }
    return *this;
}
OpenTable::OpenTable(OpenTable &&other): tableId(other.tableId) {

    for(unsigned int i=0;i<other.customers.size();i++){//stealing other's customers
        this->customers.push_back(other.customers[i]);
        other.customers[i]= nullptr;
    }
    other.customers.clear();
}
OpenTable& OpenTable ::operator=(OpenTable &&other){
    this->clear1();
    for(unsigned int i=0;i<other.customers.size();i++){//stealing other's customers
        this->customers.push_back(other.customers[i]);
        other.customers[i]= nullptr;
    }
    other.customers.clear();
    return *this;
}
void OpenTable::clear1(){

    for(unsigned int i =0; i< customers.size();i++){
        delete(customers[i]);
    }
    customers.clear();
}
void OpenTable::copy(const OpenTable &other) {
    customerS= other.customerS;

    for(unsigned int i =0; i< customers.size();i++) {
        this->customers.push_back(other.customers[i]);
    }
    copyStatus(other);
    this->responsibleOfMEMO=other.responsibleOfMEMO;
}

void OpenTable::act(Restaurant &restaurant) {

    if ((restaurant.getTable(tableId))->isOpen() ||
        restaurant.getTable(tableId) == nullptr) {//check if the table is already open or not exist
        error("Table does not exist or is already open");
        for (auto *Customer : customers) {
            delete (Customer);
        }
        customers.clear();

    } else {
        if ((restaurant.getNumOfTables()) > (tableId)) {
            responsibleOfMEMO = false;
            restaurant.getTable(tableId)->openTable();
            for (unsigned int i = 0; i < customers.size(); i++) {
                (restaurant.getTable(tableId))->addCustomer(customers[i]);
            }
            complete();
        }
    }
}
    string OpenTable::toString() const {
        if (getStatus() == ERROR) {
            return "open " + to_string(tableId) + " " + customerS + "Error: Table does not exist or is already open";
        } else {
            return "open " + to_string(tableId) + " " + customerS + "Completed";
        }
    }

    BaseAction* OpenTable::clone() {
    OpenTable* output = new OpenTable(*this);
        return  output;

}


Order::Order(int id):BaseAction(), tableId(id){}

void Order::act(Restaurant &restaurant) {
    if (restaurant.getTable(tableId) == nullptr || (restaurant.getTable(tableId))->isOpen() == false) {//check if the table is not open or not exist
        error("Table does not exist or is not open");
    } else {
        (restaurant.getTable(tableId))->order(restaurant.getMenu());
        complete();
    }


}
string Order::toString() const {
    if (getStatus() == ERROR) {
        return "order " + to_string(tableId) + " Error: Table does not exist or is not open";
    } else {
        return "order " + to_string(tableId) + " Completed";
    }
}
BaseAction* Order::clone() {
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst), id(customerId) {}
void MoveCustomer::act(Restaurant &restaurant) {
    bool errorB = false;

    if (!((restaurant.getTable(dstTable))->isOpen()) || restaurant.getTable(dstTable) == nullptr) {
        errorB = true;
    }
    if (!((restaurant.getTable(srcTable))->isOpen()) || restaurant.getTable(srcTable) == nullptr) {
        errorB = true;
    }
    if (restaurant.getTable(srcTable)->isCustomerExist(id) == false) {
        errorB = true;
    }
    if ((restaurant.getTable(dstTable)->getCustomers().size()) == (unsigned(restaurant.getTable(dstTable)->getCapacity()))) {
        errorB = true;
    }

    if (errorB == true) {
        error("Cannot move customer");
    } else {
        vector<OrderPair> newSrcVec;
        restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));//adds customer to dstTable
        for (unsigned int i = 0; i < restaurant.getTable(srcTable)->getOrders().size(); i++) {
            if ((restaurant.getTable(srcTable)->getOrders()[i].first) == (id)) {
                (restaurant.getTable(dstTable))->addDish(id, restaurant.getTable(
                        srcTable)->getOrders()[i].second);//adds the order to dstTable
            }
            else {
                newSrcVec.push_back(restaurant.getTable(srcTable)->getOrders()[i]);

            }
        }
            restaurant.getTable(srcTable)->getOrders().clear();
            for(const auto  &OrderPair: newSrcVec){
                restaurant.getTable(srcTable)->getOrders().push_back(OrderPair);

            }
            restaurant.getTable(srcTable)->removeCustomer(id);




        complete();
    }
}
    string MoveCustomer::toString() const {
    string idd = to_string(id);
        if(getStatus()==ERROR){
            return "move " + to_string(srcTable)+" "+to_string(dstTable)+" "+idd+ " Error: Cannot move customer";
        }
        else {
            return "move " + to_string(srcTable)+" "+to_string(dstTable)+" "+idd + " Completed";
        }
    }
BaseAction* MoveCustomer::clone() {
    return new MoveCustomer(*this);
}



Close::Close(int id):BaseAction(), tableId(id) {}
void Close :: act(Restaurant &restaurant) {
    if ((restaurant.getTable(tableId) == nullptr) ||(((restaurant.getTable(tableId))->isOpen()) == false)){
        //check if the table not exist or is not open
        error("Table does not exist or is not open");
    } else {
        restaurant.getTable(tableId)->closeTable(tableId);
        complete();
    }

}
string Close::toString() const {

    if(getStatus()==ERROR){
        return "close "+to_string(tableId)+" Error: Table does not exist or is not open";
    }
    else {
        return "close " + to_string(tableId) + " Completed";
    }
}
BaseAction* Close::clone() {
    return new Close(*this);
}


CloseAll::CloseAll():BaseAction(){}
void CloseAll::act(Restaurant &restaurant) {
    for(unsigned int i=0;i<restaurant.getTables().size();i++){
        if(restaurant.getTables()[i]->isOpen()){
            restaurant.getTables()[i]->closeTable(i);
        }
    }
    complete();
}

string CloseAll::toString() const {
        return "closeall Completed";

}
BaseAction* CloseAll::clone() {
    return new CloseAll(*this);
}


PrintMenu::PrintMenu() :BaseAction(){}
void PrintMenu::act(Restaurant &restaurant) {
    for(unsigned int i=0;i<restaurant.getMenu().size();i++){
    cout<<(restaurant.getMenu()[i]).toString()<<endl;
    }
    complete();

}
string PrintMenu::toString() const {

    return "menu Completed";
}
BaseAction* PrintMenu::clone() {
    return new PrintMenu(*this);
}



PrintTableStatus::PrintTableStatus(int id) :BaseAction(), tableId(id){}
void PrintTableStatus::act(Restaurant &restaurant) {
    (restaurant.getTables()[tableId])->printTable(tableId);
    complete();
}
string PrintTableStatus ::toString() const {
    return "status "+ to_string(tableId)+ " Completed";
}
BaseAction* PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}



PrintActionsLog::PrintActionsLog():BaseAction() {}
void PrintActionsLog::act(Restaurant &restaurant) {
    for(unsigned int i = 0; i<restaurant.getActionsLog().size();i++){
        cout<<restaurant.getActionsLog()[i]->toString()<<endl;
    }
}
string PrintActionsLog::toString() const {
    return "log Completed";
}
BaseAction* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}


BackupRestaurant::BackupRestaurant(): BaseAction() {}
void BackupRestaurant::act(Restaurant &restaurant) {
    if (backup) {
        *backup = restaurant;
    }
    else {
        backup = new Restaurant(restaurant);
    }
    complete();
}
string BackupRestaurant::toString() const {
    return "backup Completed";
}
BaseAction* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}


RestoreResturant::RestoreResturant():BaseAction() {}
void RestoreResturant::act(Restaurant &restaurant) {
    if (backup == nullptr) {
        error("No backup available");
    } else {
        restaurant = *backup;
        complete();
    }

}
string RestoreResturant::toString() const {
    if(getStatus()==ERROR)
    return "restore Error: No backup available";
    else{
        return "restore Completed";
    };
}
BaseAction* RestoreResturant::clone() {
    return new RestoreResturant(*this);
}













