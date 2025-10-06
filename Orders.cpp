

#include "Orders.h"
#include <sstream>


//default constructor for Order
Order::Order() {
    executed = new bool(false);
    effect = new std::string("");
}

//copy constructor for Order
Order::Order(const Order& other) {
    executed = new bool(*(other.executed));
    effect = new std::string(*(other.effect));
}

//destructor for Order
Order::~Order() {
    delete executed;
    delete effect;
}

//assignment operator for Order
Order& Order::operator=(const Order& other) {
    if (this != &other) {
        delete executed;
        delete effect;

        executed = new bool(*(other.executed));
        effect = new std::string(*(other.effect));
    }
    return *this;
}

//check if order has been executed
bool Order::isExecuted() const {
    return *executed;
}

//get the effect of the order
std::string Order::getEffect() const {
    return *effect;
}

//stream insertion operator for Order
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.getDescription();
    if (order.isExecuted() && !order.getEffect().empty()) {
        os << " - Effect: " << order.getEffect();
    }
    return os;
}




// DEPLOY ORDER IMPLEMENTATION

//default constructor for Deploy
Deploy::Deploy() : Order() {
    armyUnits = new int(0);
    targetTerritory = new std::string("");
}

//parameterized constructor for Deploy
Deploy::Deploy(int armies, const std::string& territory) : Order() {
    armyUnits = new int(armies);
    targetTerritory = new std::string(territory);
}

//copy constructor for Deploy
Deploy::Deploy(const Deploy& other) : Order(other) {
    armyUnits = new int(*(other.armyUnits));
    targetTerritory = new std::string(*(other.targetTerritory));
}

//destructor for Deploy
Deploy::~Deploy() {
    delete armyUnits;
    delete targetTerritory;
}

//assignment operator for Deploy
Deploy& Deploy::operator=(const Deploy& other) {
    if (this != &other) {
        Order::operator=(other);

        delete armyUnits;
        delete targetTerritory;

        armyUnits = new int(*(other.armyUnits));
        targetTerritory = new std::string(*(other.targetTerritory));
    }
    return *this;
}

//validate Deploy order, basic validation
bool Deploy::validate() {
    if (*armyUnits <= 0) return false;        // must deploy positive armies
    if (targetTerritory->empty()) return false; // must specify target territory
    return true;
}

//execute Deploy order
void Deploy::execute() {
    if (validate()) {
        std::ostringstream oss;
        oss << "Deployed " << *armyUnits << " army units to " << *targetTerritory;
        *effect = oss.str();
        *executed = true;
    } else {
        *effect = "Deploy order is invalid and was not executed";
        *executed = true;
    }
}

//clone Deploy order for deep copying
Order* Deploy::clone() const {
    return new Deploy(*this);
}

//get description of Deploy order
std::string Deploy::getDescription() const {
    std::ostringstream oss;
    oss << "Deploy Order: " << *armyUnits << " army units to " << *targetTerritory;
    return oss.str();
}

//get army units for Deploy order
int Deploy::getArmyUnits() const {
    return *armyUnits;
}

//get target territory for Deploy order
std::string Deploy::getTargetTerritory() const {
    return *targetTerritory;
}




// ADVANCE ORDER IMPLEMENTATION

//default constructor for Advance
Advance::Advance() : Order() {
    armyUnits = new int(0);
    sourceTerritory = new std::string("");
    targetTerritory = new std::string("");
}

//parameterized constructor for Advance
Advance::Advance(int armies, const std::string& source, const std::string& target) : Order() {
    armyUnits = new int(armies);
    sourceTerritory = new std::string(source);
    targetTerritory = new std::string(target);
}

//copy constructor for Advance
Advance::Advance(const Advance& other) : Order(other) {
    armyUnits = new int(*(other.armyUnits));
    sourceTerritory = new std::string(*(other.sourceTerritory));
    targetTerritory = new std::string(*(other.targetTerritory));
}

//destructor for Advance
Advance::~Advance() {
    delete armyUnits;
    delete sourceTerritory;
    delete targetTerritory;
}

//assignment operator for Advance
Advance& Advance::operator=(const Advance& other) {
    if (this != &other) {
        Order::operator=(other);

        delete armyUnits;
        delete sourceTerritory;
        delete targetTerritory;

        armyUnits = new int(*(other.armyUnits));
        sourceTerritory = new std::string(*(other.sourceTerritory));
        targetTerritory = new std::string(*(other.targetTerritory));
    }
    return *this;
}

//validate Advance order
bool Advance::validate() {
    if (*armyUnits <= 0) return false;
    if (sourceTerritory->empty() || targetTerritory->empty()) return false;
    if (*sourceTerritory == *targetTerritory) return false;
    return true;
}

//execute Advance order
void Advance::execute() {
    if (validate()) {
        std::ostringstream oss;
        oss << "Advanced " << *armyUnits << " army units from " << *sourceTerritory
            << " to " << *targetTerritory;
        *effect = oss.str();
        *executed = true;
    } else {
        *effect = "Advance order is invalid and was not executed";
        *executed = true;
    }
}

//clone Advance order
Order* Advance::clone() const {
    return new Advance(*this);
}

//get description of Advance order
std::string Advance::getDescription() const {
    std::ostringstream oss;
    oss << "Advance Order: " << *armyUnits << " army units from " << *sourceTerritory
        << " to " << *targetTerritory;
    return oss.str();
}

//getters of Advance
int Advance::getArmyUnits() const { return *armyUnits; }
std::string Advance::getSourceTerritory() const { return *sourceTerritory; }
std::string Advance::getTargetTerritory() const { return *targetTerritory; }




// BOMB ORDER IMPLEMENTATION

//default constructor for Bomb
Bomb::Bomb() : Order() {
    targetTerritory = new std::string("");
}

//parameterized constructor for Bomb
Bomb::Bomb(const std::string& territory) : Order() {
    targetTerritory = new std::string(territory);
}

//copy constructor for Bomb
Bomb::Bomb(const Bomb& other) : Order(other) {
    targetTerritory = new std::string(*(other.targetTerritory));
}

//destructor for Bomb
Bomb::~Bomb() {
    delete targetTerritory;
}

//assignment operator for Bomb
Bomb& Bomb::operator=(const Bomb& other) {
    if (this != &other) {
        Order::operator=(other);
        delete targetTerritory;
        targetTerritory = new std::string(*(other.targetTerritory));
    }
    return *this;
}

//validate Bomb order
bool Bomb::validate() {
    if (targetTerritory->empty()) return false;
    return true;
}

//execute Bomb order
void Bomb::execute() {
    if (validate()) {
        std::ostringstream oss;
        oss << "Bombed territory " << *targetTerritory << ", destroying half of its army units";
        *effect = oss.str();
        *executed = true;
    } else {
        *effect = "Bomb order is invalid and was not executed";
        *executed = true;
    }
}

//clone Bomb order
Order* Bomb::clone() const {
    return new Bomb(*this);
}

//get description of Bomb order
std::string Bomb::getDescription() const {
    std::ostringstream oss;
    oss << "Bomb Order: target territory " << *targetTerritory;
    return oss.str();
}

//getters for Bomb
std::string Bomb::getTargetTerritory() const { return *targetTerritory; }




// BLOCKADE ORDER IMPLEMENTATION

Blockade::Blockade() : Order() {
    targetTerritory = new std::string("");
}

//parameterized constructor for Blockade
Blockade::Blockade(const std::string& territory) : Order() {
    targetTerritory = new std::string(territory);
}

//copy constructor for Blockade
Blockade::Blockade(const Blockade& other) : Order(other) {
    targetTerritory = new std::string(*(other.targetTerritory));
}

//destructor for Blockade
Blockade::~Blockade() {
    delete targetTerritory;
}

//assignment operator for Blockade
Blockade& Blockade::operator=(const Blockade& other) {
    if (this != &other) {
        Order::operator=(other);
        delete targetTerritory;
        targetTerritory = new std::string(*(other.targetTerritory));
    }
    return *this;
}

//validate Blockade order
bool Blockade::validate() {
    if (targetTerritory->empty()) return false;
    return true;
}

//execute Blockade order
void Blockade::execute() {
    if (validate()) {
        std::ostringstream oss;
        oss << "Blockaded territory " << *targetTerritory
            << ", tripled army units and made it neutral";
        *effect = oss.str();
        *executed = true;
    } else {
        *effect = "Blockade order is invalid and was not executed";
        *executed = true;
    }
}

//clone Blockade order
Order* Blockade::clone() const {
    return new Blockade(*this);
}

//get description of Blockade order
std::string Blockade::getDescription() const {
    std::ostringstream oss;
    oss << "Blockade Order: target territory " << *targetTerritory;
    return oss.str();
}

//getters for Blockade
std::string Blockade::getTargetTerritory() const { return *targetTerritory; }




// AIRLIFT ORDER IMPLEMENTATION

//default constructor for Airlift
Airlift::Airlift() : Order() {
    armyUnits = new int(0);
    sourceTerritory = new std::string("");
    targetTerritory = new std::string("");
}

//parameterized constructor for Airlift
Airlift::Airlift(int armies, const std::string& source, const std::string& target) : Order() {
    armyUnits = new int(armies);
    sourceTerritory = new std::string(source);
    targetTerritory = new std::string(target);
}

//copy constructor for Airlift
Airlift::Airlift(const Airlift& other) : Order(other) {
    armyUnits = new int(*(other.armyUnits));
    sourceTerritory = new std::string(*(other.sourceTerritory));
    targetTerritory = new std::string(*(other.targetTerritory));
}

//destructor for Airlift
Airlift::~Airlift() {
    delete armyUnits;
    delete sourceTerritory;
    delete targetTerritory;
}

//assignment operator for Airlift
Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);

        delete armyUnits;
        delete sourceTerritory;
        delete targetTerritory;

        armyUnits = new int(*(other.armyUnits));
        sourceTerritory = new std::string(*(other.sourceTerritory));
        targetTerritory = new std::string(*(other.targetTerritory));
    }
    return *this;
}

//validate Airlift order
bool Airlift::validate() {
    if (*armyUnits <= 0) return false;
    if (sourceTerritory->empty() || targetTerritory->empty()) return false;
    if (*sourceTerritory == *targetTerritory) return false;
    return true;
}

//execute Airlift order
void Airlift::execute() {
    if (validate()) {
        std::ostringstream oss;
        oss << "Airlifted " << *armyUnits << " army units from " << *sourceTerritory
            << " to " << *targetTerritory;
        *effect = oss.str();
        *executed = true;
    } else {
        *effect = "Airlift order is invalid and was not executed";
        *executed = true;
    }
}

//clone Airlift order
Order* Airlift::clone() const {
    return new Airlift(*this);
}

//get description of Airlift order
std::string Airlift::getDescription() const {
    std::ostringstream oss;
    oss << "Airlift Order: " << *armyUnits << " army units from " << *sourceTerritory
        << " to " << *targetTerritory;
    return oss.str();
}

//getters for Airlift
int Airlift::getArmyUnits() const { return *armyUnits; }
std::string Airlift::getSourceTerritory() const { return *sourceTerritory; }
std::string Airlift::getTargetTerritory() const { return *targetTerritory; }




// NEGOTIATE ORDER IMPLEMENTATION

//default constructor for Negotiate
Negotiate::Negotiate() : Order() {
    targetPlayer = new std::string("");
}

//parameterized constructor for Negotiate
Negotiate::Negotiate(const std::string& player) : Order() {
    targetPlayer = new std::string(player);
}

//copy constructor for Negotiate
Negotiate::Negotiate(const Negotiate& other) : Order(other) {
    targetPlayer = new std::string(*(other.targetPlayer));
}

//destructor for Negotiate
Negotiate::~Negotiate() {
    delete targetPlayer;
}

// assignment operator for Negotiate
Negotiate& Negotiate::operator=(const Negotiate& other) {
    if (this != &other) {
        Order::operator=(other);
        delete targetPlayer;
        targetPlayer = new std::string(*(other.targetPlayer));
    }
    return *this;
}


//validate Negotiate order
bool Negotiate::validate() {
    if (targetPlayer->empty()) return false;
    return true;
}

//execute Negotiate order
void Negotiate::execute() {
    if (validate()) {
        std::ostringstream oss;
        oss << "Negotiated peace with player " << *targetPlayer << " until end of turn";
        *effect = oss.str();
        *executed = true;
    } else {
        *effect = "Negotiate order is invalid and was not executed";
        *executed = true;
    }
}

//clone Negotiate order
Order* Negotiate::clone() const {
    return new Negotiate(*this);
}

//get description of Negotiate order
std::string Negotiate::getDescription() const {
    std::ostringstream oss;
    oss << "Negotiate Order: with player " << *targetPlayer;
    return oss.str();
}

//getters for Negotiate
std::string Negotiate::getTargetPlayer() const { return *targetPlayer; }




// ORDERSLIST IMPLEMENTATION


//default constructor for OrdersList
OrdersList::OrdersList() {
    orders = new std::vector<Order*>();
}

//copy constructor for OrdersList
OrdersList::OrdersList(const OrdersList& other) {
    orders = new std::vector<Order*>();
    for (Order* order : *(other.orders)) {
        orders-> push_back(order->clone());
    }
}

//destructor for OrdersList
OrdersList::~OrdersList() {
    for (Order* order : *orders) {
        delete order;
    }
    delete orders;
}

//assignment operator for OrdersList
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        // clean up existing orders
        for (Order* order : *orders) {
            delete order;
        }
        orders->clear();

        // deep copy from other
        for (Order* order : *(other.orders)) {
            orders->push_back(order->clone());
        }
    }
    return *this;
}

//add order to the list
void OrdersList::addOrder(Order* order) {
    if (order != nullptr) {
        orders->push_back(order);
    }
}

//remove order at specified index
bool OrdersList::remove(int index) {
    if (index >= 0 && index < static_cast<int>(orders->size())) {
        delete (*orders)[index];
        orders->erase(orders->begin() + index);
        return true;
    }
    return false;
}

//move order from one position to another
bool OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < static_cast<int>(orders->size()) &&
        toIndex >= 0 && toIndex < static_cast<int>(orders->size()) &&
        fromIndex != toIndex) {

        Order* order = (*orders)[fromIndex];
        orders->erase(orders->begin() + fromIndex);

        if (fromIndex < toIndex) {
            toIndex--;
        }

        orders->insert(orders->begin() + toIndex, order);
        return true;
    }
    return false;
}

//get order at specified index
Order* OrdersList::getOrder(int index) const {
    if (index >= 0 && index < static_cast<int>(orders->size())) {
        return (*orders)[index];
    }
    return nullptr;
}

//get number of orders in list
int OrdersList::size() const {
    return static_cast<int>(orders->size());
}

//check if list is empty
bool OrdersList::empty() const {
    return orders->empty();
}

//execute all orders in the list
void OrdersList::executeAll() {
    for (Order* order : *orders) {
        if (order && !order->isExecuted()) {
            order->execute();
        }
    }
}

//stream insertion operator for OrdersList
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    os << "OrdersList (" << ordersList.size() << " orders):\n";
    for (int i = 0; i < ordersList.size(); ++i) {
        os << "  " << i + 1 << ". " << *(ordersList.getOrder(i)) << "\n";
    }
    return os;
}