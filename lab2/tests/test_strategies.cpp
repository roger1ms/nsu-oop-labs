#include <gtest/gtest.h>
#include "Strategy.h"
#include "StrategyFactory.h"

class StrategyTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = new StrategyFactory();
    }
    
    void TearDown() override {
        delete factory;
    }
    
    StrategyFactory* factory;
};

//allc должен ходить только C
TEST_F(StrategyTest, AllCStrategyAlwaysCooperates) {
    Strategy* allc = factory->create("allc");
    ASSERT_NE(allc, nullptr);
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(allc->makeMove(), 'C');
    }
    delete allc;
}

//alld должен ходить только D
TEST_F(StrategyTest, AllDStrategyAlwaysDefects) {
    Strategy* alld = factory->create("alld");
    ASSERT_NE(alld, nullptr);
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(alld->makeMove(), 'D');
    }
    delete alld;
}

//Рандом должен и предавать, и сотрудничать
TEST_F(StrategyTest, RandomStrategyVariability) {
    Strategy* random = factory->create("random");
    ASSERT_NE(random, nullptr);
    
    int cooperations = 0, defections = 0;
    for (int i = 0; i < 10000; i++) {
        char move = random->makeMove();
        if (move == 'C') cooperations++;
        else if (move == 'D') defections++;
    }
    
    EXPECT_GT(cooperations, 0);
    EXPECT_GT(defections, 0);
    delete random;
}


//Мстящий предает только после D
TEST_F(StrategyTest, RevengeStrategyBehavior) {
    Strategy* revenge = factory->create("revenge");
    
    EXPECT_EQ(revenge->makeMove(), 'C');
    revenge->addToHistory('C', 'D', 'C');
    EXPECT_EQ(revenge->makeMove(), 'D');
    revenge->addToHistory('C', 'C', 'C');
    EXPECT_EQ(revenge->makeMove(), 'C');
    
    delete revenge;
}

//Логика провокатора
TEST_F(StrategyTest, ProvocationStrategy) {
    Strategy* prov = factory->create("provocate");
    
    EXPECT_EQ(prov->makeMove(), 'D');
    prov->addToHistory('D', 'D', 'C');
    EXPECT_EQ(prov->makeMove(), 'C');
    prov->addToHistory('C', 'C', 'C');
    EXPECT_EQ(prov->makeMove(), 'D');
    
    delete prov;
}

//проверка логики
TEST_F(StrategyTest, RememberFirstEnemyStrategy) {
    Strategy* enemy = factory->create("youmyenemy");
    
    EXPECT_EQ(enemy->makeMove(), 'C');
    enemy->addToHistory('C', 'D', 'C');
    EXPECT_EQ(enemy->makeMove(), 'D');
    enemy->addToHistory('C', 'C', 'C');
    EXPECT_EQ(enemy->makeMove(), 'D');
    
    delete enemy;
}

// Проверка таблицы очков
TEST_F(StrategyTest, ScoreCalculationAllCombinations) {
    Strategy* allc = factory->create("allc");
    
    allc->addToHistory('C', 'C', 'C');
    std::vector<int> scores = allc->getScores();
    EXPECT_EQ(scores[0], 7);
    
    allc->clearHistory();
    allc->addToHistory('C', 'C', 'D');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 3);
    
    allc->clearHistory();
    allc->addToHistory('C', 'D', 'C');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 3);
    
    allc->clearHistory();
    allc->addToHistory('D', 'C', 'C');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 9);
    
    allc->clearHistory();
    allc->addToHistory('C', 'D', 'D');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 0);
    
    allc->clearHistory();
    allc->addToHistory('D', 'C', 'D');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 5);
    
    allc->clearHistory();
    allc->addToHistory('D', 'D', 'C');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 5);
    
    allc->clearHistory();
    allc->addToHistory('D', 'D', 'D');
    scores = allc->getScores();
    EXPECT_EQ(scores[0], 1);
    
    delete allc;
}

//Добавление очков корректно
TEST_F(StrategyTest, ScoreAccumulation) {
    Strategy* allc = factory->create("allc");
    
    allc->addToHistory('C', 'C', 'C'); // +7
    allc->addToHistory('C', 'C', 'C'); // +7 = 14
    allc->addToHistory('D', 'D', 'D'); // +1 = 15
    
    std::vector<int> scores = allc->getScores();
    EXPECT_EQ(scores[0], 15);
    
    delete allc;
}

//Все имеющиеся стратегии корректно добавляются
TEST_F(StrategyTest, FactoryCreatesAllStrategies) {
    std::vector<std::string> strategy_names = {
        "allc", "alld", "random", "revenge", "youmyenemy",
        "analyzer", "copybest", "strategymix", "findbest",
        "provocate", "cycled"
    };
    
    for (const auto& name : strategy_names) {
        Strategy* strategy = factory->create(name);
        EXPECT_NE(strategy, nullptr) << "Failed to create strategy: " << name;
        if (strategy) delete strategy;
    }
    
    EXPECT_EQ(factory->create("unknown"), nullptr);
}

//Проверка логики стратегии
TEST_F(StrategyTest, AnalyzerStrategyWithDifferentPatterns) {
    Strategy* analyzer = factory->create("analyzer");
    
    //Остальные часто сотрудничают, должен тоже
    analyzer->addToHistory('C', 'C', 'C');
    analyzer->addToHistory('C', 'C', 'C');
    analyzer->addToHistory('C', 'C', 'D');
    EXPECT_EQ(analyzer->makeMove(), 'C');
    
    analyzer->clearHistory();
    
    // Остальные часто предают -> должен тоже
    analyzer->addToHistory('C', 'D', 'D');
    analyzer->addToHistory('C', 'D', 'C');
    analyzer->addToHistory('C', 'D', 'D');
    EXPECT_EQ(analyzer->makeMove(), 'D');
    
    delete analyzer;
}

//Проверка логики стратегий
TEST_F(StrategyTest, CopyBestPlayerStrategyLogic) {
    Strategy* copybest = factory->create("copybest");
    
    copybest->addToHistory('C', 'C', 'C');
    copybest->addToHistory('C', 'D', 'C');
    
    //Повторяет за 2ым игроком
    char move = copybest->makeMove();
    EXPECT_TRUE(move == 'D');
    
    delete copybest;
}

//Проверка логики стратегии
TEST_F(StrategyTest, MixedStrategyCombinations) {
    Strategy* mix = factory->create("strategymix");
    ASSERT_NE(mix, nullptr);
    
    // Просто работает - ОК
    for (int i = 0; i < 10; i++) {
        char move = mix->makeMove();
        EXPECT_TRUE(move == 'C' || move == 'D');
        mix->addToHistory(move, 'C', 'D');
    }
    
    delete mix;
}
