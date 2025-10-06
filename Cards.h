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
#include "Orders.h"

using namespace std;

//Forward declare classes needed
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

