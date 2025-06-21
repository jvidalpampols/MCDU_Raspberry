// screenTools.cpp

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <stdlib.h>
#include <ncurses.h> /* ncurses.h includes stdio.h */
#include "fileTools.h"
#include <curses.h>
#include <ncursesw/curses.h>

#include <locale.h>
#include <wchar.h>
#include <string>

using namespace std;
#define SMALLFONT 1
#define BIGFONT 0

///////////////////////////  START Old screenTools.cpp /////////////////

#define INVISIBLE 0 // Cursor hide
#define VISIBLE 1   // Cursor normal
#define HIGHLIGHT 2 // Cursor bright

#define COLOR_AMBER 6 // To match enum MCDUColors
#define COLOR_GRAY 9  // To match enum MCDUColors

//---------------------Variables-----------------------

//-------------------- COLORS ---------------------
// namespace MCDU_SCREEN
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

string listColors[] = {"WHITE", "BLACK", "BLUE", "CYAN", "GREEN", "YELLOW", "AMBER", "RED", "MAGENTA", "GRAY"};

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

void testScreenTools(void); // Test MCDU with ncurses functions

//------------------- Write MCDU screen START -----------------------------
void writeTitle(string str);
void writeTLL(string str);
void writeTLC(string str);
void writeTLR(string str);

void write1LB(string str);
void write1LD(string str);
void write2LB(string str);
void write2LD(string str);
void write3LB(string str);
void write3LD(string str);

void write4LB(string str);
void write4LD(string str);
void write5LB(string str);
void write5LD(string str);
void write6LB(string str);
void write6LD(string str);

void write1RB(string str);
void write1RD(string str);
void write2RB(string str);
void write2RD(string str);
void write3RB(string str);
void write3RD(string str);
void write4RB(string str);
void write4RD(string str);
void write5RB(string str);
void write5RD(string str);
void write6RB(string str);
void write6RD(string str);

void write1CB(string str);
void write1CD(string str);
void write2CB(string str);
void write2CD(string str);
void write3CB(string str);
void write3CD(string str);
void write4CB(string str);
void write4CD(string str);
void write5CB(string str);
void write5CD(string str);
void write6CB(string str);
void write6CD(string str);

void writePADL(string str);
void writePADC(string str);
void writePADR(string str);
void writePAD(string str);

// void (*writeMCDU[])(string str) = {
//     writeTLL, writeTLC, writeTLR,
//     write1LB, write1LD, write2LB, write2LD, write3LB, write3LD, write4LB, write4LD, write5LB, write5LD, write6LB, write6LD,
//     write1RB, write1RD, write2RB, write2RD, write3RB, write3RD, write4RB, write4RD, write5RB, write5RD, write6RB, write6RD,
//     write1CB, write1CD, write2CB, write2CD, write3CB, write3CD, write4CB, write4CD, write5CB, write5CD, write6CB, write6CD,
//     writePADL, writePADC, writePADR};

//---------------------------------------Write MCDU screen  END --------------------------

string strTrimLH(string str);                        // Remove spaces at beginning of string
string strTrimRH(string str);                        // Remove spaces at the end of string
void stringTrim(string &str);                        // Remove spaces at the beginning and end of string
string text_between_two(string str, char a, char b); // Get a substring between two chars
string getSubStringCharToEnd(string str, char c);

bool getTextCmd(string str);         // Get CMD to draw line
bool getTextString(string str);      // Get string to draw line
string getSpecialChars(string str);
bool getTextColor(string str);       // Get Color to draw line
bool getTextFont(string str);        // Sets the global variable Font to draw line
bool getTextColPosition(string str); // Get string line position to draw line
//void specialChars(string str);
void getLineGlobals(string str);

//-------------------------------Variables-----------------------
// Line Globals
string cmdTxtLine;
string lineText;
string lineColor;
unsigned int lineFont = BIGFONT; // Set font to default value
int textPosition = 0;            // Default value

// ncurses Globals
int mcdu_rows = 14;
int mcdu_cols = 25;
int row, col, max_x, max_y; /* to store the number of rows and maximun x, y values*/
int startx, starty, width, height;

// Variables to test ncurses library
string strTitle = "A340-300";
string str1LB = "1LB";
string str1LD = "1LD";
string str2LB = "2LB";
string str2LD = "2LD";
string str3LB = "3LB";
string str3LD = "3LD";
string str4LB = "4LB";
string str4LD = "4LD";
string str5LB = "5LB";
string str5LD = "5LD";
string str6LB = "6LB";
string str6LD = "6LD";

string str1CB = "1CB";
string str1CD = "1CD";
string str2CB = "2CB";
string str2CD = "2CD";
string str3CB = "3CB";
string str3CD = "3CD";
string str4CB = "4CB";
string str4CD = "4CD";
string str5CB = "5CB";
string str5CD = "5CD";
string str6CB = "6CB";
string str6CD = "6CD";

string str1RB = "1RB";
string str1RD = "1RD";
string str2RB = "2RB";
string str2RD = "2RD";
string str3RB = "3RB";
string str3RD = "3RD";
string str4RB = "4RB";
string str4RD = "4RD";
string str5RB = "5RB";
string str5RD = "5RD";
string str6RB = "6RB";
string str6RD = "6RD";

string strPADL = "PADL";
string strPADC = "PADC";
string strPADR = "PADR";
string strPAD = "123456789012345678901234";
string str0Title = "                        ";
string str0 = "        ";

WINDOW *mcdu; // MCDU Subwindow

///////////////////////////// Functions Definitions /////////////////////////////

//------------------End Line string management ---------------------

string strTrimLH(string str) // Remove spaces at beginning of string
{
    for (std::size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ')
        {
            str.erase(i, 1);
            i--;
        }
        else
        {
            if (str[i] != ' ')
                break;
        }
    }
    return str;
}

string strTrimRH(string str) // Remove spaces at the end of string
{
    for (std::size_t i = str.size() - 1; i > 0; i--)
    {
        if (str[i] == ' ')
        {
            str.erase(i, 1);
        }

        else
        {
            if (str[i] != ' ')
                break;
        }
    }
    return str;
}

void stringTrim(string &str)
{
    // This function removes left and right spaces
    int i = 0;

    // left trim
    while (isspace(str[i]) != 0)
        i++;
    str = str.substr(i, str.length() - i);

    // right trim
    i = str.length() - 1; // string index starts with '0', not '1'
    while (isspace(str[i]) != 0)
        i--;
    str = str.substr(0, i + 1); // str,substr(start_index, str_length), so lenth = (i + 1)
}

// ------------------------- Functions to get and store line globals  START ---------------------------------

bool getTextCmd(string str)
//
{
    // Gets the string "cmdTxtLine" and store into line global: "cmdTxtLine";
    // returns "true" if "cmdTxtLine" found.
    // else returns "false"

    // Text Command Strings to check:
    string strTitle("TL");
    string strLB("LB");
    string strLD("LD");
    string strRB("RB");
    string strRD("RD");
    string strPAD("PAD");
    std::size_t found;
    std::size_t pos;

    // Remove trailing and leading spaces
    // cout << "\nBefore removing spaces:" << str;
    str = strTrimLH(str);
    str = strTrimRH(str);

    // cout << "\nAfter removing spaces:" << str << "<This is the end of string";

    // Find first '=' from beginning of string=  0
    /*
    size_t find (char c, size_t pos = 0) const;
    */
    pos = str.find('=', 0);
    if (pos != std::string::npos)
    {
        str = str.substr(0, pos); // Get the string
        str = strTrimLH(str);     // Remove spaces on left side
        str = strTrimRH(str);     // Remove spaces on right side
    }

    else
    {
        return false;
    }

    found = str.find(strTitle);
    if (found != std::string::npos)
    {
        cmdTxtLine = str.c_str(); // Store Text Command into global variable
        return true;
    }

    found = str.find(strLB);
    if (found != std::string::npos)
    {
        str.insert(0, 1, '_');
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    found = str.find(strLD);
    if (found != std::string::npos)
    {
        str.insert(0, 1, '_');
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    found = str.find(strRB);
    if (found != std::string::npos)
    {
        str.insert(0, 1, '_');
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    found = str.find(strRD);
    if (found != std::string::npos)
    {
        str.insert(0, 1, '_');
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    found = str.find(strPAD);
    if (found != std::string::npos)
    {
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    return false;
}

bool getTextString(string str) // Get string to draw line
{

    // Gets the string "text" of MCDU line and store into line global: "lineText";
    // returns "true" if "cmdTxtLine" found.
    //  else returns "false"

    // Check for '=' and ','
    string strText = str;
    strText = text_between_two(str, '=', ',');
    if (strText != "FAIL")
    {
        strText = strTrimLH(strText); // Remove spaces on left side
        strText = strTrimRH(strText); // Remove spaces on right side
        strText = getSpecialChars(strText);
        lineText = strText; // Store Global
        return true;
    }

    // Check for '=' and end of string
    strText = str;
    strText = getSubStringCharToEnd(strText, '=');

    if (strText != "FAIL")
    {
        strText = strTrimLH(strText); // Remove spaces on left side
        strText = strTrimRH(strText); // Remove spaces on right side
        strText = getSpecialChars(strText);
        lineText = strText; // Store Global
        return true;
    }

    ///////////////

    return false;
}


bool getTextStringOld(string str) // Get string to draw line
{

    // Gets the string "text" of MCDU line and store into line global: "lineText";
    // returns "true" if "cmdTxtLine" found.
    //  else returns "false"

    // Check for '=' and ','
    string strText = str;
    strText = text_between_two(str, '=', ',');
    if (strText != "FAIL")
    {
        strText = strTrimLH(strText); // Remove spaces on left side
        strText = strTrimRH(strText); // Remove spaces on right side
        lineText = strText;           // Store Global
        return true;
    }

    // strText = str;
    // strText = text_between_two(strText, '=', ' ');
    // if (strText != "FAIL")
    // {
    //     strText = strTrimLH(strText); // Remove spaces on left side
    //     strText = strTrimRH(strText); // Remove spaces on right side
    //     lineText = strText;           // Store Global
    //     return true;
    // }

    // size_t strLen = str.length();
    // str = text_between_two(str, '=', 'str[strlen-1]');
    // if (str != "FAIL")
    // {
    //     str = strTrimLH(str); // Remove spaces on left side
    //     str = strTrimRH(str); // Remove spaces on right side
    //     lineText = str; //Store Global
    //     return true;
    // }

    //////////////
    // Check for '=' and end of string
    strText = str;
    strText = getSubStringCharToEnd(strText, '=');

    if (strText != "FAIL")
    {
        strText = strTrimLH(strText); // Remove spaces on left side
        strText = strTrimRH(strText); // Remove spaces on right side

        lineText = strText; // Store Global
        return true;
    }

    ///////////////

    return false;
}

string text_between_two(string str, char a, char b)
// Get a substring of string between two chars 'a' and  'b'
//  start_char 'a' end_char 'b'
// Returns the substring  or "false" if fail to found the first char 'a'
// Will return "FAIL" if the first char 'a' is found, but not the end char 'b'
// If returns "FAIL", then call to string getSubStringCharToEnd(string str, char c)
// This function will return a substring starting from char c, until end of string
// or false if fails
{
    std::size_t aPos;
    std::size_t bPos;
    std::string sub;

    aPos = str.find(a, 0); // Search from beginning
    aPos = aPos + 1;       // To skip '='
    if (aPos == std::string::npos)
    {
        return "FAIL";
    }

    bPos = str.find(b, aPos); // Search from aPos
    if (bPos == std::string::npos)
    {
        return "FAIL";
    }

    // Get substring
    sub = str.substr(aPos, (bPos - aPos));
    return sub;
}

string getSubStringCharToEnd(string str, char c)
{

    // string subString;
    //size_t strLen;
    size_t cPos;
    //strLen = str.length();
    // string substr (size_t pos = 0, size_t len = npos) const;

    cPos = str.find(c, 0); // Search from beginning

    if (cPos == std::string::npos)
    {
        return "FAIL";
    }
    cPos = cPos + 1;                    // To skip '='
    string strTitle = str.substr(cPos); // Get from '=' to the end
    return strTitle;
}

string getSpecialCharsOld(string str)
{
    // Replace special characters as defined on MCDU manual

    //---------------------Variables used
    std::size_t found;
    std::string strCheck = str;
    // string str1 = "If found: ^ Change for arrowUp: ";
    // string str2 = "If found: ~ Change for arrowDown: ";
    // string str3 = "If found: _ Change for arrowLeft: ";
    // string str4 = "If found: $ Change for arrowRight: ";
    // string str5 = "If found: # Change for amberBox: ";

    string sChar = "⬔";
    string arrowUp = "↑";
    string arrowDown = "↓";
    string arrowLeft = "←";
    string arrowRight = "→";
    string amberBox = "⬜";

    string strArrowUpCheck = "^";
    string strArrowDownCheck = "~";
    string strArrowLeftCheck = "_";
    string strArrowRightCheck = "$";
    string strAmberBoxCheck = "#";

    // found = str.find('^');
    // if (found != std::string::npos)
    // {
    //     // string (1) string &replace(size_t pos, size_t len, const string &str);
    //     // pos: Position of the first character to be replaced.
    //     // len: Number of characters to replace
    //     if (found != std::string::npos)
    //     {
    //         str = strCheck.replace(found, 1, '↑');
    //     }
    // }

    // Check for ArrowUP
    found = strCheck.find(strArrowUpCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowUp);
        return str.c_str();
    }
    else
        found = std::string::npos;//Reset found

    // Check for ArrowDown
    found = strCheck.find(strArrowDownCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowDown);
        return str.c_str();
    }
    else
        found = std::string::npos; // Reset found

    // Check for ArrowLeft
    found = strCheck.find(strArrowLeftCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowLeft);
        return str.c_str();
    }
    else
        found = std::string::npos; // Reset found

    // Check for ArrowRight
    found = strCheck.find(strArrowRightCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowRight);
        return str.c_str();
    }
    else
        found = std::string::npos; // Reset found

    // Check for amberBox
    found = strCheck.find(strAmberBoxCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, amberBox);
        return str.c_str();
    }
    else
        found = std::string::npos; // Reset found

    return str;
} // string getSpecialChars(string str)


//-------------------------OLD------------------------
string getSpecialChars(string str)
{
    // Replace special characters as defined on MCDU manual
    std::size_t found;
    string arrowUp = "\u2191";    //':^'
    string arrowDown = "\u2193";  //':~'
    string arrowLeft = "\u2190";  //':_'
    string arrowRight = "\u2192"; //':$'
    string amberBox = "\u25A1";   // ':#'
    // Original from MCDU Documentation
    //  char amberBox = '\u00B0';//OK to write on textBlock
    //  char amberBox = '\u00BA';// OK to write'º' on textBlock
    //  char amberBox = '\u0176';// NO OK to write 'º' on textBlock

    string degreeSign = "\u00B0"; //'º'
    /*
    "\unnnn" (where n = 0-9A-F) is a convention to add unicode code points to a string.
    The format those code points take depends on their encoding.
    It could be: UTF-8? UTF-16? UTF-32?
    */

    found = str.find("^");
    if (found != std::string::npos)
    {
        // string (1) string &replace(size_t pos, size_t len, const string &str);
        // pos: Position of the first character to be replaced.
        // len: Number of characters to replace
        if (found != std::string::npos)
        {
            str = str.replace(found, 1, arrowUp);
        }
    }

    found = str.find("~");
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, arrowDown);
    }

    found = str.find("_");
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, arrowLeft);
    }

    found = str.find("$");
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, arrowRight);
    }

    found = str.find("#");
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, amberBox);
    }
    return str;
} // string getSpecialChars(string str)

//----------------------------------------

bool getTextColor(string str) // Get Color to draw line
// Gets the string "COLOR" of MCDU line and store into line global: "lineColor";
// returns "true" if "COLOR" is found.
//  else returns "false"
// Text color wiil be stored into global: string "lineColor";
{
    // Text Command Strings
    string strWhite("WHITE");
    string strGreen("GREEN");
    string strCyan("CYAN");
    string strYellow("YELLOW");
    string strAmber("AMBER");
    string strMagenta("MAGENTA");
    string strRed("RED");
    string strGray("GREY");

    // Set color to default value:
    lineColor = "WHITE";

    // std::size_t found;
    std::string::size_type pos;

    // Find color White
    pos = str.find(strWhite);

    if (pos != std::string::npos)
    {
        lineColor = "WHITE";
        // cout << strWhite << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Green
    pos = str.find(strGreen);

    if (pos != std::string::npos)
    {
        lineColor = "GREEN";
        // cout << strGreen << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Cyan
    pos = str.find(strCyan);

    if (pos != std::string::npos)
    {
        lineColor = "CYAN";
        // cout << strCyan << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Yellow
    pos = str.find(strYellow);

    if (pos != std::string::npos)
    {
        lineColor = "YELLOW";
        // cout << strYellow << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Amber
    pos = str.find(strAmber);

    if (pos != std::string::npos)
    {
        lineColor = "AMBER";
        // cout << strAmber << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Magenta
    pos = str.find(strMagenta);

    if (pos != std::string::npos)
    {
        lineColor = "MAGENTA";
        // cout << strMagenta << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Red
    pos = str.find(strRed);

    if (pos != std::string::npos)
    {
        lineColor = "RED";
        // cout << strRed << endl;
        // cout << str << endl;
        return true;
    }

    // Find color Gray
    pos = str.find(strGray);

    if (pos != std::string::npos)
    {
        lineColor = "GRAY";
        // cout << strGray << endl;
        // cout << str << endl;
        return true;
    }

    lineColor = "WHITE"; // Reset to default color
    return false;
}

bool getTextFont(string str)
// Sets the global variable font to draw the MCDU line
// First resets the global variable lineFont to BIGFONT
// if "SMALLFONT", is found on MCDU line >> set global variable lineFont = SMALLFONT >> '1'
// if "SMALLFONT", is not found on MCDU line >> set global variable lineFont = BIGFONT >> '0'

{
    std::size_t found;

    lineFont = BIGFONT; // Set default "font" value

    // if font has change in the actual MCDU page line
    found = str.find("SMALLFONT");
    if (found != std::string::npos)
    {
        lineFont = SMALLFONT; // Set default font value
        return SMALLFONT;
    }
    else
        return BIGFONT;
}

bool getTextColPosition(string str) // Get string line position to draw line
// First looks for '#'. Start column number
// If found looks for ' ' espace. End column number
// If found gets the substring between #xx=
// Trim this string and is converted to int using stoi(string)
{
    std::size_t found;
    std::size_t startPostion;
    std::size_t stopPostion;
    std::size_t strLen;

    textPosition = 0; // Reset to Default value

    // look for '#' start of coulmn position from first string char
    found = str.find('#', 0);
    if (found != std::string::npos)
        startPostion = found;
    else
        return false;

    // look for ' ' end of coulmn position from '#' char
    found = str.find(' ', startPostion);
    if (found != std::string::npos)
        stopPostion = found;
    else
        stopPostion = str.length();

    strLen = (stopPostion - (startPostion + 1));

    string strTextPosition = str.substr(startPostion + 1, strLen);

    stringTrim(strTextPosition);

    // cout << "\nText position string:" << strTextPosition;

    textPosition = stoi(strTextPosition);

    return true;
}

// void specialChars(string str)
// {

//     // to be developped
// }

// ------------------------- Functions to get and store line globals  END ---------------------------------

//-------------------------- ncurse.lib Functions ---------------------------------------------------------

//-----------------------------  write Title  --------------------------------------------------------------
void writeTitle(string str)
{
    int x_col = COLS;
    int y_row = 0; // 

    //mvprintw( y_row, 1, "%s", str0Title.c_str());
    mvprintw( y_row, (x_col - str.length()) / 2, "%s", str.c_str());
    //refresh();
}

void writeTLL(string str)
{
    int x_col = COLS;
    int y_row = 0; // 

    //mvprintw( y_row, 1, "%s", str0Title.c_str());
    mvprintw( y_row, (x_col - str.length()) / 2, "%s", str.c_str());
    //refresh();
}

void writeTLC(string str)
{
    int x_col = (int) COLS/2; //Start on the middle
    int y_row = 0; // 

    //mvprintw( y_row, 1, "%s", str0Title.c_str());
    mvprintw( y_row, (x_col - str.length() / 2), "%s", str.c_str());
    //refresh();
}
void writeTLR(string str)
{
    int x_col = COLS;
    int y_row = 0; // 
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

//-------------------------------------------------------- write LEFT COLUMM ---------------------------------
void write1LB(string str)
{
    int x_col = 0;
    int y_row = 1; // 
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write1LD(string str)
{
    int x_col = 0;
    int y_row = 2; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write2LB(string str)
{
    int x_col = 0;
    int y_row = 3; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write2LD(string str)
{
    int x_col = 0;
    int y_row = 4; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write3LB(string str)
{
    int x_col = 0;
    int y_row = 5; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write3LD(string str)
{
    int x_col = 0;
    int y_row = 6; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write4LB(string str)
{
    int x_col = 0;
    int y_row = 7; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write4LD(string str)
{
    int x_col = 0;
    int y_row = 8; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write5LB(string str)
{
    int x_col = 0;
    int y_row = 9; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write5LD(string str)
{
    int x_col = 0;
    int y_row = 10; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write6LB(string str)
{
    int x_col = 0;
    int y_row = 11; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

void write6LD(string str)
{
    int x_col = 0;
    int y_row = 12; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}

//----------------------------------------------------- write SCRAPPAD LINE  -------------------------
void writePADL(string str)
{
    int x_col = 0;
    int y_row = 13; //
    mvprintw( y_row, x_col, "%s", str.c_str());
    //refresh();
}
void writePADC(string str)
{
    int x_col = (int)(COLS/2);
    int y_row = 13; // 

    mvprintw( y_row, (x_col - str.length() / 2), "%s", str.c_str());
    //refresh();
}

void writePADR(string str)
{
    int x_col = COLS;
    int y_row = 14; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

/* void writePAD(string str)
{
    // int x = 1;  // All coordinates will increment by 1 because borders
    // int y = 14; // All coordinates will increment by 1 because borders
    // x = 1 + ((24 - str.length()) / 2);
    // mvprintw( y, x, "%s", str.c_str());
    // //refresh();
} */

//-------------------------------------------- write CENTER COLUMM ---------------------------------------
void write1CB(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 1; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write1CD(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 2; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write2CB(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 3; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}
void write2CD(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 4; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write3CB(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 5; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}
void write3CD(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 6; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write4CB(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 7; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write4CD(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 8; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write5CB(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 9; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write5CD(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 10; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write6CB(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 11; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

void write6CD(string str)
{
    int x_col = (int)(COLS / 2);
    int y_row = 12; // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    //refresh();
}

// write RIGHT COLUMM

void write1RB(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 1;  // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write1RD(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 2;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write2RB(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 3;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write2RD(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 4;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write3RB(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    //int x_col = max_x; // All coordinates will increment by 1 because borders
    int y_row = 5;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write3RD(string str)
{
    //int x_col = COLS; // All coordinates will increment by 1 because borders
    int x_col = max_x; // All coordinates will increment by 1 because borders
    int y_row = 6;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write4RB(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 7;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write4RD(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 8;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write5RB(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 9;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write5RD(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 10;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write6RB(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 11;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void write6RD(string str)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 12;    // All coordinates will increment by 1 because borders
    mvprintw( y_row, (x_col - str.length()), "%s", str.c_str());
    //refresh();
}

void initMCDU()
{
    system("clear");
    //setlocale should be called before init ncurses "initscr()", so as to draw UTF-8 correctly
    setlocale(LC_ALL, ""); // Use la función de setlocale(), para establecer la configuración regional
    initscr();            /* Start curses mode 		*/
    
    cbreak();             /* Line buffering disabled, Pass on
                           * everty thing to me 		*/
    keypad(stdscr, TRUE); /* I need that nifty F1 	*/
    clear();
    noecho();
    curs_set(INVISIBLE);

    if (has_colors() == FALSE)
    {
        endwin();
        printf("\nYour terminal does not support color\n");
        exit(1);
    }
    initColors();
    initMCDUscreen();
    refresh();
}

void initMCDUscreen()
{
    getmaxyx(stdscr, max_y, max_x); /* get the number of rows and columns os standard  */

    starty = 0;
    startx = 0;
    //mcdu = create_newwin(max_y, max_x, starty, startx); // Creates a mcdu window to cover all screen
    //mcdu = create_newwin(LINES, COLS, starty, startx); // Creates a mcdu window to cover all screen
    box(stdscr, 0, 0);
    refresh();
    start_color();
    refresh();
    waitKeyPressed();
}

void initColors()
{
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    start_color(); /* Start color*/

    init_color(MCDUColors::AMBER, 1000, 749, 0); // Color not definer in ncurses.lib
    init_color(MCDUColors::GRAY, 500, 500, 500); // Color not definer in ncurses.lib
    /*
     * ncurses library
     * init_color(param_1, param_2, param_3, param_4)
     * param_1: color name
     * param_2, _3, _4 : RGB content min = 0, max = 1000
     * */

    init_pair(MCDUColors::WHITE, COLOR_WHITE, COLOR_BLACK);     // Set a pair
    init_pair(MCDUColors::BLACK, COLOR_BLACK, COLOR_WHITE);     // Set a pair
    init_pair(MCDUColors::BLUE, COLOR_BLUE, COLOR_BLACK);       // Set a pair
    //init_pair(MCDUColors::CYAN, COLOR_CYAN, COLOR_BLACK);       // Set a pair
    init_pair(MCDUColors::CYAN, COLOR_BLUE, COLOR_BLACK);       // Set a pair
    init_pair(MCDUColors::GREEN, COLOR_GREEN, COLOR_BLACK);     // Set a pair
    init_pair(MCDUColors::YELLOW, COLOR_YELLOW, COLOR_BLACK);   // Set a pair
    init_pair(MCDUColors::AMBER, COLOR_AMBER, COLOR_BLACK);     // Set a pair
    init_pair(MCDUColors::RED, COLOR_RED, COLOR_BLACK);         // Set a pair
    init_pair(MCDUColors::MAGENTA, COLOR_MAGENTA, COLOR_BLACK); // Set a pair
    init_pair(MCDUColors::GRAY, COLOR_GRAY, COLOR_BLACK);       // Set a pair
}

void mcduTest()
{

    writeTitle(strTitle);
    write1LB(str1LB);
    write1LD(str1LD);
    write2LB(str2LB);
    write2LD(str2LD);
    write3LB(str3LB);
    write3LD(str3LD);
    write4LB(str4LB);
    write4LD(str4LD);
    write5LB(str5LB);
    write5LD(str5LD);
    write6LB(str6LB);
    write6LD(str6LD);
    writePADC("SCRACHPAD");

    // writePADL(strPADL);
    // writePADC(strPADC);
    // writePADR(strPADR);
    // writePAD(strPAD);

    write1CB(str1CB);
    write1CD(str1CD);
    write2CB(str2CB);
    write2CD(str2CD);
    write3CB(str3CB);
    write3CD(str3CD);
    write4CB(str4CB);
    write4CD(str4CD);
    write5CB(str5CB);
    write5CD(str5CD);
    write6CB(str6CB);
    write6CD(str6CD);

    write1RB(str1RB);
    write1RD(str1RD);
    write2RB(str2RB);
    write2RD(str2RD);
    write3RB(str3RB);
    write3RD(str3RD);
    write4RB(str4RB);
    write4RD(str4RD);
    write5RB(str5RB);
    write5RD(str5RD);
    write6RB(str6RB);
    write6RD(str6RD);
  }

//--------------------------------  Create windows   ------------------------------------------------

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx); // Adding 1 for borders. To take into account when  writing
    //box(local_win, 0, 0);                                  //0, 0 gives default characters
                                                            // for the vertical and horizontal
                                                            //* lines			
    wrefresh(local_win);                                   // Show that box 		

    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    /* box(local_win, ’ ’, ’ ’); : This won’t produce the desired
     * result of erasing the window. It will leave its four corners
     * and so an ugly remnant of window.
     */
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}

void clearMCDU()
{

    clear(); // ncurses clear mcdu window /* clear the screen */
    refresh();
}

void testScreenTools(void)

// This function checks the globals lines functions
{
    system("clear");

    for (size_t i = 0; i < pageLines.size(); i++)
    {
        cout << endl
             << endl
             << "Line_" << i << ":" << pageLines[i];

        if (getTextCmd(pageLines[i]) == true)
        {

            cout << "\nText Command:" << cmdTxtLine;
        }
        else
        {
            cout << "\nCommand Line Text Not Found !!";
            // return false;
        }

        if (getTextString(pageLines[i]) == true)
        {

            cout << "\nText String:" << lineText;
        }
        else
        {
            cout << "\nText string Not Found !!";
            // return false;
        }

        if (getTextColor(pageLines[i]))
        {
            cout << "\nText Color:" << lineColor;
        }

        if (getTextFont(pageLines[i]))
        {
            cout << "\nText Font:" << "SMALLFONT";
        }
        else
        {
            cout << "\nText Font:" << "BIGFONT";
        }

        if (getTextColPosition(pageLines[i]))
        {

            cout << "\nText position int:" << textPosition;
        }
    } // for
} // void testScreenTools(void)

void getLineGlobals(string str)
{

    if (getTextCmd(str)) // Get CMD to draw line
    {
        getTextString(str);      // Get string to draw line
        getTextColor(str);       // Get Color to draw line
        getTextFont(str);        // Sets the global variable Font to draw line
        getTextColPosition(str); // Get string line position to draw line
        getSpecialChars(str);
    }
    else
        lineText = "NO TEXT";
}

void drawPage() // Takes the page stored into vector<string> pageLines;
{
    clear(); // ncurses clear mcdu window // ncurses clear
    for (int i = 0; i < pageLines.size(); i++)
    {
        getLineGlobals(pageLines[i]);
        drawLine();
    }
    refresh();
}

void drawLine()
{
    int x;
    int y;
    //string strLine = lineText.c_str();
   
    // Set coordinates using cmdTxtLine
    if (lineText == "NO TEXT")
        return;


    // Title coordinates
    if (cmdTxtLine == "TLL")
    {
        x = 0;
        y = 0; // 
    }

    if (cmdTxtLine == "TLC")
    {
        x = (int)(COLS/2);
        y = 0; // 
        x = x - (lineText.length() / 2);
    }

    if (cmdTxtLine == "TLR")
    {
        x = COLS;
        y = 0; // 
        x = x - lineText.length();
    }

    // Left side  coordinates
    if (cmdTxtLine == "_1LB")
    {
        x = 0;
        y = 1; // 
    }

    if (cmdTxtLine == "_1LD")
    {
        x = 0;
        y = 2; // 
    }

    if (cmdTxtLine == "_2LB")
    {
        x = 0;
        y = 3; // 
    }

    if (cmdTxtLine == "_2LD")
    {
        x = 0;
        y = 4; //
    }

    if (cmdTxtLine == "_3LB")
    {
        x = 0;
        y = 5; // 
    }

    if (cmdTxtLine == "_3LD")
    {
        x = 0;
        y = 6; // 
    }

    if (cmdTxtLine == "_4LB")
    {
        x = 0;
        y = 7; // 
    }

    if (cmdTxtLine == "_4LD")
    {
        x = 0;
        y = 8; // 
    }

    if (cmdTxtLine == "_5LB")
    {
        x = 0;
        y = 9; // 
    }

    if (cmdTxtLine == "_5LD")
    {
        x = 0;
        y = 10; // 
    }

    if (cmdTxtLine == "_6LB")
    {
        x = 0;
        y = 11; // 
    }

    if (cmdTxtLine == "_6LD")
    {
        x = 0;
        y = 12; // 
    }

    // Right side  coordinates
    if (cmdTxtLine == "_1RB")
    {
        x = COLS; // All coordinates will increment by 1 because borders
        y = 1;  // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_1RD")
    {
        x = COLS;
        y = 2; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_2RB")
    {
        x = COLS;
        y = 3; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_2RD")
    {
        x = COLS;
        y = 4; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_3RB")
    {
        x = COLS;
        y = 5; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_3RD")
    {
        x = COLS;
        y = 6; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_4RB")
    {
        x = COLS;
        y = 7; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_4RD")
    {
        x = COLS;
        y = 8; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_5RB")
    {
        x = COLS;
        y = 9; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_5RD")
    {
        x = COLS;
        y = 10; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_6RB")
    {
        x = COLS;
        y = 11; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    if (cmdTxtLine == "_6RD")
    {
        x = COLS;
        y = 12; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    // Center side  coordinates
    if (cmdTxtLine == "_1CB")
    {
        x = (int)(COLS / 2);
        y = 1; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_1CD")
    {
        x = (int)(COLS / 2);
        y = 2; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_2CB")
    {
        x = (int)(COLS / 2);
        y = 3; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_2CD")
    {
        x = (int)(COLS / 2);
        y = 4; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_3CB")
    {
        x = (int)(COLS / 2);
        y = 5; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_3CD")
    {
        x = (int)(COLS / 2);
        y = 6; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_4CB")
    {
        x = (int)(COLS / 2);
        y = 7; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_4CD")
    {
        x = (int)(COLS / 2);
        y = 8; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_5CB")
    {
        x = (int)(COLS / 2);
        y = 9; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_5CD")
    {
        x = (int)(COLS / 2);
        y = 10; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_6CB")
    {
        x = (int)(COLS / 2);
        y = 11; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    if (cmdTxtLine == "_6CD")
    {
        x = (int)(COLS / 2);
        y = 12; // All coordinates will increment by 1 because borders
        x = (x - (lineText.length() / 2));
    }

    // Scratchpad coordinates
    if (cmdTxtLine == "PADL")
    {
        x = 0;
        y = LINES; // All coordinates will increment by 1 because borders
    }

    if (cmdTxtLine == "PADC")
    {
        x = (int)(COLS / 2);
        y = LINES; // All coordinates will increment by 1 because borders
        x = x - (int)(lineText.length() / 2);
    }

    if (cmdTxtLine == "PADR")
    {
        x = COLS;
        y = LINES; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    // Gets COLOR_PAIR
    //wattron(win, COLOR_PAIR(1));

    
    
    if (lineColor == "WHITE")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::WHITE));
        
        mvprintw( y, x, "%s", lineText.c_str());
        // mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::WHITE));
    }
        
    else if (lineColor == "BLACK")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::BLACK));
        mvprintw( y, x, "%s", lineText.c_str());
        // mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::BLACK));
    }

    else if (lineColor == "BLUE")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::BLUE));
        mvprintw( y, x, "%s", lineText.c_str());
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::BLUE));
    }

    else if (lineColor == "CYAN")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::CYAN));
        mvprintw(y, x, "%s", lineText.c_str());
        //mvprintw( y, x, "%s", "\u2192");
        //move(y, x);
        //printw("\u2192");

        //mvwaddstr(stdscr, y, x, lineText.c_str());


        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::CYAN));
    }

    else if (lineColor == "GREEN")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::GREEN));
        mvprintw( y, x, "%s", lineText.c_str());
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::GREEN));
    }

    else if (lineColor == "YELLOW")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::YELLOW));
        mvprintw( y, x, "%s", lineText.c_str());
        // mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::YELLOW));
    }

    else if (lineColor == "AMBER")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::AMBER));
        mvprintw( y, x, "%s", lineText.c_str());
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::AMBER));
    }

    else if (lineColor == "RED")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::RED));
        mvprintw( y, x, "%s", lineText.c_str());
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::RED));
    }

    else if (lineColor == "MAGENTA")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::MAGENTA));
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        mvprintw( y, x, "%s", lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::MAGENTA));
    }

    else if (lineColor == "GRAY")
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::GRAY));
        mvprintw( y, x, "%s", lineText.c_str());
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::GRAY));
    }

    else
    {
        wattron(stdscr, COLOR_PAIR(MCDUColors::WHITE));
        mvprintw( y, x, "%s", lineText.c_str());
        //mvwaddstr(stdscr, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(stdscr, COLOR_PAIR(MCDUColors::WHITE));
    }
 
    // Check for special chars
   lineColor = "WHITE"; //Reset lineColor
}
