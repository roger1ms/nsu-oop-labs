#include "game.h"
#include <iostream>
#include <algorithm>

Game::Game(int numSteps) : steps(numSteps) {}

void Game::addStrategy(Strategy* strategy) {
    strategies.push_back(strategy);
}

void Game::runGame() {
    std::cout << "Starting game for " << steps << " steps\n";
    
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
        std::cout << std::endl << std::endl;
    }

    printWinner();
    std::cout << "Game finished!\n";
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
    
    std::cout << "FINAL SCORES:\n";
    for (Strategy* strategy : strategies) {
        std::vector<int> scores = strategy->getScores();
        std::cout << strategy->getName() << ": " << scores[0] << " points\n";
    }
    
    // Выводим победителя
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