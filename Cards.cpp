#include "Cards.h"

using namespace std;

Card::Card(const string& t)
: type(new string(CardsUtil::normalizeType(t))) {}

Card::Card(const Card& other)
: type(new string(*other.type)) {}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        *type = *other.type;
    }
    return *this;
}

Card::~Card() {
    delete type;
}

const string& Card::getType() const {
    return *type;
}

void Card::play(Player* p, Deck* d, Hand* h) {
    if (!p || !d || !h) {
        cout << "[Card::play] Missing player/deck/hand context. No-op.\n";
        return;
    }

    string orderDesc;
    if (*type == "bomb") orderDesc = "Bomb order created by card.";
    else if (*type == "reinforcement") orderDesc = "Reinforcement order created by card.";
    else if (*type == "blockade") orderDesc = "Blockade order created by card.";
    else if (*type == "airlift") orderDesc = "Airlift order created by card.";
    else if (*type == "diplomacy") orderDesc = "Negotiate/Diplomacy order created by card.";
    else orderDesc = "Generic order created by card.";

    p->addOrder(new Order(orderDesc));
    h->removeCard(this);
    d->returnCard(this);

    cout << "[Card::play] Played '" << *type << "': " << orderDesc << " Card returned to deck.\n";
}

ostream& operator<<(ostream& os, const Card& c) {
    os << "Card(" << *c.type << ")";
    return os;
}

Deck::Deck() : cards(new vector<Card*>) {}

Deck::Deck(int size) : cards(new vector<Card*>) {
    const auto& types = CardsUtil::validTypes();
    for (int i = 0; i < size; ++i) {
        cards->push_back(new Card(types[static_cast<size_t>(i) % types.size()]));
    }
}

Deck::Deck(const Deck& other) : cards(new vector<Card*>) {
    cards->reserve(other.cards->size());
    for (const auto* c : *other.cards) {
        cards->push_back(new Card(*c));
    }
}

Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        for (auto* c : *cards) delete c;
        cards->clear();
        for (const auto* c : *other.cards) {
            cards->push_back(new Card(*c));
        }
    }
    return *this;
}

Deck::~Deck() {
    for (auto* c : *cards) delete c;
    delete cards;
}

mt19937& Deck::rng() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

Card* Deck::draw() {
    if (cards->empty()) {
        cout << "[Deck::draw] Deck is empty.\n";
        return nullptr;
    }
    uniform_int_distribution<size_t> dist(0, cards->size() - 1);
    size_t idx = dist(rng());
    Card* c = (*cards)[idx];
    (*cards)[idx] = cards->back();
    cards->pop_back();
    cout << "[Deck::draw] Drew " << *c << ". Remaining in deck: " << cards->size() << "\n";
    return c;
}

void Deck::returnCard(Card* c) {
    if (!c) return;
    cards->push_back(c);
    cout << "[Deck::returnCard] Returned " << *c << ". Deck size: " << cards->size() << "\n";
}

size_t Deck::size() const { return cards->size(); }
const vector<Card*>* Deck::getCards() const { return cards; }

ostream& operator<<(ostream& os, const Deck& d) {
    os << "Deck(size=" << d.size() << ")[";
    for (size_t i = 0; i < d.cards->size(); ++i) {
        os << d.cards->at(i)->getType();
        if (i + 1 < d.cards->size()) os << ", ";
    }
    os << "]";
    return os;
}

Hand::Hand() : cards(new vector<Card*>) {}

Hand::Hand(const Hand& other) : cards(new vector<Card*>) {
    for (const auto* c : *other.cards) {
        cards->push_back(new Card(*c));
    }
}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        for (auto* c : *cards) delete c;
        cards->clear();
        for (const auto* c : *other.cards) {
            cards->push_back(new Card(*c));
        }
    }
    return *this;
}

Hand::~Hand() {
    for (auto* c : *cards) delete c;
    delete cards;
}

void Hand::addCard(Card* c) { if (c) cards->push_back(c); }

void Hand::removeCard(Card* c) {
    if (!c) return;
    auto it = find(cards->begin(), cards->end(), c);
    if (it != cards->end()) cards->erase(it);
}

size_t Hand::size() const { return cards->size(); }
const vector<Card*>* Hand::getCards() const { return cards; }

ostream& operator<<(ostream& os, const Hand& h) {
    os << "Hand(size=" << h.size() << ")[";
    for (size_t i = 0; i < h.cards->size(); ++i) {
        os << h.cards->at(i)->getType();
        if (i + 1 < h.cards->size()) os << ", ";
    }
    os << "]";
    return os;
}

Player::Player(const string& n)
: name(new string(n)), orders(new vector<Order*>) {}

Player::Player(const Player& other)
: name(new string(*other.name)), orders(new vector<Order*>) {
    for (const auto* o : *other.orders) {
        orders->push_back(new Order(*o));
    }
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        *name = *other.name;
        for (auto* o : *orders) delete o;
        orders->clear();
        for (const auto* o : *other.orders) {
            orders->push_back(new Order(*o));
        }
    }
    return *this;
}

Player::~Player() {
    for (auto* o : *orders) delete o;
    delete orders;
    delete name;
}

void Player::addOrder(Order* o) {
    if (!o) return;
    orders->push_back(o);
    cout << "[Player] Added " << *o << ". Total orders: " << orders->size() << "\n";
}

ostream& operator<<(ostream& os, const Player& p) {
    os << "Player(" << *p.name << ", orders=" << p.orders->size() << ")";
    return os;
}

void testCards() {
    cout << "=== Part 4: Cards deck/hand demo ===\n";

    Deck deck(15);
    cout << deck << "\n\n";

    Player alice("Alice");
    Hand hand;
    cout << alice << "\n" << hand << "\n\n";

    for (int i = 0; i < 7; ++i) {
        Card* c = deck.draw();
        if (c) hand.addCard(c);
    }

    cout << "After drawing:\n";
    cout << "  " << deck << "\n";
    cout << "  " << hand << "\n\n";

    vector<Card*> toPlay = *hand.getCards();
    for (auto* c : toPlay) {
        cout << "Playing " << *c << "...\n";
        c->play(&alice, &deck, &hand);
    }

    cout << "\nAfter playing all drawn cards:\n";
    cout << "  " << deck << "\n";
    cout << "  " << hand << "\n";
    cout << "  " << alice << "\n";

    cout << "=== End of Part 4 demo ===\n";
}