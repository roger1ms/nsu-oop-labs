#include "Game.h"
#include <curses.h>
#include <algorithm>

Game::Game() : initialized(false), bonusSpawnTimer(0), activeBonuses(0), nextSpawnTime((2) * 20){}

void Game::initializeGame() {
    if (initialized) return;

    const int W = 40;
    const int H = 16;

    int playerX = W / 2;
    int playerY = H - 2;
    context.addObject(std::make_unique<MyTank>(playerX, playerY));

    // Разрушаемые стены вокруг игрока
    context.addObject(std::make_unique<DestroyableWall>(playerX - 1, playerY));
    context.addObject(std::make_unique<DestroyableWall>(playerX + 1, playerY));
    context.addObject(std::make_unique<DestroyableWall>(playerX, playerY - 1));

    // Враги
    context.addObject(std::make_unique<EnemyTank>(2, 2));
    context.addObject(std::make_unique<EnemyTank>(W - 3, 2));
    context.addObject(std::make_unique<EnemyTank>(W / 2, 2));

    for (int x = 1; x < W; x++) {
        context.addObject(std::make_unique<Wall>(x, 1));
        context.addObject(std::make_unique<Wall>(x, H));
    }
    for (int y = 2; y <= H - 1; y++) {
        context.addObject(std::make_unique<Wall>(1, y));
        context.addObject(std::make_unique<Wall>(W - 1, y));
    }

    // ?????????????????????????????
    // ЦЕНТРАЛЬНАЯ КРЕПОСТЬ 9?5
    // ?????????????????????????????
    int cx1 = W/2 - 4;
    int cx2 = W/2 + 4;
    int cy1 = H/2 - 2;
    int cy2 = H/2 + 2;

    for (int x = cx1; x <= cx2; x++) {
        for (int y = cy1; y <= cy2; y++) {

            bool border =
                x == cx1 || x == cx2 ||
                y == cy1 || y == cy2;

            if (!border) continue;

            // Оставляем 4 входа
            if ((x == W/2 && y == cy1) ||
                (x == W/2 && y == cy2) ||
                (y == H/2 && x == cx1) ||
                (y == H/2 && x == cx2))
                continue;

            context.addObject(std::make_unique<Wall>(x, y));
        }
    }

    // ?????????????????????????????
    // ЛЕВЫЙ ЛАБИРИНТ
    // ?????????????????????????????
    for (int y = 3; y <= H - 2; y++) {
        if (y % 2 == 1) continue;
        for (int x = 3; x <= 10; x++) {
            if (x % 2 == 0)
                context.addObject(std::make_unique<Wall>(x, y));
        }
    }

    // ?????????????????????????????
    // ПРАВЫЙ ЛАБИРИНТ
    // ?????????????????????????????
    for (int y = 4; y <= H - 3; y += 3) {
        for (int x = W - 12; x <= W - 3; x++) {
            if ((x + y) % 3 == 0) {
                context.addObject(std::make_unique<Wall>(x, y));
            }
        }
    }

    // ?????????????????????????????
    // СЕРЕДИННЫЕ УКРЫТИЯ (ДЕТЕРМИНИРОВАННЫЕ)
    // ?????????????????????????????
    int midx[] = { 12, 14, 17, 20, 23, 25, 27 };
    int midy[] = { 4, 6, 8, 10 };

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 4; j++) {
            if ((i + j) % 2 == 0)
                context.addObject(std::make_unique<DestroyableWall>(midx[i], midy[j]));
        }
    }

    initialized = true;
}

void Game::render() const {
    for (const auto& obj : context.getAllObjects()) {
        if (obj->isAlive())
            obj->draw();
    }
}

void Game::renderUI() const {
    int uiBase = Config::SCREEN_HEIGHT + 2;

    // Глобальный счет игры
    mvprintw(uiBase, 0, "Global Score: %d", context.getScore());
    
    // Поиск лидера и максимального счета
    int maxScore = 0;
    const char* leaderName = "No one";
    bool playerIsLeader = false;
    
    for (const auto& obj : context.getAllObjects()) {
        if (auto* tank = dynamic_cast<Tank*>(obj.get())) {
            if (tank->getScore() > maxScore) {
                maxScore = tank->getScore();
                leaderName = dynamic_cast<MyTank*>(tank) ? "Player" : "Enemy";
                playerIsLeader = dynamic_cast<MyTank*>(tank);
            }
        }
    }
    
    // Прогресс-бар к победе
    mvprintw(uiBase, 25, "Leader: ");
    if (playerIsLeader) {
        attron(COLOR_PAIR(2)); // Зеленый для игрока
    } else {
        attron(COLOR_PAIR(1)); // Красный для врага
    }
    printw("%s", leaderName);
    attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
    printw(" [%d/1000]", maxScore);
    
    // Визуальный прогресс-бар
    mvprintw(uiBase + 1, 25, "[");
    int progress = (maxScore * 20) / 1000; // 20 символов для прогресс-бара
    for (int i = 0; i < 20; i++) {
        if (i < progress) {
            if (playerIsLeader) {
                attron(COLOR_PAIR(2)); // Зеленый
            } else {
                attron(COLOR_PAIR(1)); // Красный
            }
            addch('=');
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
        } else {
            addch(' ');
        }
    }
    printw("]");
    
    // Информация о бонусах
    mvprintw(uiBase, 50, "Bonuses: %d/%d", countActiveBonuses(), MAX_BONUSES);
    
    // Информация об игроке
    int infoLine = uiBase + 2;
    for (const auto& obj : context.getAllObjects()) {
        if (auto* player = dynamic_cast<MyTank*>(obj.get())) {
            // Очки игрока
            mvprintw(infoLine, 0, "Player Score: %d", player->getScore());
            
            // Состояние игрока
            if (!player->isAlive()) {
                int respawnTime = 10 - (player->getRespawnTimer() / 20);
                mvprintw(infoLine + 1, 0, "Respawn in: %d seconds", std::max(0, respawnTime));
                attron(COLOR_PAIR(1)); // Красный
                mvprintw(infoLine + 2, 0, "DESTROYED!");
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2)); // Зеленый
                mvprintw(infoLine + 1, 0, "ALIVE");
                attroff(COLOR_PAIR(2));
            }
            
            // Статусы усилений
            if (player->getStrength() > 1) {
                attron(A_BOLD | COLOR_PAIR(3)); // Желтый жирный
                mvprintw(infoLine + 3, 0, "DAMAGE BOOST ACTIVE!");
                attroff(A_BOLD | COLOR_PAIR(3));
            }
            
            if (player->isFrozen()) {
                attron(A_BOLD | COLOR_PAIR(4)); // Голубой жирный
                mvprintw(infoLine + 4, 0, "FROZEN!");
                attroff(A_BOLD | COLOR_PAIR(4));
            }
            break;
        }
    }
    
    // Список всех танков и их очки (справа)
    int rightColumn = 50;
    int tankListLine = uiBase + 2;
    mvprintw(tankListLine, rightColumn, "=== ALL TANKS ===");
    tankListLine++;
    
    for (const auto& obj : context.getAllObjects()) {
        if (auto* tank = dynamic_cast<Tank*>(obj.get())) {
            const char* tankType = "Enemy";
            int colorPair = 1; // Красный по умолчанию
            
            if (dynamic_cast<MyTank*>(tank)) {
                tankType = "Player";
                colorPair = 2; // Зеленый
            }
            
            attron(COLOR_PAIR(colorPair));
            mvprintw(tankListLine, rightColumn, "%s: %d points", tankType, tank->getScore());
            attroff(COLOR_PAIR(colorPair));
            
            // Статус жив/мертв
            if (tank->isAlive()) {
                attron(COLOR_PAIR(2));
                mvprintw(tankListLine, rightColumn + 25, "ALIVE");
                attroff(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(1));
                mvprintw(tankListLine, rightColumn + 25, "DEAD");
                attroff(COLOR_PAIR(1));
            }
            
            // Предупреждение о близости к победе
            if (tank->getScore() >= 800) {
                attron(A_BOLD | COLOR_PAIR(3));
                mvprintw(tankListLine, rightColumn + 32, "NEAR WIN!");
                attroff(A_BOLD | COLOR_PAIR(3));
            }
            
            tankListLine++;
        }
    }
    
    // Сообщения о состоянии игры
    int messageLine = uiBase + 1;
    if (!context.isGameRunning()) {
        if (context.isGameWon()) {
            attron(A_BOLD | COLOR_PAIR(2));
            mvprintw(messageLine, 0, "VICTORY! Player reached 1000 points!");
            attroff(A_BOLD | COLOR_PAIR(2));
        } else {
            attron(A_BOLD | COLOR_PAIR(1));
            mvprintw(messageLine, 0, "DEFEAT! Press any key to exit");
            attroff(A_BOLD | COLOR_PAIR(1));
        }
    } else {
        // Управление внизу
        mvprintw(messageLine, 0, "Controls: Arrows=Move | Space=Shoot | Goal: 1000 points");
        
        // Предупреждение, если враг близок к победе
        for (const auto& obj : context.getAllObjects()) {
            if (auto* enemy = dynamic_cast<EnemyTank*>(obj.get())) {
                if (enemy->getScore() >= 800 && enemy->isAlive()) {
                    attron(A_BOLD | COLOR_PAIR(1));
                    mvprintw(messageLine + 1, 0, "WARNING: Enemy is close to victory!");
                    attroff(A_BOLD | COLOR_PAIR(1));
                    break;
                }
            }
        }
    }
    
    
}

void Game::controlFrameRate() const {
    napms(50);
}

void Game::run() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    scrollok(stdscr, FALSE);
    initscr();
    start_color();
    use_default_colors();
    
    init_pair(1, COLOR_RED, -1);     // красный 
    init_pair(2, COLOR_GREEN, -1);   // зелёный
    init_pair(3, COLOR_YELLOW, -1);  // жёлтый
    init_pair(4, COLOR_CYAN, -1);    // голубой

    if (!initialized)
        initializeGame();

    while (context.isGameRunning()) {

        context.updateInput();
        updateBonusSpawning();

        for (auto& obj : context.getAllObjects())
            obj->update(context);

        context.removeDeadObjects();

        for (const auto& obj : context.getAllObjects()) {
            if (auto* tank = dynamic_cast<Tank*>(obj.get())) {
                if (tank->isAlive() && tank->hasWinningScore()) {
                    context.endGame(true);
                    break;
                }
            }
        }

        erase();     // только один раз
        render();       // рисуем игру
        renderUI();     // рисуем UI
        refresh();      // один refresh

        controlFrameRate();
    }

    nodelay(stdscr, FALSE);
    getch();
    endwin();
}


void Game::renderFinalScreen() const {
    int centerX = Config::SCREEN_WIDTH / 2;
    int line = 5;
    
    // Заголовок
    if (context.isGameWon()) {
        attron(A_BOLD | COLOR_PAIR(2)); // Зеленый
        mvprintw(line, centerX - 10, "=== VICTORY ===");
        attroff(A_BOLD | COLOR_PAIR(2));
    } else {
        attron(A_BOLD | COLOR_PAIR(1)); // Красный
        mvprintw(line, centerX - 8, "=== DEFEAT ===");
        attroff(A_BOLD | COLOR_PAIR(1));
    }
    
    line += 2;
    
    // Таблица результатов
    mvprintw(line, centerX - 15, "FINAL RESULTS:");
    line++;
    
    // Собираем всех танков и сортируем по очкам
    std::vector<Tank*> tanks;
    for (const auto& obj : context.getAllObjects()) {
        if (auto* tank = dynamic_cast<Tank*>(obj.get())) {
            tanks.push_back(tank);
        }
    }
    
    // Сортируем по убыванию очков
    std::sort(tanks.begin(), tanks.end(), 
        [](Tank* a, Tank* b) { return a->getScore() > b->getScore(); });
    
    // Выводим таблицу
    mvprintw(line, 10, "PLAYER");
    mvprintw(line, 30, "SCORE");
    mvprintw(line, 40, "STATUS");
    line++;
    
    for (auto* tank : tanks) {
        const char* name = "Enemy";
        const char* status = tank->isAlive() ? "Alive" : "Destroyed";
        
        if (dynamic_cast<MyTank*>(tank)) {
            name = "Player";
            attron(COLOR_PAIR(2)); // Зеленый для игрока
        } else {
            attron(COLOR_PAIR(1)); // Красный для врагов
        }
        
        mvprintw(line, 10, "%s", name);
        mvprintw(line, 30, "%d", tank->getScore());
        mvprintw(line, 40, "%s", status);
        
        // Подсвечиваем победителя
        if (tank->hasWinningScore()) {
            attron(A_BOLD);
            mvprintw(line, 50, "WINNER!");
            attroff(A_BOLD);
        }
        
        attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
        line++;
    }
    
    line += 2;
    mvprintw(line, centerX - 15, "Press any key to exit...");
    
    // Дополнительная информация
    line++;
    mvprintw(line, 5, "Total game score: %d", context.getScore());
}

int Game::countActiveBonuses() const {
    int count = 0;
    for (const auto& obj : context.getAllObjects()) {
        if (obj->isAlive() && 
            (dynamic_cast<StopClock*>(obj.get()) || 
             dynamic_cast<IncreaseDamage*>(obj.get()))) {
            count++;
        }
    }
    return count;
}

void Game::updateBonusSpawning() {
    bonusSpawnTimer++;
    
    int currentBonuses = countActiveBonuses();
    
    if (bonusSpawnTimer >= nextSpawnTime && currentBonuses < MAX_BONUSES) {
        spawnRandomBonus();
        
        bonusSpawnTimer = 0;
        nextSpawnTime = (rand() % 11 + 10) * 20;
    }
}

void Game::spawnRandomBonus() {
    int maxAttempts = 50; 
    int spawnX, spawnY;
    bool positionFound = false;
    
    for (int attempt = 0; attempt < maxAttempts; attempt++) {
        spawnX = rand() % (Config::SCREEN_WIDTH - 4) + 2;
        spawnY = rand() % (Config::SCREEN_HEIGHT - 4) + 2;
        
        if (context.isPositionFree(spawnX, spawnY)) {
            positionFound = true;
            break;
        }
    }
    
    if (positionFound) {
        if (rand() % 2 == 0) {
            context.addObject(std::make_unique<StopClock>(spawnX, spawnY));
        } else {
            context.addObject(std::make_unique<IncreaseDamage>(spawnX, spawnY));
        }
    }
}