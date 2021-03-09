#ifndef TABLE_H_
#define TABLE_H_


#include <vector>
#include "Customer.h"
#include "Dish.h"
using namespace std;

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    virtual ~Table();//destructor
    Table(const Table & other);//copy constructor
    Table & operator=(const Table & other);// assignment operator
    Table(Table && other) ;//move constructor (noexcept)
    Table & operator=(Table && other);//move assignment operator

    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    void addDish(int id,Dish dish);
    Customer* getCustomer(int id);
    vector<Customer*>& getCustomers();
    vector<OrderPair>& getOrders();
    void order(const vector<Dish> &menu);
    void openTable();
    void closeTable(int id);
    int getBill();
    bool isOpen();
    Dish getDish(int id,const vector<Dish> &menu);
    bool isCustomerExist(int id);
    void printTable(int id);
    string getCustomerOrder(int id) const;
   // Table* clone();

private:
    int capacity;
    bool open;
    vector<Customer*> customersList;
    vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif