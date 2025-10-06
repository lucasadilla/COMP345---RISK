
#ifndef COMP345_RISK_ORDERS_H
#define COMP345_RISK_ORDERS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Order {

    protected:
        bool* executed;
        string* effect;

    public:
        Order();
        Order(const Order& other);
        virtual ~Order();

        Order& operator=(const Order& other);

        virtual bool validate() =0;
        virtual void execute() =0;
        virtual Order* clone() const =0;
        virtual string getDescription() const = 0;

        bool isExecuted() const;
        string getEffect() const;

        friend ostream& operator<<(ostream& os, const Order& order);
};

class Deploy : public Order {

    private:
        int* armyUnits;
        string* targetTerritory;

    public:
        Deploy();
        Deploy(int armies, const string& territory);
        Deploy(const Deploy& other);
        ~Deploy();

        Deploy& operator=(const Deploy& other);
        bool validate() override;
        void execute() override;
        Order* clone() const override;
        string getDescription() const override;

        int getArmyUnits() const;
        string getTargetTerritory() const;
};

class Advance : public Order {

    private:
        int* armyUnits;
        string* sourceTerritory;
        string* targetTerritory;

    public:
        Advance();
        Advance(int armies,const string& source, const string& target);

        Advance(const Advance& other);
        ~Advance();
        Advance& operator=(const Advance& other);

        bool validate() override;
        void execute() override;
        Order* clone() const override;
        string getDescription() const override;

        int getArmyUnits() const;
        string getSourceTerritory() const;
        string getTargetTerritory() const;
};

class Bomb : public Order {

    private:
        string* targetTerritory;

    public:
        Bomb();
        Bomb(const string& territory);

        Bomb(const Bomb& other);
        ~Bomb();

        Bomb& operator = (const Bomb& other);

        bool validate() override;
        void execute() override;
        Order* clone() const override;
        string getDescription() const override;
        string getTargetTerritory() const;
};

class Blockade : public Order {
    private:
        string* targetTerritory;

    public:
        Blockade();
        Blockade(const string& territory);
        Blockade(const Blockade& other);
        ~Blockade();
        Blockade& operator=(const Blockade& other);
        bool validate() override;
        void execute() override;
        Order* clone() const override;
        string getDescription() const override;
        string getTargetTerritory() const;
};

class Airlift: public Order {

    private:
        int* armyUnits;
        string* sourceTerritory;
        string* targetTerritory;

    public:
        Airlift();
        Airlift(int armies, const string& source, const string& target);
        Airlift(const Airlift& other);
        ~Airlift();
        Airlift& operator=(const Airlift& other);

        bool validate() override;
        void execute() override;
        Order* clone() const override;
        string getDescription() const override;
        int getArmyUnits() const;
        string getSourceTerritory() const;
        string getTargetTerritory() const;
};

class Negotiate : public Order {

    private:
        string* targetPlayer;

    public:
        Negotiate();
        Negotiate(const string& player);
        Negotiate(const Negotiate& other);
        ~Negotiate();

        Negotiate& operator=(const Negotiate& other);
        bool validate() override;
        void execute() override;
        Order* clone() const override;
        string getDescription() const override;
        string getTargetPlayer() const;
};

class OrdersList {

    private:
        vector<Order*>* orders;

    public:
        OrdersList();
        OrdersList(const OrdersList& other);
        ~OrdersList();
        OrdersList& operator=(const OrdersList& other);

        void addOrder(Order* order);
        bool remove(int index);
        bool move(int fromIndex, int toIndex);
        Order* getOrder(int index) const;

        int size() const;
        bool empty() const;
        void executeAll();
        friend ostream& operator<<(ostream& os, const OrdersList& orders);
};

void testOrdersLists();
#endif


