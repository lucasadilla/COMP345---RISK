//
// Created by Rahath on 2025-09-09.
//

#ifndef COMP345_RISK_PLAYER_H
#define COMP345_RISK_PLAYER_H

#include "Map.h"    // required for Map::territoryNode
#include <iostream>
#include <vector>
#include <string>

// Forward declarations to avoid circular includes
class Order;
class OrdersList;
class Hand;
class Card;
class PlayerStrategy;

using namespace std;

class Player {
private:
    string* name;
    vector<Map::territoryNode*>* ownedTerritories;
    Hand* hand;
    OrdersList* ordersList;
    int reinforcementPool;
    PlayerStrategy* strategy;  // Strategy pattern: player behavior

public:
    Player(const string& n = "Player");
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    // Strategy pattern delegation methods
    void issueOrder();  // Delegates to strategy's issueOrder()
    vector<Map::territoryNode*> toDefend() const;  // Delegates to strategy
    vector<Map::territoryNode*> toAttack() const;  // Delegates to strategy
    
    // Direct order issuing (used internally by strategies)
    void issueOrder(Order* order);
    
    // Strategy management
    void setStrategy(PlayerStrategy* s);
    PlayerStrategy* getStrategy() const;

    void addTerritory(Map::territoryNode* t);
    void addCard(Card* c);

    string getName() const;
    const vector<Map::territoryNode*>* getOwnedTerritories() const;
    OrdersList* getOrdersList() const;
    Hand* getHand() const;
    int getReinforcementPool() const;
    void setReinforcementPool(int armies);
    void addReinforcements(int armies);
    void clearTerritories();

    friend ostream& operator<<(ostream& os, const Player& p);
};

void testPlayers();

#endif // COMP345_RISK_PLAYER_H
