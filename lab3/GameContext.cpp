#include "GameContext.h"
#include "GameObjects.h"
#include <curses.h>
#include <algorithm>

GameContext::GameContext() = default;

void GameContext::addObject(std::unique_ptr<GameObject> object) {
    objects.push_back(std::move(object));
}

void GameContext::removeDeadObjects() {
    for (auto obj= objects.begin(); obj!= objects.end(); ) {
        if (!(*obj)->isAlive()) {
            if (!dynamic_cast<Tank*>(obj->get())) {
                obj= objects.erase(obj);
            } else {
                ++obj;
            }
        } else {
            ++obj;
        }
    }
}
std::vector<GameObject*> GameContext::getObjectsAt(int x, int y) const {
    std::vector<GameObject*> req;
    for (auto& obj : objects) {
        if (obj->getX() == x && obj->getY() == y && obj->isAlive()) {
            req.push_back(obj.get());
        }
    }
    return req;
}

bool GameContext::isPositionFree(int x, int y) const {
    for (auto& obj : objects) {
        if (obj->getX() == x && obj->getY() == y && obj->isAlive()) {
            return false;
        }
    }
    return true;
}

void GameContext::addScore(int points) {
    score+=points;
}

void GameContext::endGame(bool won) {
    gameRunning = false;
    gameWon = won;
}

void GameContext::updateInput() {
    pressedKeys.clear();
    
    int ch;
    while ((ch = getch()) != ERR) {
        pressedKeys.push_back(ch);
    }
}

bool GameContext::isKeyPressed(int key) const {
    return std::find(pressedKeys.begin(), pressedKeys.end(), key) != pressedKeys.end();

}

int GameContext::getScore() const {
    return score;
}

bool GameContext::isGameRunning() const {
    return gameRunning;
}

bool GameContext::isGameWon() const {
    return gameWon;
}