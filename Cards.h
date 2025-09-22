//
// Created by Rahath on 2025-09-09.
//


#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;


class Order {
public:
    string* description;
    Order(const string& d = "") : description(new string(d)) {}
    Order(const Order& o) : description(new string(*o.description)) {}
    Order& operator=(const Order& o) {
        if (this != &o) { *description = *o.description; }
        return *this;
    }
    virtual ~Order() { delete description; }
    friend ostream& operator<<(ostream& os, const Order& o) {
        os << "Order(" << *o.description << ")";
        return os;
    }
};

class Player;


class Deck;
class Hand;

class Card {
public:
    string* type;

    Card(const string& t = "reinforcement");
    Card(const Card& other);
    Card& operator=(const Card& other);
    ~Card();

    const string& getType() const;

    void play(Player* p, Deck* d, Hand* h);

    friend ostream& operator<<(ostream& os, const Card& c);
};


class Deck {
public:
    vector<Card*>* cards;

    Deck();
    explicit Deck(int size);
    Deck(const Deck& other);
    Deck& operator=(const Deck& other);
    ~Deck();

    Card* draw();
    void returnCard(Card* c);

    size_t size() const;
    const vector<Card*>* getCards() const;

    friend ostream& operator<<(ostream& os, const Deck& d);

private:
    static mt19937& rng();
};


class Hand {
public:
    vector<Card*>* cards;

    Hand();
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    ~Hand();

    void addCard(Card* c);
    void removeCard(Card* c);
    size_t size() const;
    const vector<Card*>* getCards() const;

    friend ostream& operator<<(ostream& os, const Hand& h);
};


class Player {
public:
    string* name;
    vector<Order*>* orders;

    Player(const string& n = "Player");
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    void addOrder(Order* o);

    friend ostream& operator<<(ostream& os, const Player& p);
};


namespace CardsUtil {
    inline const vector<string>& validTypes() {
        static const vector<string> types{
            "bomb", "reinforcement", "blockade", "airlift", "diplomacy"
        };
        return types;
    }

    inline string normalizeType(const string& t) {
        for (const auto& s : validTypes()) {
            if (s == t) return s;
        }
        return "reinforcement";
    }
}


void testCards();

#endif

