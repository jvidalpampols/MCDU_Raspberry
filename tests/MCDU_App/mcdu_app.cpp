// mcdu_app.cpp
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <string>
#include <vector>
#include <unistd.h> // sleep

using namespace std;

const int MCDU_ROWS = 14;
const int MCDU_COLS = 24;

const std::vector<std::wstring> lsk_left = {
    L"<LSK1L", L"<LSK1D", L"<LSK2L", L"<LSK2D", L"<LSK3L", L"<LSK3D", L"<LSK4L", L"<LSK4D", L"<LSK5L", L"<LSK5D", L"<LSK6L", L"<LSK6D"};

const std::vector<std::wstring> lsk_right = {
    L"LSK1L>", L"LSK1D>", L"LSK2L>", L"LSK2D>", L"LSK3L>", L"LSK3D>", L"LSK4L>", L"LSK4D>", L"LSK5L>", L"LSK5D>", L"LSK6L>", L"LSK6D>"};

void draw_mcdu_window(WINDOW *win, int row, int col)
{
    string strTitle = "A320 MCDU";

    // box(win, 0, 0);
    wattron(win, COLOR_PAIR(1));

    mvwprintw(win, 0, (col - strTitle.length()) / 2, strTitle.c_str()); // MCDU Title

    for (size_t i = 0; i < lsk_left.size(); ++i)
        mvwaddwstr(win, 1 + i, 0, lsk_left[i].c_str());

    for (size_t i = 0; i < lsk_right.size(); ++i)
        mvwaddwstr(win, 1 + i, col - lsk_right[i].size(), lsk_right[i].c_str());

    //Note we have 25 columns instead of 24
        mvwprintw(win, 13, 0, "0123456789012345678901234"); // Scratchpad

    wattroff(win, COLOR_PAIR(1));
    wrefresh(win);
}

int main()
{
    setlocale(LC_ALL, "");
    sleep(1); // Espera a que la terminal esté lista

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    // int row, col, rowMax, colMax;
    int rowMax, colMax;
    getmaxyx(stdscr, rowMax, colMax);
    mvwprintw(stdscr, 1, 1, "Cols: %d, Rows: %d", colMax, rowMax);
    mvwprintw(stdscr, 2, 2, "\nPress a key to follow");
    getch();

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
    }

    WINDOW *mcdu_win = newwin(rowMax, colMax, 0, 0);
    draw_mcdu_window(mcdu_win, rowMax, colMax);

    getch(); // Espera una tecla
    delwin(mcdu_win);
    endwin();
    return 0;
}
