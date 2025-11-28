#include "GameRunner.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <regex>
#include <algorithm>

void GameRunner::run() {
    srand(time(0));
    StrategyFactory factory;
    
    std::cout << "Prisoner's Dilemma Simulator\n\n";
    
    std::string mode = selectMode();
    factory.printAvailableStrategies();
    std::vector<std::string> strategies = inputStrategies(mode, factory);
    int steps = inputSteps(mode);
    
    if (mode == "tournament") {
        runTournament(strategies, steps, factory);
    } else {
        runStandardGame(strategies, mode, steps, factory);
    }
    
    std::cout << "\nGame finished! Thanks for playing!\n";
}

std::string GameRunner::selectMode() {
    std::string mode;
    
    while (true) {
        std::cout << "Select mode (detailed/fast/tournament): ";
        std::getline(std::cin, mode);
        
        std::regex mode_regex("^(detailed|fast|tournament)$", std::regex_constants::icase);
        
        if (std::regex_match(mode, mode_regex)) {
            std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
            break;
        } else {
            std::cout << "Invalid mode! Please enter 'detailed', 'fast', or 'tournament'.\n";
        }
    }
    
    return mode;
}

std::vector<std::string> GameRunner::inputStrategies(const std::string& mode, StrategyFactory& factory) {
    std::vector<std::string> strategies;
    std::vector<std::string> available = factory.getAvailableStrategies();
    
    if (mode == "tournament") {
        std::cout << "\nEnter strategy names for tournament (min 3, space separated): ";
    } else {
        std::cout << "\nEnter 3 strategy names (space separated): ";
    }
    
    std::string input;
    std::getline(std::cin, input);
    
    std::regex word_regex("[a-zA-Z]+");
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), word_regex);
    auto words_end = std::sregex_iterator();
    
    for (auto it = words_begin; it != words_end; ++it) {
        std::string strategy_name = it->str();
        
        if (std::find(available.begin(), available.end(), strategy_name) != available.end()) {
            strategies.push_back(strategy_name);
        } else {
            std::cout << "Unknown strategy: '" << strategy_name << "' - skipping\n";
        }
    }
    
    if (mode == "tournament") {
        if (strategies.size() < 3) {
            std::cout << "Need at least 3 valid strategies. Using defaults: allc, alld, random\n";
            strategies = {"allc", "alld", "random"};
        }
    } else {
        if (strategies.size() < 3) {
            std::cout << "Need exactly 3 valid strategies. Using defaults: allc, alld, random\n";
            strategies = {"allc", "alld", "random"};
        } else if (strategies.size() > 3) {
            std::cout << "Using first 3 valid strategies.\n";
            strategies.resize(3);
        }
    }
    
    std::cout << "Selected strategies: ";
    for (const auto& s : strategies) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    
    return strategies;
}

int GameRunner::inputSteps(const std::string& mode) {
    int steps = 10;
    
    std::cout << "Enter number of steps per game [" << steps << "]: ";
    std::string input;
    std::getline(std::cin, input);

    input.erase(0, input.find_first_not_of(" \t"));
    input.erase(input.find_last_not_of(" \t") + 1);
    
    if (!input.empty()) {
        std::regex number_regex("^[1-9]\\d*$");
        
        if (std::regex_match(input, number_regex)) {
            try {
                steps = std::stoi(input);
            } catch (const std::exception& e) {
                std::cout << "Invalid number! Using default: " << steps << std::endl;
            }
        } else {
            std::cout << "Its not a positive integer. Using default: " << steps << std::endl;
        }
    }
    
    return steps;
}

void GameRunner::runStandardGame(const std::vector<std::string>& strategies, const std::string& mode, int steps, StrategyFactory& factory) {
    Game game(steps);
    
    for (const auto& name : strategies) {
        Strategy* strategy = factory.create(name);
        if (strategy) {
            game.addStrategy(strategy);
        }
    }
    
    if (mode == "detailed") {
        game.runDetailed();
    } else {
        game.runFast();
    }
}

void GameRunner::runTournament(const std::vector<std::string>& strategies, int steps, StrategyFactory& factory) {
    Game game(steps);
    game.runTournament(strategies, factory);
}