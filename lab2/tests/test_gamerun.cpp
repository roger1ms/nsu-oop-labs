#include <gtest/gtest.h>
#include "Game.h"
#include "GameRunner.h"
#include "StrategyFactory.h"

class GameRunTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = new StrategyFactory();
    }
    
    void TearDown() override {
        delete factory;
    }
    
    StrategyFactory* factory;
};

//класс создается
TEST_F(GameRunTest, GameRunnerCreation) {
    GameRunner runner;
    SUCCEED();
}

//В фабрике есть стратегии
TEST_F(GameRunTest, FactoryAvailableStrategies) {
    auto strategies = factory->getAvailableStrategies();
    
    EXPECT_FALSE(strategies.empty());
    
    bool has_allc = false, has_alld = false, has_random = false;
    for (const auto& name : strategies) {
        if (name == "allc") has_allc = true;
        if (name == "alld") has_alld = true;
        if (name == "random") has_random = true;
    }
    
    EXPECT_TRUE(has_allc);
    EXPECT_TRUE(has_alld);
    EXPECT_TRUE(has_random);
}

//Несколько игр могут создаваться
TEST_F(GameRunTest, MultipleGames) {
    Game game1(10);
    Game game2(20);
    
    game1.addStrategy(factory->create("allc"));
    game2.addStrategy(factory->create("alld"));
    
    SUCCEED();
}

//Можем добавить одну стратегию в 2 игры
TEST_F(GameRunTest, StrategyReuse) {
    Strategy* allc = factory->create("allc");
    Game game1(5);
    game1.addStrategy(allc);
    
    allc->clearHistory();
    
    Game game2(5);
    game2.addStrategy(allc);
    
    delete allc;
    SUCCEED();
}

//Стратегии с лишними символами или слишком длинные не ломают логику
TEST_F(GameRunTest, EdgeCases) {
    EXPECT_EQ(factory->create(""), nullptr);
    
    std::string long_name(1000, 'a');
    EXPECT_EQ(factory->create(long_name), nullptr);
    
    EXPECT_EQ(factory->create("allc!"), nullptr);
}

//Ходы корректно добавляются в историю
TEST_F(GameRunTest, StrategyPersistence) {
    Strategy* revenge = factory->create("revenge");
    
    char move1 = revenge->makeMove();
    revenge->addToHistory(move1, 'C', 'C');
    
    char move2 = revenge->makeMove();
    revenge->addToHistory(move2, 'D', 'C');
    
    char move3 = revenge->makeMove();
    EXPECT_TRUE(move3 == 'C' || move3 == 'D');
    
    delete revenge;
}

//Все игроки должны получать одинаковые очки при одинаковых ходах
TEST_F(GameRunTest, ScoreDistribution) {
    Strategy* player1 = factory->create("allc");
    Strategy* player2 = factory->create("allc");
    Strategy* player3 = factory->create("allc");
    
    player1->addToHistory('C', 'C', 'C');
    player2->addToHistory('C', 'C', 'C'); 
    player3->addToHistory('C', 'C', 'C');
    
    std::vector<int> scores1 = player1->getScores();
    std::vector<int> scores2 = player2->getScores();
    std::vector<int> scores3 = player3->getScores();
    
    EXPECT_EQ(scores1[0], 7);
    EXPECT_EQ(scores2[0], 7);
    EXPECT_EQ(scores3[0], 7);
    
    delete player1;
    delete player2;
    delete player3;
}

//Проверяем ожидаемые результаты игр
TEST_F(GameRunTest, AllDBeatsAllCInLongGame) {
    Game game(100);
    
    Strategy* allc = factory->create("allc");
    Strategy* alld = factory->create("alld");
    Strategy* random = factory->create("random");
    
    game.addStrategy(allc);
    game.addStrategy(alld);
    game.addStrategy(random);
    
    game.runFast();
    
    std::vector<int> scores_allc = allc->getScores();
    std::vector<int> scores_alld = alld->getScores();
    
    EXPECT_GT(scores_alld[0], scores_allc[0]);
    
    game.clearStrategies();
}