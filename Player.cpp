//
// Created by Rahath on 2025-09-09.
//
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include <algorithm>
#include <iostream>
using namespace std;

// Constructor
Player::Player(const string& n)
        : name(new string(n)),
          ownedTerritories(new vector<Map::territoryNode*>()),
          hand(new Hand()),
          ordersList(new OrdersList()) {
    cout << "[Player] Created player '" << *name << "'\n";
}

// Copy constructor
Player::Player(const Player& other)
        : name(new string(*other.name)),
          ownedTerritories(new vector<Map::territoryNode*>()),
          hand(new Hand(*other.hand)),
          ordersList(new OrdersList(*other.ordersList)) {

    // Copy territories (shallow)
    for (auto* t : *other.ownedTerritories) {
        ownedTerritories->push_back(t);
    }

    cout << "[Player] Copied player '" << *name << "'\n";
}

// Assignment operator
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        *name = *other.name;

        delete ownedTerritories;
        delete hand;
        delete ordersList;

        ownedTerritories = new vector<Map::territoryNode*>(*other.ownedTerritories);
        hand = new Hand(*other.hand);
        ordersList = new OrdersList(*other.ordersList);
    }
    return *this;
}

// Destructor
Player::~Player() {
    delete name;
    delete ownedTerritories;
    delete hand;
    delete ordersList;
    cout << "[Player] Destroyed player object\n";
}

// issueOrder — adds the given order to the player’s OrdersList
void Player::issueOrder(Order* order) {
    if (!order) return;
    ordersList->addOrder(order);
    cout << "[Player::issueOrder] " << *name
         << " issued: " << *order << endl;
}

// addTerritory
void Player::addTerritory(Map::territoryNode* t) {
    if (t) ownedTerritories->push_back(t);
}

// addCard
void Player::addCard(Card* c) {
    if (c) hand->addCard(c);
}

// toDefend
vector<Map::territoryNode*> Player::toDefend() const {
    cout << "[Player::toDefend] " << *name << " chooses territories to defend.\n";
    return *ownedTerritories;
}

// toAttack
vector<Map::territoryNode*> Player::toAttack() const {
    cout << "[Player::toAttack] " << *name << " chooses territories to attack.\n";
    return *ownedTerritories;
}

// Getters
string Player::getName() const { return *name; }
const vector<Map::territoryNode*>* Player::getOwnedTerritories() const { return ownedTerritories; }
OrdersList* Player::getOrdersList() const { return ordersList; }
Hand* Player::getHand() const { return hand; }

// Stream insertion operator
ostream& operator<<(ostream& os, const Player& p) {
    os << "Player(" << *p.name
       << ", territories=" << p.ownedTerritories->size()
       << ", cards=" << p.hand->size()
       << ", orders=" << p.ordersList->size() << ")";
    return os;
}
