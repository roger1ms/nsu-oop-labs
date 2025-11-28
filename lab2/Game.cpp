#include "Game.h"
#include <iostream>
#include <algorithm>
#include <regex>

Game::Game(int numSteps) : steps(numSteps) {}

void Game::addStrategy(Strategy* strategy) {
    strategies.push_back(strategy);
}

void Game::clearStrategies() {
    strategies.clear();
}

void Game::runDetailed() {
    std::cout << "=== DETAILED MODE ===\n";
    std::cout << "Press Enter after each step, 'quit' to exit\n\n";
    
    for (int i = 0; i < steps; i++) {
        std::cout << "Step " << (i + 1) << ": ";
        
        std::vector<char> moves;
        for (Strategy* strategy : strategies) {
            char move = strategy->makeMove();
            moves.push_back(move);
            std::cout << strategy->getName() << "=" << move << " ";
        }
        std::cout << std::endl;
        
        std::vector<int> oldScores;
        for (Strategy* strategy : strategies) {
            std::vector<int> scores = strategy->getScores();
            oldScores.push_back(scores[0]);
        }
        
        for (int j = 0; j < strategies.size(); j++) {
            char myMove = moves[j];
            char opp1Move = moves[(j + 1) % 3];
            char opp2Move = moves[(j + 2) % 3];
            strategies[j]->addToHistory(myMove, opp1Move, opp2Move);
        }
        
        std::cout << "Points this round: ";
        for (int j = 0; j < strategies.size(); j++) {
            std::vector<int> newScores = strategies[j]->getScores();
            int roundPoints = newScores[0] - oldScores[j];
            std::cout << strategies[j]->getName() << "=" << roundPoints << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Total scores: ";
        for (Strategy* strategy : strategies) {
            std::vector<int> scores = strategy->getScores();
            std::cout << strategy->getName() << "=" << scores[0] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Press Enter to continue...";
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "quit") {
            std::cout << "Game interrupted.\n";
            return;
        }
        std::cout << std::endl;
    }
    
    printWinner();
}

void Game::runFast() {
    std::cout << "=== FAST MODE ===\n";
    std::cout << "Running " << steps << " steps...\n\n";
    
    for (int i = 0; i < steps; i++) {
        std::vector<char> moves;
        for (Strategy* strategy : strategies) {
            char move = strategy->makeMove();
            moves.push_back(move);
        }
        
        for (int j = 0; j < strategies.size(); j++) {
            char myMove = moves[j];
            char opp1Move = moves[(j + 1) % 3];
            char opp2Move = moves[(j + 2) % 3];
            strategies[j]->addToHistory(myMove, opp1Move, opp2Move);
        }
    }
    
    std::cout << "Game completed!\n";
    printWinner();
}

void Game::runFastForTournament() {
    for (int i = 0; i < steps; i++) {
        std::vector<char> moves;
        for (Strategy* strategy : strategies) {
            moves.push_back(strategy->makeMove());
        }
        
        for (int j = 0; j < strategies.size(); j++) {
            char myMove = moves[j];
            char opp1Move = moves[(j + 1) % 3];
            char opp2Move = moves[(j + 2) % 3];
            strategies[j]->addToHistory(myMove, opp1Move, opp2Move);
        }
    }
}

void Game::runTournament(const std::vector<std::string>& allStrategies, StrategyFactory& factory) {
    if (allStrategies.size() < 3) {
        std::cout << "Need at least 3 strategies for tournament\n";
        return;
    }
    
    std::cout << "=== TOURNAMENT MODE ===\n";
    std::cout << "Strategies: " << allStrategies.size() << "\n";
    std::cout << "Steps per game: " << steps << "\n\n";
    
    std::map<std::string, int> totalScores;
    int totalGames = 0;
    
    for (int i = 0; i < allStrategies.size(); i++) {
        for (int j = i + 1; j < allStrategies.size(); j++) {
            for (int k = j + 1; k < allStrategies.size(); k++) {
                totalGames++;
                
                std::vector<std::string> currentTriple = {
                    allStrategies[i], allStrategies[j], allStrategies[k]
                };
                
                std::cout << "Game " << totalGames << ": " 
                          << currentTriple[0] << " vs " 
                          << currentTriple[1] << " vs " 
                          << currentTriple[2] << "\n";
                
                clearStrategies();
                
                for (const auto& name : currentTriple) {
                    Strategy* strategy = factory.create(name);
                    if (strategy) {
                        addStrategy(strategy);
                    }
                }
                
                runFastForTournament();
                
                std::string gameWinner;
                int maxScore = -1;
                
                for (Strategy* strategy : strategies) {
                    std::vector<int> scores = strategy->getScores();
                    totalScores[strategy->getName()] += scores[0];
                    
                    if (scores[0] > maxScore) {
                        maxScore = scores[0];
                        gameWinner = strategy->getName();
                    }
                }
                
                std::cout << "Winner: " << gameWinner << " (" << maxScore << " points)\n";
                std::cout << "----------------------------------------\n";
                
                for (Strategy* strategy : strategies) {
                    delete strategy;
                }
                strategies.clear();
            }
        }
    }
    
    std::cout << "\n=== TOURNAMENT FINAL RESULTS ===\n";
    std::cout << "Total games played: " << totalGames << "\n\n";
    
    std::vector<std::pair<std::string, int>> sortedResults;
    for (const auto& pair : totalScores) {
        sortedResults.push_back(pair);
    }
    
    std::sort(sortedResults.begin(), sortedResults.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (int i = 0; i < sortedResults.size(); i++) {
        std::cout << (i + 1) << ". " << sortedResults[i].first 
                  << ": " << sortedResults[i].second << " points\n";
    }
    
    if (!sortedResults.empty()) {
        std::cout << "\nTOURNAMENT CHAMPION: " << sortedResults[0].first 
                  << " with " << sortedResults[0].second << " points!\n";
    }
}


void Game::printWinner() const {
    int maxScore = -1;
    std::vector<std::string> winners;
    
    for (Strategy* strategy : strategies) {
        std::vector<int> scores = strategy->getScores();
        int score = scores[0];
        
        if (score > maxScore) {
            maxScore = score;
            winners.clear();
            winners.push_back(strategy->getName());
        } else if (score == maxScore) {
            winners.push_back(strategy->getName());
        }
    }
    
    std::cout << "=== FINAL RESULTS ===\n";
    for (Strategy* strategy : strategies) {
        std::vector<int> scores = strategy->getScores();
        std::cout << strategy->getName() << ": " << scores[0] << " points\n";
    }
    
    if (winners.size() == 1) {
        std::cout << "\nWINNER: " << winners[0] << " with " << maxScore << " points!\n";
    } else {
        std::cout << "\nIt's a TIE between: ";
        for (const auto& winner : winners) {
            std::cout << winner << " ";
        }
        std::cout << "with " << maxScore << " points each!\n";
    }
}