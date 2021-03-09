#include "Dish.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
//construct
Dish::Dish(int d_id, string d_name, int d_price, DishType d_type):id(d_id),name(d_name),price(d_price),type(d_type){}


int Dish::getId()const{
    return  id;
}
string Dish::getName()const{
    return  name;
}
int Dish::getPrice()const{
    return  price;
}
 DishType Dish ::getType()const{
    return type;
}
string Dish::toString()const{
    return getName()+" "+typeString()+" "+to_string(getPrice())+"NIS";
}
string Dish::toString2(int id) const {
    return getName()+" " +to_string(getPrice())+"NIS "+to_string(id);
}
string Dish::typeString() const {
    string output="";
    if(type==VEG)
        output=output+"VEG";
    else if(type==BVG)
        output=output+"BVG";
    else if(type==ALC)
        output=output+"ALC";
    else
        output=output+"SPC";
    return output;

}

