#pragma once

namespace Config {
    const int SCREEN_WIDTH = 40;
    const int SCREEN_HEIGHT = 16;
    const int GAME_SPEED_MS = 50; 
    
    const char PLAYER_SYMBOL = 'A';
    const char ENEMY_SYMBOL = 'E';
    const char BULLET_SYMBOL = '.';
    const char WALL_SYMBOL = '#';
    const char BRICK_SYMBOL = '%';
    const char EMPTY_SYMBOL = ' ';
    
    enum Direction { UP, RIGHT, DOWN, LEFT };
}