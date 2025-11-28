#include <curses.h>

int main() {
    initscr();
    noecho();
    cbreak();

    if (!has_colors()) {
        endwin();
        printf("PDCurses: терминал НЕ поддерживает цвета\n");
        return 1;
    }

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    clear();

    mvprintw(0, 0, "COLORS = %d", COLORS);
    mvprintw(1, 0, "COLOR_PAIRS = %d", COLOR_PAIRS);

    attron(COLOR_PAIR(1));
    mvprintw(3, 0, "Красный текст");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, 0, "Зеленый жирный текст");
    attroff(COLOR_PAIR(2) | A_BOLD);

    attron(COLOR_PAIR(3));
    mvprintw(5, 0, "Желтый текст");
    attroff(COLOR_PAIR(3));

    refresh();
    getch();
    endwin();
    return 0;
}
