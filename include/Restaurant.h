#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant();
    virtual ~Restaurant();//destructor
    Restaurant(const Restaurant & other);//copy constructor
    Restaurant & operator=(const Restaurant & other);// assignment operator
    Restaurant(Restaurant && other) ;//move constructor (noexcept)
    Restaurant & operator=(Restaurant && other);//move assignment operator
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    std::vector<Table*>& getTables();
    void clear1();
    void copy(const Restaurant &other);
    void steal( Restaurant &other);

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif