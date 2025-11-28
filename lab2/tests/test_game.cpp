#include <gtest/gtest.h>
#include "Game.h"
#include "StrategyFactory.h"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = new StrategyFactory();
    }
    
    void TearDown() override {
        delete factory;
    }
    
    StrategyFactory* factory;
};

//Игра инициализируется
TEST_F(GameTest, GameCreation) {
    Game game(10);
    SUCCEED();
}

//Стратегии добавляются
TEST_F(GameTest, AddStrategiesToGame) {
    Game game(5);
    
    game.addStrategy(factory->create("allc"));
    game.addStrategy(factory->create("alld"));
    game.addStrategy(factory->create("random"));
    
    SUCCEED();
}

//Стратегии удаляются
TEST_F(GameTest, ClearStrategies) {
    Game game(5);
    
    game.addStrategy(factory->create("allc"));
    game.addStrategy(factory->create("alld"));
    game.clearStrategies();
    
    SUCCEED();
}

//Игра иницализируется и на разном количестве шагов
TEST_F(GameTest, DifferentStepCounts) {
    Game short_game(1);
    Game medium_game(1001);
    Game long_game(10000001);
    
    SUCCEED();
}

//Все стратегии добавлены
TEST_F(GameTest, GameWithDifferentStrategies) {
    Game game(10);
    
    game.addStrategy(factory->create("allc"));
    game.addStrategy(factory->create("alld"));
    game.addStrategy(factory->create("random"));
    game.addStrategy(factory->create("revenge"));
    game.addStrategy(factory->create("analyzer"));
    
    SUCCEED();
}

//Игра работает без ошибок
TEST_F(GameTest, MultipleRoundsConsistency) {
    Game game(1000);
    
    Strategy* allc = factory->create("allc");
    Strategy* alld = factory->create("alld");
    
    game.addStrategy(allc);
    game.addStrategy(alld);
    game.addStrategy(factory->create("random"));
    
    SUCCEED();
    
    game.clearStrategies();
}

//Игра работает без ошибок
TEST_F(GameTest, TournamentModeSetup) {
    std::vector<std::string> strategies = {"allc", "alld", "random", "revenge", "analyzer"};
    Game game(100);
    
    for (const auto& name : strategies) {
        Strategy* strategy = factory->create(name);
        game.addStrategy(strategy);
    }
    
    game.clearStrategies();
    SUCCEED();
}