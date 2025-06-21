// mcdu_app-1.cpp
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <string.h>
#include <string>
#include <vector>
using namespace std;

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

    string strTitle = "A320 MCDU";
    // Dibuja borde
    box(win, 0, 0);

    // Color del texto principal
    wattron(win, COLOR_PAIR(1));

    // Título centrado
    mvwprintw(win, 0, (col - (strTitle.size() / 2)), strTitle.c_str());
    //mvwprintw(win, 0, (MCDU_COLS - 11) / 2, " A320 MCDU ");
    //    mvwprintw(win, 1, 1, "COLS: %d" , COLS);
    //    mvwprintw(win, 2, 1, "LINES: %d" , LINES);

    
    // Etiquetas LSK izquierdas
    for (size_t i = 0; i < lsk_left.size(); ++i) // i=1 because Title shoulbe at the top of window
    {
        mvwaddwstr(win, 1 + i * 2, 1, lsk_left[i].c_str());
    }

    // Etiquetas LSK derechas
    for (size_t i = 0; i < lsk_right.size(); ++i)
    {
        // mvwaddwstr(win, 1 + i * 2, MCDU_COLS - 10, lsk_right[i].c_str());
        mvwaddwstr(win, 1 + i * 2, (col - lsk_right[i].size()), lsk_right[i].c_str());
    }

    
    // Línea inferior
    mvwprintw(win, row-1, 0, "123456789012345678901234");
    wrefresh(win);
    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
    getch();
}

int main()
{
    setlocale(LC_ALL, ""); // Soporte Unicode
    initscr();
    
    clear(); //Make sure no residual windows
    refresh();

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

    //getmaxyx(stdscr, LINES, COLS); /* get the number of rows and columns */
    mvprintw(1, 1, "Lineas:%d \t Columnas:%d", row, col);
    refresh();
    getch();
    clear();

    // Create a window
    // WINDOW* mcdu_win = newwin(MCDU_ROWS, MCDU_COLS, starty, startx);
    mcdu_win = newwin(row, col, 0, 0);
    draw_mcdu_window(mcdu_win);
    wrefresh(mcdu_win);

    // Esperar una tecla para salir
    getch();
    getch();

    delwin(mcdu_win);
    endwin();
    return 0;
}
