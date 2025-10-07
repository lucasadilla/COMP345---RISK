//
// Created by Rahath on 2025-09-09.
//

#include "Orders.h"
#include <iostream>


void testOrdersLists() {
    std::cout << "========== Testing Orders List ==========\n\n";

    // create orders
    std::cout << "Creating orders of every kind...\n";
    Deploy* deploy = new Deploy(5, "Territory1");
    Advance* advance = new Advance(3, "Territory1", "Territory2");
    Bomb* bomb = new Bomb("EnemyTerritory");
    Blockade* blockade = new Blockade("Territory3");
    Airlift* airlift = new Airlift(4, "Territory4", "Territory5");
    Negotiate* negotiate = new Negotiate("PlayerB");

    std::cout << "Deploy: " << *deploy << "\n";
    std::cout << "Advance: " << *advance << "\n";
    std::cout << "Bomb: " << *bomb << "\n";
    std::cout << "Blockade: " << *blockade << "\n";
    std::cout << "Airlift: " << *airlift << "\n";
    std::cout << "Negotiate: " << *negotiate << "\n\n";

    // create OrdersList and add all orders
    std::cout << "Creating OrdersList and adding all orders...\n";
    OrdersList* ordersList = new OrdersList();
    ordersList->addOrder(deploy);
    ordersList->addOrder(advance);
    ordersList->addOrder(bomb);
    ordersList->addOrder(blockade);
    ordersList->addOrder(airlift);
    ordersList->addOrder(negotiate);

    std::cout << "Orders added to list.\n";
    std::cout << *ordersList << "\n";

    // demonstrate validate() method for each order
    std::cout << "Demonstrating validate() method:\n";
    for (int i = 0; i < ordersList->size(); i++) {
        Order* order = ordersList->getOrder(i);
        bool valid = order->validate();
        std::cout << "Order " << i << " is " << (valid ? "valid" : "invalid") << "\n";
    }
    std::cout << "\n";

    // demonstrate execute() method, validates then executes
    std::cout << "Demonstrating execute() method:\n";
    std::cout << "Executing all orders in the list...\n";
    for (int i = 0; i < ordersList->size(); i++) {
        Order* order = ordersList->getOrder(i);
        order->execute();
    }

    std::cout << "\nOrders after execution:\n";
    std::cout << *ordersList << "\n";

    //demonstrate move() method
    std::cout << "Demonstrating move() method:\n";
    std::cout << "Moving order from index position 0 to index position 3...\n";
    ordersList->move(0, 3);
    std::cout << *ordersList << "\n";

    //demonstrate remove() method
    std::cout << "Demonstrating remove() method:\n";
    std::cout << "Removing order at index position 1...\n";
    ordersList->remove(1);
    std::cout << *ordersList << "\n";

    // cleanup
    delete ordersList;

    std::cout << "========== End of Orders List Test ==========\n";
}

/**
int main() {
    testOrdersLists();
    return 0;
}
 */