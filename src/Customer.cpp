# include "Customer.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

Customer ::Customer(string c_name, int c_id) :name(c_name), id(c_id) {}

string Customer :: getName()const{
    return name;
}
int Customer ::getId() const {
    return this->id;
}
Customer::~Customer(){}

VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name,id) {}
string VegetarianCustomer::getType() const {
    return "veg";
}
VegetarianCustomer::~VegetarianCustomer() {}
vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {// return vector of Dish's id

    vector<int> returnVOrder;
    int maxPrice = 0;
    int idMaxPrice = -1;
    bool hasVeg = false;

    for (unsigned int i = 0; i < menu.size(); i++) {
        if (((menu[i].getType()) ==( VEG)) & (hasVeg== false)) {
            hasVeg = true;
            returnVOrder.push_back(menu[i].getId());
        } else if ((menu[i].getType()) == (BVG)) {
            if ((menu[i].getPrice()) >= (maxPrice)) {// we need the most expensive bvg
                if ((menu[i].getPrice()) == (maxPrice)) { //if the price is equal check by id
                    if ((menu[i].getId()) < (idMaxPrice)) {
                        maxPrice = menu[i].getPrice();
                        idMaxPrice = menu[i].getId();
                    }
                } else {
                    maxPrice = menu[i].getPrice();
                    idMaxPrice = menu[i].getId();
                }
            }

        }


    }
    if(hasVeg==true){
        returnVOrder.push_back(idMaxPrice);
    }

    return returnVOrder;
}


string VegetarianCustomer::toString() const {
    return "name:" +getName() + "id:" + to_string(getId());
}
Customer* VegetarianCustomer::clone()  {
    return new VegetarianCustomer(this->getName(),this->getId()) ;
}
CheapCustomer::~CheapCustomer() {}
CheapCustomer::CheapCustomer(string name, int id): Customer(name,id),alredyOrder(false) {}
string CheapCustomer::getType() const {
    return "chp";
}

vector<int> CheapCustomer::order(const vector<Dish> &menu) {
    vector<int> returnOrder;
    if(menu.size() != 0) {
        if (alredyOrder == false) {// can only order once!
            int minPrice = menu[0].getPrice();
            int idMinPrice = menu[0].getId();
            for (unsigned int i = 1; i < menu.size(); i++) {
                if ((menu[i].getPrice()) <= (minPrice)) {
                    if ((menu[i].getPrice()) == (minPrice)) { //if the price is equal check by id
                        if ((menu[i].getId()) < (idMinPrice)) {
                            minPrice = menu[i].getPrice();
                            idMinPrice = menu[i].getId();
                        }
                    } else {
                        minPrice = menu[i].getPrice();
                        idMinPrice = menu[i].getId();
                    }

                }
            }

            returnOrder.push_back(idMinPrice);
            alredyOrder = true;
        }
    }

    return returnOrder;
}
    string CheapCustomer::toString() const {
        return "name:" + getName() + "id:" + to_string(getId());
    }
Customer* CheapCustomer::clone() {
    CheapCustomer* output = new CheapCustomer(this->getName(),this->getId());
    output->alredyOrder=this->alredyOrder;
    return output;
}
SpicyCustomer::SpicyCustomer(string name, int id): Customer(name,id),alredyOrderDish(false){}
SpicyCustomer::~SpicyCustomer(){}
string SpicyCustomer::getType() const {
    return "spc";
}
vector<int> SpicyCustomer::order(const vector<Dish> &menu) {
    vector<int> sOrder;
    if(alredyOrderDish== false) {
        int dMaxPrice = 0;
        int dIdMaxPrice=-1;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if ((menu[i].getType()) ==( SPC)) {//chek if the dish is spicy
                alredyOrderDish = true;
                if ((menu[i].getPrice()) >= (dMaxPrice)) {//check the price
                    if((menu[i].getPrice()) == (dMaxPrice)){
                        if((menu[i].getId())< (dIdMaxPrice)){//if the price is equal check by id
                            dMaxPrice = menu[i].getPrice();
                            dIdMaxPrice = menu[i].getId();
                        }
                    }
                    else{
                        dMaxPrice = menu[i].getPrice();
                        dIdMaxPrice = menu[i].getId();
                    }
                }
            }
        }
        if(dIdMaxPrice != -1){
            sOrder.push_back(dIdMaxPrice);
        }


    }
    else{
        int bMinPrice=INT8_MAX;
        int bMinId=-1;

        for(unsigned int i = 0; i < menu.size(); i++){
            if ((menu[i].getType()) == (BVG)) {
                if ( (menu[i].getPrice() )<= (bMinPrice)) {
                    if((menu[i].getPrice() )== (bMinPrice)){
                        if((menu[i].getId())< (bMinId)) {
                            bMinPrice = menu[i].getPrice();
                            bMinId = menu[i].getId();
                        }
                    }
                    else{
                        bMinPrice = menu[i].getPrice();
                        bMinId = menu[i].getId();
                    }
                }
            }
        }
        sOrder.push_back(bMinId);
    }


    return sOrder;

}

string SpicyCustomer::toString() const {
    return "name:" + getName() + "id:" + to_string(getId());
}
Customer* SpicyCustomer::clone(){
    SpicyCustomer* output= new SpicyCustomer(getName(),getId());
    output->alredyOrderDish=alredyOrderDish;
    return output;
}
AlchoholicCustomer::~AlchoholicCustomer() {}
AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name,id), lastOrderPrice(0),lastOrderId(0){}
string AlchoholicCustomer::getType() const {
    return "alc";
}
vector<int> AlchoholicCustomer::order(const vector<Dish> &menu) {
    vector<int> alcoOrder;
    int minPrice =INT8_MAX;//THE MAX VALUE FOR INT
    int minPriceId;
    for(unsigned int i=0;i<menu.size();i++){
        if((menu[i].getType())==ALC){
             if(((menu[i].getPrice()) <= minPrice) & ((menu[i].getPrice())>=lastOrderPrice)){
                if(((menu[i].getPrice())==lastOrderPrice )& ((menu[i].getPrice())==minPrice)){ //both equal
                    if(((menu[i].getId())<minPriceId) & ((menu[i].getId()) > lastOrderId)){
                        minPrice=menu[i].getPrice();
                        minPriceId=menu[i].getId();
                    }
                }
                else if(((menu[i].getPrice()) > lastOrderPrice) &( (menu[i].getPrice())==minPrice)){
                    if((menu[i].getId() )< minPriceId){// check who have the min id
                        minPrice=menu[i].getPrice();
                        minPriceId=menu[i].getId();
                    }
                }
                else if(((menu[i].getPrice()) == lastOrderPrice) & ((menu[i].getPrice()) < minPrice) ){
                    if((menu[i].getId()) > lastOrderId){
                        minPrice=menu[i].getPrice();
                        minPriceId=menu[i].getId();
                    }

                }
                else if (((menu[i].getPrice() )> lastOrderPrice) &( (menu[i].getPrice()) < minPrice)){
                    minPrice=menu[i].getPrice();
                    minPriceId=menu[i].getId();
                }
            }
        }
    }
    if(minPrice != INT8_MAX){ // check that a ALC was found, if not return a empty vector
        lastOrderPrice = minPrice;
        lastOrderId = minPriceId;
        alcoOrder.push_back(minPriceId);
    }
    return alcoOrder;
}


string AlchoholicCustomer::toString() const {
    return "name:" + getName() + "id:" + to_string(getId());
}
Customer* AlchoholicCustomer::clone() {
    AlchoholicCustomer* output =new AlchoholicCustomer(getName(),getId());
    output->lastOrderId=lastOrderId;
    output->lastOrderPrice=lastOrderPrice;
    return output;
}


