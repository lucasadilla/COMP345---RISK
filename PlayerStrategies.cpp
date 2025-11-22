//
// PlayerStrategies.cpp
// Implementation of player strategies using the Strategy design pattern
//

#include "PlayerStrategies.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>

// Forward declaration to avoid circular dependency
class GameEngine;

// ============================================================================
// PlayerStrategy Base Class Implementation
// ============================================================================

PlayerStrategy::PlayerStrategy(Player* p) : player(p) {
    if (!player) {
        std::cerr << "[PlayerStrategy] Warning: Created strategy with null player pointer.\n";
    }
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& other) : player(other.player) {}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& other) {
    if (this != &other) {
        player = other.player;
    }
    return *this;
}

PlayerStrategy::~PlayerStrategy() {
}

void PlayerStrategy::setPlayer(Player* p) {
    player = p;
}

std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy) {
    os << "Strategy: " << strategy.getStrategyName();
    return os;
}

// ============================================================================
// HumanPlayerStrategy Implementation
// ============================================================================

HumanPlayerStrategy::HumanPlayerStrategy(Player* p) : PlayerStrategy(p) {}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& other) : PlayerStrategy(other) {}

HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

HumanPlayerStrategy::~HumanPlayerStrategy() {}

std::string HumanPlayerStrategy::getStrategyName() const {
    return "Human";
}

PlayerStrategy* HumanPlayerStrategy::clone() const {
    return new HumanPlayerStrategy(*this);
}

std::vector<Map::territoryNode*> HumanPlayerStrategy::toAttack() const {
    if (!player) return {};
    
    std::vector<Map::territoryNode*> attackable;
    const auto* owned = player->getOwnedTerritories();
    if (!owned) return {};
    
    // For human players, we'll return all territories adjacent to owned ones
    // The actual selection will be done through user interaction in issueOrder
    return attackable;
}

std::vector<Map::territoryNode*> HumanPlayerStrategy::toDefend() const {
    if (!player) return {};
    
    const auto* owned = player->getOwnedTerritories();
    if (!owned) return {};
    
    // Return all owned territories for human players
    std::vector<Map::territoryNode*> defendable;
    for (auto* t : *owned) {
        defendable.push_back(t);
    }
    return defendable;
}

void HumanPlayerStrategy::issueOrder() {
    if (!player) {
        std::cout << "[HumanPlayerStrategy] Cannot issue orders: player is null.\n";
        return;
    }

    std::cout << "\n=== " << player->getName() << " (Human Player) - Issue Orders ===\n";
    std::cout << "Reinforcement Pool: " << player->getReinforcementPool() << "\n";
    std::cout << "Owned Territories: " << player->getOwnedTerritories()->size() << "\n";
    std::cout << "Cards in Hand: " << player->getHand()->size() << "\n\n";

    // Issue deploy orders first
    issueDeployOrders();

    // Issue advance orders
    issueAdvanceOrders();

    // Play cards
    playCards();

    std::cout << "[HumanPlayerStrategy] " << player->getName() << " finished issuing orders.\n\n";
}

void HumanPlayerStrategy::issueDeployOrders() {
    if (!player || player->getReinforcementPool() <= 0) {
        return;
    }

    const auto* owned = player->getOwnedTerritories();
    if (!owned || owned->empty()) {
        std::cout << "[HumanPlayerStrategy] No territories to deploy to.\n";
        return;
    }

    int remainingReinforcements = player->getReinforcementPool();
    std::cout << "--- Deploy Phase ---\n";
    std::cout << "You have " << remainingReinforcements << " reinforcements to deploy.\n";

    while (remainingReinforcements > 0) {
        std::cout << "\nRemaining reinforcements: " << remainingReinforcements << "\n";
        std::cout << "Enter territory name to deploy to (or 'done' to finish): ";
        std::string territoryName;
        std::getline(std::cin, territoryName);

        if (territoryName == "done" || territoryName.empty()) {
            break;
        }

        // Find the territory
        Map::territoryNode* target = nullptr;
        for (auto* t : *owned) {
            if (t && t->name == territoryName) {
                target = t;
                break;
            }
        }

        if (!target) {
            std::cout << "Territory '" << territoryName << "' not found or not owned by you.\n";
            std::cout << "Your territories: ";
            for (size_t i = 0; i < owned->size(); ++i) {
                std::cout << (*owned)[i]->name;
                if (i + 1 < owned->size()) std::cout << ", ";
            }
            std::cout << "\n";
            continue;
        }

        int armies = getIntInput("Enter number of armies to deploy: ", 1, remainingReinforcements);
        if (armies <= 0 || armies > remainingReinforcements) {
            std::cout << "Invalid number of armies.\n";
            continue;
        }

        player->issueOrder(new Deploy(armies, target->name));
        remainingReinforcements -= armies;
        player->setReinforcementPool(remainingReinforcements);
    }
}

void HumanPlayerStrategy::issueAdvanceOrders() {
    if (!player) return;

    const auto* owned = player->getOwnedTerritories();
    if (!owned || owned->empty()) {
        return;
    }

    std::cout << "\n--- Advance Phase ---\n";
    std::string continueAdvance = "yes";
    
    while (continueAdvance == "yes" || continueAdvance == "y") {
        std::cout << "\nYour territories: ";
        for (size_t i = 0; i < owned->size(); ++i) {
            std::cout << (*owned)[i]->name;
            if (i + 1 < owned->size()) std::cout << ", ";
        }
        std::cout << "\n";

        std::cout << "Enter source territory name (or 'done' to finish): ";
        std::string sourceName;
        std::getline(std::cin, sourceName);

        if (sourceName == "done" || sourceName.empty()) {
            break;
        }

        Map::territoryNode* source = nullptr;
        for (auto* t : *owned) {
            if (t && t->name == sourceName) {
                source = t;
                break;
            }
        }

        if (!source) {
            std::cout << "Source territory '" << sourceName << "' not found or not owned by you.\n";
            continue;
        }

        std::cout << "Enter target territory name: ";
        std::string targetName;
        std::getline(std::cin, targetName);

        if (targetName.empty()) {
            std::cout << "Target territory name cannot be empty.\n";
            continue;
        }

        // Check if target is adjacent to source
        bool isAdjacent = false;
        for (int adjIdx : source->adjacentIndices) {
            // We need access to the map to resolve indices to territory nodes
            // For now, we'll check by name matching - this is a limitation
            // that should be fixed by passing the map to the strategy
        }

        int armies = getIntInput("Enter number of armies to advance: ", 1, 1000);
        if (armies <= 0) {
            std::cout << "Invalid number of armies.\n";
            continue;
        }

        player->issueOrder(new Advance(armies, sourceName, targetName));

        std::cout << "Issue another advance order? (yes/no): ";
        std::getline(std::cin, continueAdvance);
    }
}

void HumanPlayerStrategy::playCards() {
    if (!player) return;

    Hand* hand = player->getHand();
    if (!hand || hand->size() == 0) {
        return;
    }

    std::cout << "\n--- Card Playing Phase ---\n";
    const auto* cards = hand->getCards();
    if (!cards || cards->empty()) {
        return;
    }

    std::cout << "Your cards: ";
    for (size_t i = 0; i < cards->size(); ++i) {
        std::cout << (i + 1) << ". " << cards->at(i)->getType();
        if (i + 1 < cards->size()) std::cout << ", ";
    }
    std::cout << "\n";

    std::string playMore = "yes";
    while (playMore == "yes" || playMore == "y") {
        if (hand->size() == 0) {
            std::cout << "No more cards to play.\n";
            break;
        }

        int cardIndex = getIntInput("Enter card number to play (or 0 to skip): ", 0, static_cast<int>(hand->size()));
        if (cardIndex == 0) {
            break;
        }

        if (cardIndex < 1 || cardIndex > static_cast<int>(hand->size())) {
            std::cout << "Invalid card number.\n";
            continue;
        }

        Card* cardToPlay = cards->at(cardIndex - 1);
        std::string cardType = cardToPlay->getType();

        // For human players, we need to get specific parameters for card orders
        if (cardType == "bomb") {
            std::string targetTerritory;
            std::cout << "Enter target territory for bomb: ";
            std::getline(std::cin, targetTerritory);
            if (!targetTerritory.empty()) {
                player->issueOrder(new Bomb(targetTerritory));
                hand->removeCard(cardToPlay);
                // Note: In a full implementation, we'd return the card to the deck
            }
        } else if (cardType == "reinforcement") {
            std::string targetTerritory;
            int armies;
            std::cout << "Enter target territory for reinforcement: ";
            std::getline(std::cin, targetTerritory);
            armies = getIntInput("Enter number of armies: ", 1, 1000);
            if (!targetTerritory.empty() && armies > 0) {
                player->issueOrder(new Deploy(armies, targetTerritory));
                hand->removeCard(cardToPlay);
            }
        } else if (cardType == "blockade") {
            std::string targetTerritory;
            std::cout << "Enter target territory for blockade: ";
            std::getline(std::cin, targetTerritory);
            if (!targetTerritory.empty()) {
                player->issueOrder(new Blockade(targetTerritory));
                hand->removeCard(cardToPlay);
            }
        } else if (cardType == "airlift") {
            std::string source, target;
            int armies;
            std::cout << "Enter source territory: ";
            std::getline(std::cin, source);
            std::cout << "Enter target territory: ";
            std::getline(std::cin, target);
            armies = getIntInput("Enter number of armies: ", 1, 1000);
            if (!source.empty() && !target.empty() && armies > 0) {
                player->issueOrder(new Airlift(armies, source, target));
                hand->removeCard(cardToPlay);
            }
        } else if (cardType == "diplomacy") {
            std::string targetPlayer;
            std::cout << "Enter target player name for diplomacy: ";
            std::getline(std::cin, targetPlayer);
            if (!targetPlayer.empty()) {
                player->issueOrder(new Negotiate(targetPlayer));
                hand->removeCard(cardToPlay);
            }
        }

        std::cout << "Play another card? (yes/no): ";
        std::getline(std::cin, playMore);
    }
}

Map::territoryNode* HumanPlayerStrategy::selectTerritory(const std::vector<Map::territoryNode*>& options, const std::string& prompt) const {
    if (options.empty()) {
        return nullptr;
    }

    std::cout << prompt << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << options[i]->name << "\n";
    }

    int choice = getIntInput("Enter choice: ", 1, static_cast<int>(options.size()));
    if (choice >= 1 && choice <= static_cast<int>(options.size())) {
        return options[choice - 1];
    }
    return nullptr;
}

int HumanPlayerStrategy::getIntInput(const std::string& prompt, int min, int max) const {
    int value;
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        std::istringstream iss(input);
        if (iss >> value && value >= min && value <= max) {
            return value;
        }
        
        std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
    }
}

// ============================================================================
// AggressivePlayerStrategy Implementation (Stub)
// ============================================================================

AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p) : PlayerStrategy(p) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& other) : PlayerStrategy(other) {}
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

std::string AggressivePlayerStrategy::getStrategyName() const {
    return "Aggressive";
}

PlayerStrategy* AggressivePlayerStrategy::clone() const {
    return new AggressivePlayerStrategy(*this);
}

std::vector<Map::territoryNode*> AggressivePlayerStrategy::toAttack() const {
    // TODO: Implement aggressive attack logic
    return {};
}

std::vector<Map::territoryNode*> AggressivePlayerStrategy::toDefend() const {
    // TODO: Implement aggressive defense logic (strongest territory)
    if (!player) return {};
    const auto* owned = player->getOwnedTerritories();
    if (!owned || owned->empty()) return {};
    return {owned->at(0)}; // Placeholder
}

void AggressivePlayerStrategy::issueOrder() {
    // TODO: Implement aggressive order issuing
    std::cout << "[AggressivePlayerStrategy] " << (player ? player->getName() : "Unknown") << " issues aggressive orders.\n";
}

// ============================================================================
// BenevolentPlayerStrategy Implementation (Stub)
// ============================================================================

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p) : PlayerStrategy(p) {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other) : PlayerStrategy(other) {}
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

std::string BenevolentPlayerStrategy::getStrategyName() const {
    return "Benevolent";
}

PlayerStrategy* BenevolentPlayerStrategy::clone() const {
    return new BenevolentPlayerStrategy(*this);
}

std::vector<Map::territoryNode*> BenevolentPlayerStrategy::toAttack() const {
    return {}; // Benevolent never attacks
}

std::vector<Map::territoryNode*> BenevolentPlayerStrategy::toDefend() const {
    // TODO: Implement benevolent defense logic (weakest territories)
    if (!player) return {};
    const auto* owned = player->getOwnedTerritories();
    if (!owned || owned->empty()) return {};
    return *owned; // Placeholder
}

void BenevolentPlayerStrategy::issueOrder() {
    // TODO: Implement benevolent order issuing
    std::cout << "[BenevolentPlayerStrategy] " << (player ? player->getName() : "Unknown") << " issues benevolent orders.\n";
}

// ============================================================================
// NeutralPlayerStrategy Implementation (Stub)
// ============================================================================

NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p) : PlayerStrategy(p) {}
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& other) : PlayerStrategy(other) {}
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

std::string NeutralPlayerStrategy::getStrategyName() const {
    return "Neutral";
}

PlayerStrategy* NeutralPlayerStrategy::clone() const {
    return new NeutralPlayerStrategy(*this);
}

std::vector<Map::territoryNode*> NeutralPlayerStrategy::toAttack() const {
    return {}; // Neutral never attacks
}

std::vector<Map::territoryNode*> NeutralPlayerStrategy::toDefend() const {
    if (!player) return {};
    const auto* owned = player->getOwnedTerritories();
    if (!owned) return {};
    return *owned;
}

void NeutralPlayerStrategy::issueOrder() {
    // Neutral never issues orders unless attacked (then becomes aggressive)
    std::cout << "[NeutralPlayerStrategy] " << (player ? player->getName() : "Unknown") << " is neutral and issues no orders.\n";
}

// ============================================================================
// CheaterPlayerStrategy Implementation (Stub)
// ============================================================================

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* p) : PlayerStrategy(p) {}
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& other) : PlayerStrategy(other) {}
CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}

std::string CheaterPlayerStrategy::getStrategyName() const {
    return "Cheater";
}

PlayerStrategy* CheaterPlayerStrategy::clone() const {
    return new CheaterPlayerStrategy(*this);
}

std::vector<Map::territoryNode*> CheaterPlayerStrategy::toAttack() const {
    // TODO: Return all adjacent territories
    return {};
}

std::vector<Map::territoryNode*> CheaterPlayerStrategy::toDefend() const {
    if (!player) return {};
    const auto* owned = player->getOwnedTerritories();
    if (!owned) return {};
    return *owned;
}

void CheaterPlayerStrategy::issueOrder() {
    // TODO: Automatically conquer all adjacent territories
    std::cout << "[CheaterPlayerStrategy] " << (player ? player->getName() : "Unknown") << " cheats and conquers adjacent territories.\n";
}

