//screenTools.h
#ifndef _SCREEN_TOOLS
#define _SCREEN_TOOLS

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
//#include <ncurses.h> /* ncurses.h includes stdio.h */
#include <ncursesw/ncurses.h>
#include <locale.h>
// #include <boost/algorithm/string.hpp>
// #include <algorithm>
using namespace std;

#define INVISIBLE 0 // Cursor hide
#define VISIBLE 1   // Cursor normal
#define HIGHLIGHT 2 // Cursor bright
#define SMALLFONT 1
#define BIGFONT 0

string strTrimLH(string str);                        // Remove spaces at beginning of string
string strTrimRH(string str);                        // Remove spaces at the end of string
void stringTrim(string &str);                        // Remove spaces at the beginning and end of string
string text_between_two(string str, char a, char b); // Get a substring between two chars
string getSubStringCharToEnd(string str, char c);

bool getTextCmd(string str);         // Get CMD to draw line
bool getTextString(string str);      // Get string to draw line
bool getTextColor(string str);       // Get Color to draw line
bool getTextFont(string str);        // Sets the global variable Font to draw line
bool getTextColPosition(string str); // Get string line position to draw line
void specialChars(string str);
void getLineGlobals(string str);
void clearTimer(void);
void mcduTimer(int t_sec);

// Line Globals

extern string cmdTxtLine;
extern string lineText;
extern string lineColor;
extern unsigned int lineFont; // Set font to default value
extern unsigned int textPosition; // Default value


// Colors defined on MCDU
//-------------------- COLORS ---------------------
//namespace MCDU_SCREEN
//{
    enum MCDUColors
    {
        WHITE,
        BLACK,
        BLUE,
        CYAN,
        GREEN,
        YELLOW,
        AMBER,
        RED,
        MAGENTA,
        GRAY
    }; // enum

    extern string listColors[];
   

    // Init enum of TEXT Commands index.
    enum txtCmds
    {
        TLL,
        TLC,
        TLR,
        _1LB,
        _1LD,
        _2LB,
        _2LD,
        _3LB,
        _3LD,
        _4LB,
        _4LD,
        _5LB,
        _5LD,
        _6LB,
        _6LD,
        _1RB,
        _1RD,
        _2RB,
        _2RD,
        _3RB,
        _3RD,
        _4RB,
        _4RD,
        _5RB,
        _5RD,
        _6RB,
        _6RD,
        _1CB,
        _1CD,
        _2CB,
        _2CD,
        _3CB,
        _3CD,
        _4CB,
        _4CD,
        _5CB,
        _5CD,
        _6CB,
        _6CD,
        PADL,
        PADC,
        PADR
    };

    // Init enum of PAGE Commands index.
    enum pageCmds
    {
        LSK1L,
        LSK2L,
        LSK3L,
        LSK4L,
        LSK5L,
        LSK6L,
        LSK1R,
        LSK2R,
        LSK3R,
        LSK4R,
        LSK5R,
        LSK6R,
        ARROWUP,
        ARROWDN,
        ARROWLH,
        ARROWRH
    }; 

    //} // namespace MCDU_SCREEN

    // Functions
    WINDOW *create_newwin(int height, int width, int starty, int startx);
    void destroy_win(WINDOW *local_win);
    void clearMCDU();
    void initMCDU();
    void initMCDUscreen();
    void initColors();
    void mcduTest();
    void drawPage(); // Takes the page stored into vector<string> pageLines;
    void drawLine(); // Draws line cheking the global variables stored
    string checkVars(string strLine); // Change lineText when contains special command @VAR(X);

    void testScreenTools(void); // Test MCDU with ncurses functions

    ////////////////////

    void writeTitle(string str, WINDOW *mcdu);
    void write1LB(string str, WINDOW *mcdu);
    void write1LD(string str, WINDOW *mcdu);
    void write2LB(string str, WINDOW *mcdu);
    void write2LD(string str, WINDOW *mcdu);
    void write3LB(string str, WINDOW *mcdu);
    void write3LD(string str, WINDOW *mcdu);

    void write4LB(string str, WINDOW *mcdu);
    void write4LD(string str, WINDOW *mcdu);
    void write5LB(string str, WINDOW *mcdu);
    void write5LD(string str, WINDOW *mcdu);
    void write6LB(string str, WINDOW *mcdu);
    void write6LD(string str, WINDOW *mcdu);

    void writePADL(string str, WINDOW *mcdu);
    void writePADC(string str, WINDOW *mcdu);
    void writePADR(string str, WINDOW *mcdu);
    void writePAD(string str, WINDOW *mcdu);

    void write1CB(string str, WINDOW *mcdu);
    void write1CD(string str, WINDOW *mcdu);
    void write2CB(string str, WINDOW *mcdu);
    void write2CD(string str, WINDOW *mcdu);
    void write3CB(string str, WINDOW *mcdu);
    void write3CD(string str, WINDOW *mcdu);
    void write4CB(string str, WINDOW *mcdu);
    void write4CD(string str, WINDOW *mcdu);
    void write5CB(string str, WINDOW *mcdu);
    void write5CD(string str, WINDOW *mcdu);
    void write6CB(string str, WINDOW *mcdu);
    void write6CD(string str, WINDOW *mcdu);

    void write1RB(string str, WINDOW *mcdu);
    void write1RD(string str, WINDOW *mcdu);
    void write2RB(string str, WINDOW *mcdu);
    void write2RD(string str, WINDOW *mcdu);
    void write3RB(string str, WINDOW *mcdu);
    void write3RD(string str, WINDOW *mcdu);
    void write4RB(string str, WINDOW *mcdu);
    void write4RD(string str, WINDOW *mcdu);
    void write5RB(string str, WINDOW *mcdu);
    void write5RD(string str, WINDOW *mcdu);
    void write6RB(string str, WINDOW *mcdu);
    void write6RD(string str, WINDOW *mcdu);

    // Variables
    extern string strTitle;
    extern string str1LB;
    extern string str1LD;
    extern string str2LB;
    extern string str2LD;
    extern string str3LB;
    extern string str3LD;
    extern string str4LB;
    extern string str4LD;
    extern string str5LB;
    extern string str5LD;
    extern string str6LB;
    extern string str6LD;

    extern string str1CB;
    extern string str1CD;
    extern string str2CB;
    extern string str2CD;
    extern string str3CB;
    extern string str3CD;
    extern string str4CB;
    extern string str4CD;
    extern string str5CB;
    extern string str5CD;
    extern string str6CB;
    extern string str6CD;

    extern string str1RB;
    extern string str1RD;
    extern string str2RB;
    extern string str2RD;
    extern string str3RB;
    extern string str3RD;
    extern string str4RB;
    extern string str4RD;
    extern string str5RB;
    extern string str5RD;
    extern string str6RB;
    extern string str6RD;

    extern string strPADL;
    extern string strPADC;
    extern string strPADR;
    extern string strPAD;

    extern WINDOW *mcdu;
    // int mcdu_rows = 14;
    // int mcdu_cols = 28;
    extern int row, col, max_x, max_y; /* to store the number of rows and maximun x, y values*/
    extern int startx, starty, width, height;

#endif

//////////////