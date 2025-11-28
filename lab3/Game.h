#pragma once
#include "GameContext.h"
#include "GameObjects.h"
#include "GameConfig.h"
#include <cstdlib>
#include <ctime> 

class Game {
private:
    GameContext context;
    bool initialized;

    int bonusSpawnTimer;
    int nextSpawnTime;
    int activeBonuses; 
    const int MAX_BONUSES = 2;
public:
    Game();
    void initializeGame();
    void run();
    void processInput();
    void render() const;
    void renderUI() const;
    void controlFrameRate() const;
    void renderFinalScreen() const; 
    
    void updateBonusSpawning();
    void spawnRandomBonus();
    int countActiveBonuses() const;
};