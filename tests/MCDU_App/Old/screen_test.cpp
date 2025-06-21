// screen_test.cpp
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <string>
#include <vector>

// Dimensiones estándar del MCDU
const int MCDU_ROWS = 14;
const int MCDU_COLS = 24;
int row = 0;
int col = 0;

// LSK Labels
const std::vector<std::wstring> lsk_left = {
    L"<LSK1L", L"<LSK2L", L"<LSK3L", L"<LSK4L", L"<LSK5L", L"<LSK6L"};

const std::vector<std::wstring> lsk_right = {
    L"<LSK1R", L"<LSK2R", L"<LSK3R", L"<LSK4R", L"<LSK5R", L"<LSK6R"};
WINDOW *mcdu_win;

// Draw window
void draw_mcdu_window(WINDOW *win)
{
    // Dibuja borde
    box(win, 0, 0);
    // Color del texto principal
    wattron(win, COLOR_PAIR(1));

    // Título centrado

    mvwprintw(win, 1, (COLS - 11) / 2, " A320 MCDU "); // 11 is the length of " A320 MCDU "

    // Línea inferior
    // mvwprintw(win, MCDU_ROWS - 1, 1, "MAINTENANCE MODE");
    // mvwprintw(win, LINES - 1, 1, "MAINTENANCE MODE");

    // Etiquetas LSK izquierdas
    for (size_t i = 0; i < lsk_left.size(); ++i)
    {
        mvwaddwstr(win, 1 + i * 2, 1, lsk_left[i].c_str());
    }

    // Etiquetas LSK derechas
    for (size_t i = 0; i < lsk_right.size(); ++i)
    {
        // mvwaddwstr(win, 1 + i * 2, MCDU_COLS - 10, lsk_right[i].c_str());
        mvwaddwstr(win, 1 + i * 2, COLS - 10, lsk_right[i].c_str());
    }
    wrefresh(win);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}

int main()
{
    setlocale(LC_ALL, ""); // Soporte Unicode
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK); // Texto verde
    }

    getmaxyx(stdscr, row, col);

    mvwprintw(stdscr, 0, (MCDU_COLS - 11) / 2, " A320 MCDU ");
    mvwprintw(stdscr, 1, 1, "COLS: %d", COLS);
    mvwprintw(stdscr, 2, 1, "LINES: %d", LINES);
    mvwprintw(stdscr, 3, 1, "Press any key");

    getch();
    clear();

    // Create a window
    // WINDOW* mcdu_win = newwin(MCDU_ROWS, MCDU_COLS, starty, startx);
    mcdu_win = newwin(LINES, COLS, 0, 0);
    draw_mcdu_window(mcdu_win);
    wrefresh(mcdu_win);
   

    // getmaxyx(stdscr, row, col); /* get the number of rows and columns */
    // mvprintw(1, 1, "Lineas:%d \t Columnas:%d", row, col);
   
    getch();  

    // Esperar una tecla para salir
    getch();

    delwin(mcdu_win);
    endwin();
    return 0;
}
