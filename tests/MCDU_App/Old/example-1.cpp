//example-1.cpp
#include <ncurses.h>
using namespace std;

int main(int argc, char **argv)
{
    initscr();

    // creating a window;
    // with height = 15 and width = 10
    // also with start x axis 10 and start y axis = 20
    //WINDOW* mcdu_win = newwin(MCDU_ROWS, MCDU_COLS, starty, startx);
    //WINDOW *win = newwin(15, 17, 2, 10);
    WINDOW *win = newwin(LINES, COLS, 0, 0);
    refresh();

    // making box border with default border styles
    box(win, 0, 0);

    // move and print in window
    mvwprintw(win, 1, 1, "Greeter");
    mvwprintw(win, 2, 1, "Hello");

    // refreshing the window
    wrefresh(win);
    getch();
    getch();
    endwin();
    return 0;
}