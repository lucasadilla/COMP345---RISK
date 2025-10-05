//
// Created by Nathan on 2025-10-05.
//
#ifndef COMP345_RISK_GAMEENGINE_H
#define COMP345_RISK_GAMEENGINE_H

#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

enum class State {
    Start,
    MapLoaded,
    MapValidated,
    PlayersAdded,
    AssignReinforcement,
    IssueOrders,
    ExecuteOrders,
    Win,
    Finished
};

class GameEngine {
public:
    GameEngine();
    void testGameStates();

private:
    State current;
    
    // transitions[current_state][command] = next_state
    std::unordered_map<State,std::unordered_map<std::string, State>> transition;

    static const char* name(State s);

    bool apply(const std::string& cmd); // returns true if transition happened
};

#endif
