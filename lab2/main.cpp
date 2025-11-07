#include "StrategyFactory.h"
#include "Game.h"
#include "Strategy.h"
#include "Strategy.cpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));

    StrategyFactory factory;
    factory.printAvailableStrategies();

    int steps;
    std::cout << "\nEnter number of steps: ";
    std::cin >> steps;

    Game game(steps);

    std::string names[3];
    Strategy* strategies[3];

    for (int i = 0; i < 3; i++) {
        std::cout << "Enter strategy name for player " << (i + 1) << ": ";
        std::cin >> names[i];

        strategies[i] = factory.create(names[i]);
        if (!strategies[i]) {
            std::cout << "Invalid strategy! Using 'allc' as default.\n";
            strategies[i] = factory.create("allc");
        }

        game.addStrategy(strategies[i]);
    }

    game.runGame();

    // Очистка памяти
    for (int i = 0; i < 3; i++)
        delete strategies[i];

    return 0;
}
