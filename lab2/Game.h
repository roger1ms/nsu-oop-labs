#pragma once
#include "Strategy.h"
#include <vector>

class Game {
private:
    std::vector<Strategy*> strategies;
    int steps;

public:
    Game(int numSteps);
    void addStrategy(Strategy* strategy);
    void runGame();
    void printWinner() const;
};