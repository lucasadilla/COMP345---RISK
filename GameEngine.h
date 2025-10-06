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
    GameEngine(const GameEngine& other);    //copy
    GameEngine& operator=(const GameEngine& other); //assignment operator  
    friend std::ostream& operator<<(std::ostream& os, const GameEngine& g);  //stream insertion

    static const char* name(State s);

    bool apply(const std::string& cmd); // returns true if transition happened
    State state() const { return current; }

private:
    State current;
    
    // transitions[current_state][command] = next_state
    std::unordered_map<State,std::unordered_map<std::string, State>> transition;

};


#endif
