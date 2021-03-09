#include "Table.h"
#include <fstream>
#include <string>
#include <iostream>
#include<vector>


using namespace std;


Table::Table(int t_capacity):capacity(t_capacity),open(false){
    customersList=vector<Customer*>();
    orderList=vector<OrderPair>();
}

Table::~Table() {//destructor
    orderList.clear();
   for (unsigned int i=0;i<customersList.size();i=i+1){
       delete(customersList[i]);
   }
    customersList.clear();
}
Table:: Table(const Table &other){//copy constructor
    capacity=other.capacity;
    open=other.open;
    customersList=vector<Customer*>();
    orderList=vector<OrderPair>();
    for(unsigned int i=0;i<other.orderList.size();i++ ){
        orderList.push_back(other.orderList[i]);
    }
    for(unsigned int i=0;i<other.customersList.size();i++) {
        customersList.push_back(other.customersList[i]->clone());//know which type of customer at run time
    }
}
Table &Table :: operator=(const Table & other){
    if(&other != this){
        this->capacity=other.capacity;
        this->open=other.open;
        this->orderList.clear();
        for(unsigned int i=0;i<other.orderList.size();i++ ){
            orderList.push_back(other.orderList[i]);
        }
        for (unsigned int i=0;i<customersList.size();i=i+1){
            delete customersList[i];
        }
        customersList.clear();
        for(unsigned int i=0;i<other.customersList.size();i++) {
            customersList.push_back(other.customersList[i]->clone());//know which type of customer at run time
        }
    }

    return *this;

}
Table::Table(Table &&other) {
    this->capacity=other.capacity;
    this->open=other.open;
    for(unsigned int i=0;i<other.orderList.size();i++ ){
        orderList.push_back(other.orderList[i]);
    }
    for(unsigned int i=0; i<other.customersList.size();i++){//stealing other's customers
        this->customersList.push_back(other.customersList[i]);
        other.customersList[i]= nullptr;
    }
    other.customersList.clear();

}
Table &Table ::operator=(Table &&other) {
    if(&other != this) {
        this->capacity = other.capacity;
        this->open = other.open;
        this->orderList.clear();
        for (unsigned int i = 0; i < other.orderList.size(); i++) {
            orderList.push_back(other.orderList[i]);
        }
        for (unsigned int i=0;i<customersList.size();i=i+1){
            delete(customersList[i]);
        }
        customersList.clear();
        for (unsigned int i = 0; i < other.customersList.size(); i++) {//stealing other's customers
            this->customersList.push_back(other.customersList[i]);
            other.customersList[i] = nullptr;
        }
        other.customersList.clear();
    }
    return *this;
    }

int Table:: getCapacity() const {
    return capacity;
}
Dish Table::getDish(int id, const vector<Dish>& menu) {

    int index;
    for(unsigned int i=0;i<menu.size();i++){
        if(menu[i].getId()==id){
            index=i;
        }
    }
    Dish dish = menu[index];
    return dish;//the wanted Dish
}

Customer* Table::getCustomer(int id) {
    int index;
    int w;
    for(unsigned int i=0;i<customersList.size();i=i+1){
        w = customersList[i]->getId();
        if(w==id){
            index=i;
        }
    }
    return customersList[index];
}


void Table::addCustomer(Customer* customer) {
    if(customer!=nullptr)
        customersList.push_back(customer);//make sure that someone not delete it

}
void Table::removeCustomer(int id){// check if input valid
    int removeValue;
   for(unsigned int i =0;i<customersList.size();i++){
       if(customersList[i]->getId()==id){
           removeValue=i;
       }
   }

   customersList.erase(customersList.begin()+removeValue);
 if(customersList.size()==0){//if there is no more customer at the table close the table
       this->open=false;
   }
}




vector<Customer*>& Table:: getCustomers(){ //remember to add & to the func that gets it
    return customersList;
}

vector<OrderPair>& Table::getOrders() {
    return orderList;
}

void Table::order(const vector<Dish> &menu) {
    vector<int> vecOfDId;
    for(unsigned int i=0;i<customersList.size();i++){
        vecOfDId=(customersList[i]->order(menu));
            if(vecOfDId.size()!=0){
                for(unsigned int j = 0; j < vecOfDId.size(); j++){
                    Dish dish=(getDish(vecOfDId[j],menu));
                    int customerID=(customersList[i]->getId());
                    OrderPair current(customerID,dish);
                    orderList.push_back(current);
                    cout<<customersList[i]->getName()+" ordered "+current.second.getName() <<endl;
                }


        }


    }

}
void Table::openTable() {
    open=true;
}
void Table::closeTable(int id) {
    string idd = to_string(id);
    cout << "Table " + idd + " was closed. Bill " + to_string(getBill())+"NIS"<<endl;
    open=false;
    //delete the customers
    for (unsigned int i=0;i<customersList.size();i=i+1){
        delete(customersList[i]);
    }
    customersList.clear();
   //delete customers' orders
    orderList.clear();

}

int Table::getBill() {
    int outPutBill=0;
    for(unsigned int i=0;i<orderList.size();i++){
       outPutBill=outPutBill+ (orderList[i].second).getPrice();
    }
    return outPutBill;
}
bool Table::isOpen() {
    return open;
}
bool Table::isCustomerExist(int id) {
    bool isExsit = false;
    for (unsigned int i = 0; i < customersList.size(); i++) {
        if(customersList[i]->getId() == id)
            isExsit =true;
    }

    return isExsit;
}
void Table::addDish(int id, Dish dish) {
    OrderPair myP(id,dish);
    orderList.push_back(myP);
}
void Table::printTable(int id) {
    string tableStatus;
    string customer ="";
    string order="";
    string getBillS="Current Bill: "+to_string(getBill())+"NIS";
    if(!open){
        cout<<"Table "+to_string(id)+" status: closed"<<endl;
    }
    else {
        tableStatus="Table "+to_string(id)+" status: open";
        for(unsigned int i=0;i<customersList.size();i++){// prints the customers
            customer = customer+to_string(customersList[i]->getId())+" "+customersList[i]->getName()+"\n";
        }
        for(unsigned int i=0;i < orderList.size();i++){
            order = order +((orderList[i].second).getName())+" "+to_string(orderList[i].second.getPrice())+"NIS"+" "+to_string(orderList[i].first)+"\n";
        }
        cout<<tableStatus<<endl;
        cout<<"Customers:"<<endl;
        cout<<customer;
        cout<<"Orders:"<<endl;
        cout<<order;
        cout<<getBillS<<endl;
    }

}
string Table::getCustomerOrder(int custId) const {
    string customerOrder="";
    for(unsigned int i=0;i<orderList.size();i++){
        if(orderList[i].first==custId){
        customerOrder=customerOrder+orderList[i].second.toString2(custId)+"\n";
        }
    }
    return customerOrder;
}


