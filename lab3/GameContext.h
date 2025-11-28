#pragma once
#include <vector>
#include <memory>

class GameObject;

class GameContext {
private:
    std::vector<std::unique_ptr<GameObject>> objects;
    int score = 0;
    bool gameRunning = true;
    bool gameWon = false;
    std::vector<int> pressedKeys;   
    
public:
    GameContext();
    
    void addObject(std::unique_ptr<GameObject> object);
    void removeDeadObjects();
    std::vector<GameObject*> getObjectsAt(int x, int y) const;
    bool isPositionFree(int x, int y) const;

    void addScore(int points);
    void endGame(bool won);
    void updateInput();

    bool isKeyPressed(int key) const;
    
    int getScore() const;
    bool isGameRunning() const;
    bool isGameWon() const;
    const auto& getAllObjects() const { return objects; }
};