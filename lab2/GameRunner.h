#pragma once
#include "StrategyFactory.h"
#include "Game.h"
#include <vector>
#include <string>

class GameRunner {
public:
    void run();
    
private:
    std::string selectMode();
    std::vector<std::string> inputStrategies(const std::string& mode, StrategyFactory& factory);
    int inputSteps(const std::string& mode);
    void runStandardGame(const std::vector<std::string>& strategies, const std::string& mode, int steps, StrategyFactory& factory);
    void runTournament(const std::vector<std::string>& strategies, int steps, StrategyFactory& factory);
};