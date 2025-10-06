//
// Created by Rahath on 2025-10-06.
// Main driver that calls all test functions for COMP345 Assignment 1.
//

#include <iostream>
#include "MapDriver.h"
#include "MapLoader.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"

using namespace std;

int main() {
    cout << "==============================" << endl;
    cout << " COMP345 - Assignment 1 Demo " << endl;
    cout << "==============================" << endl << endl;

    try {
        cout << ">>> PART 1: MAP & MAPLOADER <<<" << endl;
        testLoadMaps(R"(Maps/Americas 1792.map)");
        cout << "\n------------------------------------\n" << endl;

        cout << ">>> PART 2: PLAYER <<<" << endl;
        testPlayers();
        cout << "\n------------------------------------\n" << endl;

        cout << ">>> PART 3: ORDERS LIST <<<" << endl;
        testOrdersLists();
        cout << "\n------------------------------------\n" << endl;

        cout << ">>> PART 4: CARDS <<<" << endl;
        testCards();
        cout << "\n------------------------------------\n" << endl;

        cout << ">>> PART 5: GAME ENGINE <<<" << endl;
        //TODO: testGameStates();
        cout << "\n------------------------------------\n" << endl;

        cout << "All module tests completed successfully!" << endl;
    }
    catch (const exception& e) {
        cerr << "An error occurred during execution: " << e.what() << endl;
    }

    return 0;
}
