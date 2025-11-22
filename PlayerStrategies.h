//
// PlayerStrategies.h
// Strategy pattern implementation for player behaviors
//

#ifndef COMP345_RISK_PLAYERSTRATEGIES_H
#define COMP345_RISK_PLAYERSTRATEGIES_H

#include "Map.h"
#include <vector>
#include <string>

// Forward declarations
class Player;
class Order;
class Card;
class Hand;
class Deck;

/**
 * Abstract base class for player strategies.
 * Implements the Strategy design pattern for different player behaviors.
 */
class PlayerStrategy {
protected:
    Player* player;  // Pointer to the player using this strategy

public:
    PlayerStrategy(Player* p);
    PlayerStrategy(const PlayerStrategy& other);
    PlayerStrategy& operator=(const PlayerStrategy& other);
    virtual ~PlayerStrategy();

    /**
     * Issues orders for the player based on the strategy.
     * This is the main method that determines player behavior.
     */
    virtual void issueOrder() = 0;

    /**
     * Determines which territories the player should attack.
     * @return Vector of territories to attack
     */
    virtual std::vector<Map::territoryNode*> toAttack() const = 0;

    /**
     * Determines which territories the player should defend.
     * @return Vector of territories to defend
     */
    virtual std::vector<Map::territoryNode*> toDefend() const = 0;

    /**
     * Creates a deep copy of the strategy.
     * @return Pointer to a new strategy instance
     */
    virtual PlayerStrategy* clone() const = 0;

    /**
     * Gets the name of the strategy.
     * @return Strategy name
     */
    virtual std::string getStrategyName() const = 0;

    /**
     * Sets the player pointer for this strategy.
     * Used when cloning strategies to update the player reference.
     */
    void setPlayer(Player* p);

    friend std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy);
};

/**
 * Human player strategy.
 * Requires user interaction for all decisions.
 */
class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy(Player* p);
    HumanPlayerStrategy(const HumanPlayerStrategy& other);
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& other);
    virtual ~HumanPlayerStrategy();

    void issueOrder() override;
    std::vector<Map::territoryNode*> toAttack() const override;
    std::vector<Map::territoryNode*> toDefend() const override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;

private:
    /**
     * Prompts user to issue deploy orders.
     */
    void issueDeployOrders();

    /**
     * Prompts user to issue advance orders.
     */
    void issueAdvanceOrders();

    /**
     * Prompts user to play cards.
     */
    void playCards();

    /**
     * Helper to get user input for territory selection.
     */
    Map::territoryNode* selectTerritory(const std::vector<Map::territoryNode*>& options, const std::string& prompt) const;

    /**
     * Helper to get user input for integer values.
     */
    int getIntInput(const std::string& prompt, int min = 0, int max = 1000) const;
};

/**
 * Aggressive player strategy.
 * Focuses on attacking and strengthening strongest territory.
 */
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy(Player* p);
    AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);
    virtual ~AggressivePlayerStrategy();

    void issueOrder() override;
    std::vector<Map::territoryNode*> toAttack() const override;
    std::vector<Map::territoryNode*> toDefend() const override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * Benevolent player strategy.
 * Focuses on defending weakest territories.
 */
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy(Player* p);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);
    virtual ~BenevolentPlayerStrategy();

    void issueOrder() override;
    std::vector<Map::territoryNode*> toAttack() const override;
    std::vector<Map::territoryNode*> toDefend() const override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * Neutral player strategy.
 * Never issues orders unless attacked, then becomes aggressive.
 */
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy(Player* p);
    NeutralPlayerStrategy(const NeutralPlayerStrategy& other);
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& other);
    virtual ~NeutralPlayerStrategy();

    void issueOrder() override;
    std::vector<Map::territoryNode*> toAttack() const override;
    std::vector<Map::territoryNode*> toDefend() const override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * Cheater player strategy.
 * Automatically conquers all adjacent territories.
 */
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    CheaterPlayerStrategy(Player* p);
    CheaterPlayerStrategy(const CheaterPlayerStrategy& other);
    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& other);
    virtual ~CheaterPlayerStrategy();

    void issueOrder() override;
    std::vector<Map::territoryNode*> toAttack() const override;
    std::vector<Map::territoryNode*> toDefend() const override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

#endif // COMP345_RISK_PLAYERSTRATEGIES_H

