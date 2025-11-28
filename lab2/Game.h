#pragma once
#include "Strategy.h"
#include <vector>
#include <string>
#include "StrategyFactory.h"

class Game {
private:
    std::vector<Strategy*> strategies;
    int steps;

public:
    Game(int numSteps);
    void addStrategy(Strategy* strategy);
    
    void runDetailed();
    void runFast();
    void runFastForTournament();
    void runTournament(const std::vector<std::string>& allStrategies, StrategyFactory& factory);
    
    void printWinner() const;
    void clearStrategies();
};