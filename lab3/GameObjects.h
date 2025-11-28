#pragma once
#include "GameConfig.h"
#include "GameContext.h"


class GameObject {
protected:
    int x, y;
    char symbol;
    bool alive;
    
public:
    GameObject(int startX, int startY, char sym);
    virtual ~GameObject() = default;
    
    virtual void update(GameContext& context) = 0;
    
    virtual void draw();
    virtual void onCollision(GameObject* other, GameContext& context);

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
    bool isAlive() const { return alive; }
    
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void kill() { alive = false; }
};


class Tank : public GameObject {
protected:
    int direction;
    int speed;
    int strength;
    int baseSpeed;
    int reloadTime;
    int currentReload;
    int heatPoints;
    int respawnTimer;
    int startX, startY;
    char startSymbol;
    int freezeTimer;
    int IncreaseDamageTimer;
    int score;
public:
    Tank(int startX, int startY, char sym, int dir);
    int getRespawnTimer() const { return respawnTimer; }
    int getDirection() { return direction; }
    bool isFrozen() const { return freezeTimer > 0; }
    int getStrength() { return strength; };
    
    void setIncreaseTime(int time) { IncreaseDamageTimer = time; }
    void increaseDamage() { strength = 2; }
    void update(GameContext& context) override;
    void onCollision(GameObject* other, GameContext& context) override;
    void move(int newDirection, GameContext& context);
    void respawn();
    void freeze(int duration); 
    void setSpeed(int newSpeed);
    void resetSpeed() { speed = baseSpeed; }
    void draw() override;
    void checkForBonuses(GameContext& context);

    virtual void takeDamage(GameContext& context, int takeDamage);
    void shoot(GameContext& context);
    bool canShoot() const { return currentReload == 0 && alive; }


    void addScore(int points) { score += points; }
    int getScore() const { return score; }
    bool hasWinningScore() const { return score >= 1000; }
    
    virtual void onUpdate(GameContext& context) = 0;
};


class MyTank : public Tank {
public:
    MyTank(int startX, int startY);
    void onUpdate(GameContext& context) override;
};


class Bullet : public GameObject {
private:
    int direction;      // Направление движения
    int damage;         // Урон
    GameObject* owner;  // Кто выпустил пулю

public:
    Bullet(int startX, int startY, int dir,int strength, GameObject* bulletOwner);
    int getDamage() const { return damage; } 
    void update(GameContext& context) override;
    void onCollision(GameObject* other, GameContext& context) override;
    void draw() override;
    
    GameObject* getOwner() { return owner; };
};


class EnemyTank : public Tank {
private:
    int moveCounter = 0;
    int aiTimer;
    int reactionDelay;
    GameObject* currentTarget;
public:
    EnemyTank(int startX, int startY);
    void onUpdate(GameContext& context) override;
    void takeDamage(GameContext& context, int damage) override;
    GameObject* findTarget(GameContext& context);
    bool hasClearShot(GameObject* target);
    void aimAtTarget(GameObject* target);
    void makeSmartMove(GameContext& context);

    void setSpeed(int newSpeed) { speed = newSpeed; }
};


class Wall : public GameObject {
public:
    Wall(int startX, int startY);
    void update(GameContext& context) override;
};

class DestroyableWall : public GameObject {
private:
    int heatPoints;
public:
    DestroyableWall(int startX, int startY);
    void update(GameContext& context) override;
    void takeDamage(GameContext& context, int takeDamage);
};

class StopClock : public GameObject {
private:
    int effectDuration;
public:
    StopClock(int startX, int startY);
    void update(GameContext& context) override;
    void onCollision(GameObject* other, GameContext& context) override;
    void draw() override;
};

class IncreaseDamage : public GameObject {
private:
    int effectDuration;
public:
    IncreaseDamage(int startX, int startY);
    void update(GameContext& context) override;
    void onCollision(GameObject* other, GameContext& context) override;
    void draw() override;
};


