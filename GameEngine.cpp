//
// Created by Naathan on 2025-10-05.
//

#include "GameEngine.h"

#include "GameEngine.h"


//transition table / transitions[current_state][command] = next_state
GameEngine::GameEngine() : current(State::Start)
{

    // startup
    transition[State::Start]["loadmap"] = State::MapLoaded;
    transition[State::MapLoaded]["loadmap"] = State::MapLoaded;     // self-loop
    transition[State::MapLoaded]["validatemap"] = State::MapValidated;
    transition[State::MapValidated]["addplayer"] = State::PlayersAdded;
    transition[State::PlayersAdded]["addplayer"] = State::PlayersAdded; // self-loop

    // play
    transition[State::PlayersAdded]["assigncountries"] = State::AssignReinforcement;
    transition[State::AssignReinforcement]["issueorder"] = State::IssueOrders;
    transition[State::IssueOrders]["issueorder"] = State::IssueOrders; // self-loop
    transition[State::IssueOrders]["endissueorders"] = State::ExecuteOrders;
    transition[State::ExecuteOrders]["execorder"] = State::ExecuteOrders; // self-loop
    transition[State::ExecuteOrders]["endexecorders"] = State::AssignReinforcement; // back to assign
    transition[State::ExecuteOrders]["win"] = State::Win;

    // terminal / replay
    transition[State::Win]["play"] = State::Start;
    transition[State::Win]["end"] = State::Finished;
}

//user feedback on input
const char* GameEngine::name(State s) {
    switch (s)
    {
        case State::Start: return "start";
        case State::MapLoaded: return "map loaded";
        case State::MapValidated: return "map validated";
        case State::PlayersAdded: return "players added";
        case State::AssignReinforcement: return "assign reinforcement";
        case State::IssueOrders: return "issue orders";
        case State::ExecuteOrders: return "execute orders";
        case State::Win: return "win";
        case State::Finished: return "finished";
    }
    return "?";
}

//transition validation
bool GameEngine::apply(const std::string& cmd) {

    auto itState = transition.find(current);

    if (itState == transition.end()) //if not found
    {
        return false;
    }

    auto itCmd = itState->second.find(cmd);  //equal to the value of the inner unordered map ( (key) state, (value) [ (key) string , (value) state ])

    if (itCmd == itState->second.end())  
    {
        return false;
    }

    //apply transition 
    current = itCmd->second;  
    return true;
}

//test function
void GameEngine::testGameStates() {
    std::cout << "Current state: " << name(current) << "\n";
    while (current != State::Finished) 
    {

        std::cout << "Enter command: ";
        std::string cmd;

        if (!std::getline(std::cin, cmd))
        {
            break;
        }

        if (cmd.empty())
        {
            continue;
        }

        if (apply(cmd)) 
        {
            std::cout << "OK -> " << name(current) << "\n";
        }
        else 
        {
            std::cout << "Invalid command from '" << name(current) << "': " << cmd << "\n";
        }
    }
    std::cout << "Reached 'finished'. Game end.\n";
}

