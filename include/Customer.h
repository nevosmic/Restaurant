#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
using namespace std;
class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual std:: string getType()const=0;
    virtual Customer* clone() =0;
    virtual ~Customer();
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std:: string getType()const;
    Customer* clone();
    ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std:: string getType()const;
    Customer* clone();
    ~CheapCustomer();
private:
    bool alredyOrder;
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std:: string getType()const;
    Customer* clone();
    ~SpicyCustomer();
private:
    bool alredyOrderDish;
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std:: string getType()const;
    Customer* clone();
    ~AlchoholicCustomer();
private:
    int lastOrderPrice;
    int lastOrderId;
};


#endif
