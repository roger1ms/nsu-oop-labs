#include "GameObjects.h"
#include <ctime>
#include <cstdlib>
#include <curses.h>

GameObject::GameObject(int startX, int startY, char sym) 
    : x(startX), y(startY), symbol(sym), alive(true) {}

void GameObject::draw() {
    mvaddch(y, x, symbol);
}

void GameObject::onCollision(GameObject* other, GameContext& context) {
    // Базовая обработка столкновений
}



Tank::Tank(int startX, int startY, char sym, int dir) 
    : GameObject(startX, startY, sym), direction(dir), 
      speed(1), reloadTime(10), currentReload(0), heatPoints(2),
      respawnTimer(0), startX(startX), startY(startY), 
      startSymbol(sym), baseSpeed(1), strength(1), score(0) {}

void Tank::update(GameContext& context) {
    if (!alive) {
        respawnTimer++;
        if (respawnTimer >= 200) {
            respawn();
        }
        return;
    }
    if (freezeTimer > 0) {
        freezeTimer--;
        if (freezeTimer == 0) {
            resetSpeed();
        }
    }
    if (!isFrozen()) {
        if (currentReload > 0) currentReload--;
        onUpdate(context);
    }
    
}

void Tank::freeze(int duration) {
    freezeTimer = duration;
    setSpeed(0); 
}

void Tank::setSpeed(int newSpeed) {
    if (!isFrozen()) {
        speed = newSpeed;
    }
}

void Tank::respawn() {
    alive = true;
    heatPoints = 2;
    x = startX;
    y = startY;
    direction = 2;
    currentReload = 0;
    respawnTimer = 0;
    symbol = startSymbol;
    freezeTimer = 0;
}

void Tank::draw() {
    if (!alive) return;
    char sym;
    switch (direction) {
        case 0: sym = '^'; break; // UP
        case 1: sym = '>'; break; // RIGHT  
        case 2: sym = 'v'; break; // DOWN
        case 3: sym = '<'; break; // LEFT
    }
     if (dynamic_cast<MyTank*>(this)) {
        // ИГРОК - зеленый жирный
        attron(A_BOLD | COLOR_PAIR(2)); // зелёный
        mvaddch(y, x, sym);
        attroff(A_BOLD | COLOR_PAIR(2));
    } else {
        // ВРАГ - красный жирный  
        attron(A_BOLD | COLOR_PAIR(1)); // красный
        mvaddch(y, x, sym);
        attroff(A_BOLD | COLOR_PAIR(1));
    }
}

void Tank::move(int dir, GameContext& context) {
    int newX = x, newY = y;
    switch (dir) {
        case 0: newY--; break; // UP
        case 1: newX++; break; // RIGHT  
        case 2: newY++; break; // DOWN
        case 3: newX--; break; // LEFT
    }
    
    direction = dir;
    
    auto objects = context.getObjectsAt(newX, newY);
    
    for (auto* obj : objects) {
        if (obj != this) {
            // Пропускаем бонусы - они не блокируют движение
            if (!dynamic_cast<StopClock*>(obj) && !dynamic_cast<IncreaseDamage*>(obj)) {
                onCollision(obj, context);
                return; // Не двигаемся
            }
        }
    }
    // Двигаемся
    x = newX;
    y = newY;
    
    // ПОСЛЕ ДВИЖЕНИЯ проверяем бонусы в НОВОЙ позиции
    checkForBonuses(context);
}

void Tank::checkForBonuses(GameContext& context) {
    auto objects = context.getObjectsAt(x, y);
    for (auto* obj : objects) {
        if (obj != this && (dynamic_cast<StopClock*>(obj) || dynamic_cast<IncreaseDamage*>(obj))) {
            obj->onCollision(this, context);
        }
    }
}

void Tank::shoot(GameContext& context) {
    if (canShoot()) {
        int bulletX = x, bulletY = y;
        switch (direction) {
            case 0: bulletY--; break; // UP
            case 1: bulletX++; break; // RIGHT  
            case 2: bulletY++; break; // DOWN
            case 3: bulletX--; break; // LEFT
        }
        
        // Создаем пулю
        auto bullet = std::make_unique<Bullet>(bulletX, bulletY, direction, strength, this);
        context.addObject(std::move(bullet));
        currentReload = reloadTime;
    }
}

void Tank::takeDamage(GameContext& context, int takeDamage) {
    if (heatPoints - takeDamage <= 0) {
        heatPoints = 0;
        kill();
        respawnTimer = 0;
        context.addScore(100);
    }
    else {
        heatPoints -= takeDamage;
    }
}

void Tank::onCollision(GameObject* other, GameContext& context) {
    if (dynamic_cast<Wall*>(other) || dynamic_cast<DestroyableWall*>(other) || 
        dynamic_cast<Tank*>(other)) {
        // Стена, другой танк, бонус - блокировка движения 
    }
}



MyTank::MyTank(int startX, int startY) 
    : Tank(startX, startY, 'A', 0) {}

void MyTank::onUpdate(GameContext& context) {
    checkForBonuses(context);
    if (IncreaseDamageTimer > 0) {
        IncreaseDamageTimer--;
        if (IncreaseDamageTimer == 0) {
            strength = 1;
        }
    }

    if (context.isKeyPressed(KEY_UP)) {
        move(0, context);
    }
    else if (context.isKeyPressed(KEY_RIGHT)) {
        move(1, context);
    }
    else if (context.isKeyPressed(KEY_DOWN)) {
        move(2, context);
    }
    else if (context.isKeyPressed(KEY_LEFT)) {
        move(3, context);
    }
    
    if (context.isKeyPressed(' ')) {
        shoot(context);
    }
}



EnemyTank::EnemyTank(int startX, int startY) 
    : Tank(startX, startY, 'E', 2), 
      moveCounter(rand() % 20),
      aiTimer(0),
      reactionDelay(rand() % 8 + 10),
      currentTarget(nullptr) {}

void EnemyTank::onUpdate(GameContext& context) {
    if (!alive) return;
    if (currentReload > 0) currentReload--;
    
    aiTimer++;
    moveCounter++;
    
    currentTarget = findTarget(context);
    bool canSeeTarget = currentTarget && hasClearShot(currentTarget);
    
    if (canSeeTarget) {
        aimAtTarget(currentTarget);
        
        if (aiTimer >= reactionDelay && canShoot()) {
            shoot(context);
            aiTimer = 0;
        }
    } 
    else if (moveCounter >= (rand() % 10 + 15)) {
        makeSmartMove(context);
        moveCounter = 0;
        
        if ((rand() % 8 == 0) && canShoot()) {
            shoot(context);
        }
    }
}

void EnemyTank::takeDamage(GameContext& context, int damage) {
    heatPoints -= damage;
    if (heatPoints <= 0) {
        kill();
        context.addScore(100); // +100 очков за убитого врага
    }
}

GameObject* EnemyTank::findTarget(GameContext& context) {
    std::vector<GameObject*> possibleTargets;
    
    // Собираем все живые танки кроме себя
    for (const auto& obj : context.getAllObjects()) {
        if (!obj->isAlive() || obj.get() == this) continue;
        
        if (dynamic_cast<Tank*>(obj.get())) {
            possibleTargets.push_back(obj.get());
        }
    }
    
    // Случайно выбираем цель из всех доступных
    if (!possibleTargets.empty()) {
        int randomIndex = rand() % possibleTargets.size();
        return possibleTargets[randomIndex];
    }
    
    return nullptr;
}

bool EnemyTank::hasClearShot(GameObject* target) {
    if (!target) return false;
    
    int dx = target->getX() - x;
    int dy = target->getY() - y;
    
    if (dx == 0 || dy == 0) {
        if ((direction == 0 && dy < 0) ||
            (direction == 1 && dx > 0) ||
            (direction == 2 && dy > 0) ||
            (direction == 3 && dx < 0)) {
            return true;
        }
    }
    return false;
}

void EnemyTank::aimAtTarget(GameObject* target) {
    if (!target) return;
    
    int dx = target->getX() - x;
    int dy = target->getY() - y;
    
    if (abs(dx) > abs(dy)) {
        direction = (dx > 0) ? 1 : 3;
    } else {
        direction = (dy > 0) ? 2 : 0;
    }
}

void EnemyTank::makeSmartMove(GameContext& context) {
    if (currentTarget && (rand() % 3 != 0)) {
        int dx = currentTarget->getX() - x;
        int dy = currentTarget->getY() - y;
        
        if (abs(dx) > abs(dy)) {
            move(dx > 0 ? 1 : 3, context);
        } else {
            move(dy > 0 ? 2 : 0, context);
        }
    } else {
        int randomDir = rand() % 4;
        move(randomDir, context);
    }
}



Bullet::Bullet(int startX, int startY, int dir,int strength, GameObject* bulletOwner)
    : GameObject(startX, startY, '.'), direction(dir), damage(strength), owner(bulletOwner) {}

void Bullet::update(GameContext& context) {
    if (!alive) return;

    auto objects = context.getObjectsAt(x, y);
    for (auto* obj : objects) {
        if (obj != this && obj != owner) {
            onCollision(obj, context);
            if (!alive) return;
        }
    }

    switch (direction) {
        case 0: y--; break; // UP
        case 1: x++; break; // RIGHT  
        case 2: y++; break; // DOWN
        case 3: x--; break; // LEFT
    }

    if (x < 0 || x >= Config::SCREEN_WIDTH || y < 0 || y >= Config::SCREEN_HEIGHT) {
        kill();
        return;
    }

    objects = context.getObjectsAt(x, y);
    for (auto* obj : objects) {
        if (obj != this && obj != owner) {
            onCollision(obj, context);
            return;
        }
    }
}

void Bullet::onCollision(GameObject* other, GameContext& context) {
    if (!alive) return;
    
    kill();
    
    if (auto* tank = dynamic_cast<Tank*>(other)) {
        tank->takeDamage(context, damage);
        // Если танк уничтожен, начисляем очки владельцу пули
        if (!tank->isAlive() && owner) {
            if (auto* ownerTank = dynamic_cast<Tank*>(owner)) {
                ownerTank->addScore(100); // +100 очков за убийство
            }
        }
    } else if (auto* destroyableWall = dynamic_cast<DestroyableWall*>(other)) {
        destroyableWall->takeDamage(context, damage);
        // +10 очков за разрушение стены
        if (owner && dynamic_cast<Tank*>(owner)) {
            auto* ownerTank = dynamic_cast<Tank*>(owner);
            ownerTank->addScore(10);
        }
    }
}

void Bullet::draw() {
    if (damage == 1) {
        mvaddch(y, x, symbol);
    }
    else {
        attron(A_BOLD | COLOR_PAIR(4));
        mvaddch(y, x, 'o');
        attroff(A_BOLD | COLOR_PAIR(4));
    }
}


Wall::Wall(int startX, int startY) 
    : GameObject(startX, startY, '#') {}

void Wall::update(GameContext& context) {
    // Стены ничего не делают
}



DestroyableWall::DestroyableWall(int startX, int startY) 
    : GameObject(startX, startY, '@'), heatPoints(2){}

void DestroyableWall::update(GameContext& context) {
    // Стены ничего не делают
}

void DestroyableWall::takeDamage(GameContext& context, int takeDamage) {
    if (heatPoints - takeDamage <= 0) {
        heatPoints = 0;
        kill();
    }
    else {
        heatPoints -= takeDamage;
    }
}



StopClock::StopClock(int startX, int startY) 
    : GameObject(startX, startY, 'C'), effectDuration(40) {} 

void StopClock::update(GameContext& context) {}

void StopClock::onCollision(GameObject* other, GameContext& context) {
    if (!alive) return;
    
    if (auto* player = dynamic_cast<MyTank*>(other)) {
        for (auto& obj : context.getAllObjects()) {
            if (auto* enemy = dynamic_cast<EnemyTank*>(obj.get())) {
                enemy->freeze(effectDuration);
            }
        }
        
        kill();
    }
}

void StopClock::draw() {
    if (!alive) return;
    
    attron(A_BOLD | COLOR_PAIR(4));
    mvaddch(y, x, symbol);
    attroff(A_BOLD | COLOR_PAIR(4));
    
}




IncreaseDamage::IncreaseDamage(int startX, int startY) 
    : GameObject(startX, startY, 'D'), effectDuration(100) {} 

void IncreaseDamage::update(GameContext& context) {}

void IncreaseDamage::onCollision(GameObject* other, GameContext& context) {
    if (!alive) return;
    
    if (auto* player = dynamic_cast<MyTank*>(other)) {
        player->setIncreaseTime(effectDuration);
        player->increaseDamage();
        kill();
    }
}

void IncreaseDamage::draw() {
    if (!alive) return;
    
    attron(A_BOLD | COLOR_PAIR(4));
    mvaddch(y, x, symbol);
    attroff(A_BOLD | COLOR_PAIR(4));
    
}