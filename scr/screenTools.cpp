// screenTools.cpp

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <stdlib.h>
#include <ncurses.h>              /* ncurses.h includes stdio.h */
#include "../include/fileTools.h" // Adjust the path to the correct location of fileTools.h
#include <curses.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
// #include <format> // Ensure this header is included for std::format
// using std::format; // Use the format function from the std namespace

using namespace std;

void checkForVars(string strLine);

#define SMALLFONT 1
#define BIGFONT 0

///////////////////////////  START Old screenTools.cpp /////////////////

#define INVISIBLE 0 // Cursor hide
#define VISIBLE 1   // Cursor normal
#define HIGHLIGHT 2 // Cursor bright

#define COLOR_AMBER 6 // To match enum MCDUColors
#define COLOR_GRAY 9  // To match enum MCDUColors

//--------------------Functions------------------------
void clearLine(WINDOW *mcdu, int y);
void mcduTimer(int t_sec);
bool getCol(string strLine);

//---------------------Variables-----------------------
extern string actualPage;
extern string strTestPage;
extern bool testPage;
extern bool mcduFlightMode;
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

// Functions
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void clearMCDU();
void initMCDU();
void initMCDUscreen();
void initColors();
void mcduTest();
void drawPage();                  // Takes the page stored into vector<string> pageLines;
void drawLine();                  // Draws line cheking the global variables stored
string checkVars(string strLine); // Change lineText when contains special command @VAR(X)

void testScreenTools(void); // Test MCDU with ncurses functions

//------------------- Write MCDU screen START -----------------------------
void writeTitle(string str, WINDOW *mcdu);
void writeTLL(string str, WINDOW *mcdu);
void writeTLC(string str, WINDOW *mcdu);
void writeTLR(string str, WINDOW *mcdu);

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

void writePADL(string str, WINDOW *mcdu);
void writePADC(string str, WINDOW *mcdu);
void writePADR(string str, WINDOW *mcdu);
void writePAD(string str, WINDOW *mcdu);

void (*writeMCDU[])(string str, WINDOW *mcdu) = {
    writeTLL, writeTLC, writeTLR,
    write1LB, write1LD, write2LB, write2LD, write3LB, write3LD, write4LB, write4LD, write5LB, write5LD, write6LB, write6LD,
    write1RB, write1RD, write2RB, write2RD, write3RB, write3RD, write4RB, write4RD, write5RB, write5RD, write6RB, write6RD,
    write1CB, write1CD, write2CB, write2CD, write3CB, write3CD, write4CB, write4CD, write5CB, write5CD, write6CB, write6CD,
    writePADL, writePADC, writePADR};

//---------------------------------------Write MCDU screen  END --------------------------

string strTrimLH(string str); // Remove spaces at beginning of string
string strTrimRH(string str); // Remove spaces at the end of string
void stringTrim(string &str); // Remove spaces at the beginning and end of string

string text_between_two(string str, char a, char b); // Get a substring between two chars
string getSubStringCharToEnd(string str, char c);

bool getTextCmd(string str);    // Get CMD to draw line
bool getTextString(string str); // Get string to draw line

string replaceUTF8Chars(string text, const string oldChar, const string newChar);
bool getTextColor(string str); // Get Color to draw line
bool getTextFont(string str);  // Sets the global variable Font to draw line
// bool getTextColPosition(string str); // Get string line position to draw line
// int getTextColPosition(string pageLine, string lineText);
void getSpecialChars(string str); // Check if a string has special chars to replace them for UTF-8 char

void getLineGlobals(string str);

//-------------------------------Variables-----------------------
// Line Globals
string cmdTxtLine;
string lineText;
string lineColor;
unsigned int lineFont = BIGFONT; // Set font to default value
int textColPosition = 0;         // Default value
bool specialChar = false;        // when a text has a special char its lenght should increased in 2 chars

// ncurses Globals
int mcdu_rows = 14;
int mcdu_cols = 24;
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
    string strCB("CB");
    string strCD("CD");

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

    //////////////////CB-CD/////////////////////
    found = str.find(strCB);
    if (found != std::string::npos)
    {
        str.insert(0, 1, '_');
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    found = str.find(strCD);
    if (found != std::string::npos)
    {
        str.insert(0, 1, '_');
        cmdTxtLine = str.c_str(); // Store Global
        return true;
    }

    ////////////////////////////////////////////

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

        getSpecialChars(strText); // Replace special chars and stored int lineText
        // strText = getSpecialChars(strText);
        // lineText = strText; // Store Global
        return true;
    }

    // Check for '=' and end of string
    strText = str;
    strText = getSubStringCharToEnd(strText, '=');

    if (strText != "FAIL")
    {
        strText = strTrimLH(strText); // Remove spaces on left side
        strText = strTrimRH(strText); // Remove spaces on right side
        getSpecialChars(strText);     // Replace special chars and stored int lineText
        // strText = getSpecialChars(strText);
        // lineText = strText; // Store Global
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
    // size_t strLen;
    size_t cPos;
    // strLen = str.length();
    //  string substr (size_t pos = 0, size_t len = npos) const;

    cPos = str.find(c, 0); // Search from beginning

    if (cPos == std::string::npos)
    {
        return "FAIL";
    }
    cPos = cPos + 1;                    // To skip '='
    string strTitle = str.substr(cPos); // Get from '=' to the end
    return strTitle;
}

string replaceUTF8Chars(string text, const string oldChar, const string newChar)
{
    size_t pos = 0;
    while ((pos = text.find(oldChar, pos)) != string::npos)
    {
        text.replace(pos, oldChar.length(), newChar);
        pos += newChar.length(); // Move past the replaced character
    }
    return text;
}

// string getSpecialChars(string str)
void getSpecialChars(string str)
{
    // Replace special characters as defined on MCDU manual

    //---------------------Variables used
    std::size_t found;
    string strCheck = str;

    const string sChar = "⬔";
    const string arrowUp = "↑";
    const string arrowDown = "↓";
    const string arrowLeft = "←";
    const string arrowRight = "→";
    // string amberBox = "\u25A1"; // '□'
    const string amberBox = "□"; // '□'

    const string strArrowUpCheck = "^";
    const string strArrowDownCheck = "~";
    const string strArrowLeftCheck = "_";
    const string strArrowRightCheck = "$";
    const string strAmberBoxCheck = "#";

    specialChar = false;

    // Check for ArrowUP
    found = strCheck.find(strArrowUpCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowUp);
        lineText = str; // Store Global
        specialChar = true;
        // return str;
    }
    else
    {
        found = std::string::npos; // Reset found
        lineText = str;            // Store Global
    }

    // Check for ArrowDown
    found = strCheck.find(strArrowDownCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowDown);
        lineText = str; // Store Global
        specialChar = true;
        // return str;
    }
    else
    {
        found = std::string::npos; // Reset found
        lineText = str;            // Store Global
    }

    // Check for ArrowLeft
    found = strCheck.find(strArrowLeftCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)
        str = strCheck.replace(found, 1, arrowLeft);
        lineText = str; // Store Global
        specialChar = true;
        // return str;
    }
    else
    {
        found = std::string::npos; // Reset found
        lineText = str;            // Store Global
    }

    // Check for ArrowRight
    found = strCheck.find(strArrowRightCheck);
    if (found != std::string::npos)
    {
        // string& replace (size_t pos,  size_t len,  const string& str)

        str = strCheck.replace(found, 1, arrowRight);
        lineText = str; // Store Global
        specialChar = true;
        // return str;
    }
    else
    {
        found = std::string::npos; // Reset found
        lineText = str;            // Store Global
    }

    // Check for amberBox
    // found = strCheck.find(strAmberBoxCheck);
    // if (found != string::npos)
    // {
    //     // string& replace (size_t pos,  size_t len,  const string& str)
    //     str = strCheck.replace(found, 1, amberBox);
    //     lineText = str; // Store Global
    //     specialChar = true;
    //     //return str;
    // }
    // else
    //     found = std::string::npos; // Reset found

    // Check for amberBox
    found = str.find(strAmberBoxCheck);
    if (found != std::string::npos)
    {
        lineText = replaceUTF8Chars(str, strAmberBoxCheck, amberBox); // Store Global
        specialChar = true;
        // lineText = str; // Store Global
        // return str;
    }
    // return str;
} // string getSpecialChars(string str)

//-------------------------OLD------------------------
string getSpecialCharsOld(string str)
{
    // Replace special characters as defined on MCDU manual
    std::size_t found;
    string arrowUp = "\u2191";    //'^'
    string arrowDown = "\u2193";  //'~'
    string arrowLeft = "\u2190";  //'_'
    string arrowRight = "\u2192"; //'$'
    string amberBox = "\u25A1";   // '#'
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

    found = str.find('^');
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

    found = str.find('~');
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, arrowDown);
    }

    found = str.find('_');
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, arrowLeft);
    }

    found = str.find('$');
    if (found != std::string::npos)
    {
        str = str.replace(found, 1, arrowRight);
    }

    found = str.find('#');
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

int getTextColPosition(string pageLine, string lineText) // Get string line position to draw line
//  str_original is from pageLines[i] <string> vector, like: = "1LD = Hello World, GREEN, #10";
//  str_find is from lineText[]
//  First looks for lineText[i] into pageLines[i]
//  We want disregard "lineText" because it may contain '#' as special char.
// '#'. Start column number
//  If found looks for ' ' espace. End column number
//  If found gets the substring between #xx=
//  Trim this string and is converted to int using stoi(string)
//  Returns the column position to draw the line, so as the lineText will start on this column
//  Text Cmd will not be taken into account
{
    size_t found;
    size_t startPostion;
    size_t textColPosition;
    string strTextPosition;

    textColPosition = 0; // Reset to Default value

    if (lineText == "NO TEXT")
    {
        return 0;
    }

    found = pageLine.find(lineText, 0); // Finding line text on whole str_original
    if (found != string::npos)
        startPostion = found + lineText.length();
    else
        return 0;

    // look for ',' >> the end of string lineText
    found = pageLine.find(',', startPostion);
    if (found != string::npos)
        startPostion = found;
    else
        return 0;

    strTextPosition = pageLine.substr(startPostion + 1); // Get substring after the comma

    startPostion = strTextPosition.find('#');                   // Get substring after the '#'
    strTextPosition = strTextPosition.substr(startPostion + 1); // Get substring after the '#'
    trim(strTextPosition);                                      // Trim blank chars
    // Convert substring to int
    textColPosition = stoi(strTextPosition);
    return textColPosition; // Return column position
}

// ------------------------- Functions to get and store line globals  END ---------------------------------

//-------------------------- ncurse.lib Functions ---------------------------------------------------------

//-----------------------------  write Title  --------------------------------------------------------------
void writeTitle(string str, WINDOW *mcdu)
{
    int x_col = COLS;
    int y_row = 0; //

    // mvwprintw(mcdu, y_row, 1, "%s", str0Title.c_str());
    mvwprintw(mcdu, y_row, (x_col - str.length()) / 2, "%s", str.c_str());
    // wrefresh(mcdu);
}

void writeTLL(string str, WINDOW *mcdu)
{
    int x_col = COLS;
    int y_row = 0; //

    // mvwprintw(mcdu, y_row, 1, "%s", str0Title.c_str());
    mvwprintw(mcdu, y_row, (x_col - str.length()) / 2, "%s", str.c_str());
    // wrefresh(mcdu);
}

void writeTLC(string str, WINDOW *mcdu)
{
    int x_col = (int)COLS / 2; // Start on the middle
    int y_row = 0;             //

    // mvwprintw(mcdu, y_row, 1, "%s", str0Title.c_str());
    mvwprintw(mcdu, y_row, (x_col - str.length() / 2), "%s", str.c_str());
    // wrefresh(mcdu);
}
void writeTLR(string str, WINDOW *mcdu)
{
    int x_col = COLS;
    int y_row = 0; //
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

//-------------------------------------------------------- write LEFT COLUMM ---------------------------------
void write1LB(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 1; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write1LD(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 2; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write2LB(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 3; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write2LD(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 4; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write3LB(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 5; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write3LD(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 6; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write4LB(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 7; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write4LD(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 8; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write5LB(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 9; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write5LD(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 10; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write6LB(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 11; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

void write6LD(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 12; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}

//----------------------------------------------------- write SCRAPPAD LINE  -------------------------
void writePADL(string str, WINDOW *mcdu)
{
    int x_col = 0;
    int y_row = 13; //
    mvwprintw(mcdu, y_row, x_col, "%s", str.c_str());
    // wrefresh(mcdu);
}
void writePADC(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 13; //

    mvwprintw(mcdu, y_row, (x_col - str.length() / 2), "%s", str.c_str());
    // wrefresh(mcdu);
}

void writePADR(string str, WINDOW *mcdu)
{
    int x_col = COLS;
    int y_row = 14; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

/* void writePAD(string str, WINDOW *mcdu)
{
    // int x = 1;  // All coordinates will increment by 1 because borders
    // int y = 14; // All coordinates will increment by 1 because borders
    // x = 1 + ((24 - str.length()) / 2);
    // mvwprintw(mcdu, y, x, "%s", str.c_str());
    // //wrefresh(mcdu);
} */

//-------------------------------------------- write CENTER COLUMM ---------------------------------------
void write1CB(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 1; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write1CD(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 2; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write2CB(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 3; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}
void write2CD(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 4; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write3CB(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 5; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}
void write3CD(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 6; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write4CB(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 7; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write4CD(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 8; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write5CB(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 9; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write5CD(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 10; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write6CB(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 11; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write6CD(string str, WINDOW *mcdu)
{
    int x_col = (int)(COLS / 2);
    int y_row = 12; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - (str.length() / 2)), "%s", str.c_str());
    // wrefresh(mcdu);
}

// write RIGHT COLUMM

void write1RB(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 1;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write1RD(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 2;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write2RB(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 3;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write2RD(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 4;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write3RB(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    // int x_col = max_x; // All coordinates will increment by 1 because borders
    int y_row = 5; // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write3RD(string str, WINDOW *mcdu)
{
    // int x_col = COLS; // All coordinates will increment by 1 because borders
    int x_col = max_x; // All coordinates will increment by 1 because borders
    int y_row = 6;     // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write4RB(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 7;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write4RD(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 8;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write5RB(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 9;    // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write5RD(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 10;   // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write6RB(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 11;   // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void write6RD(string str, WINDOW *mcdu)
{
    int x_col = COLS; // All coordinates will increment by 1 because borders
    int y_row = 12;   // All coordinates will increment by 1 because borders
    mvwprintw(mcdu, y_row, (x_col - str.length()), "%s", str.c_str());
    // wrefresh(mcdu);
}

void initMCDU()
{
    system("clear");
    // setlocale(LC_ALL, ""), should be used before initscr(), so as to show up UTF-8 correctly
    setlocale(LC_ALL, ""); // Use la función de setlocale(), para establecer la configuración regional
    initscr();             /* Start curses mode 		*/

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

    starty = ((int)(LINES / 2) - 7); // LINES and COLS, have been defined on terminal as 14x24
    startx = ((int)(COLS / 2) - 12);
    mcdu = create_newwin(max_y, max_x, starty, startx); // Creates a mcdu window to cover all screen
    start_color();
    // wrefresh(mcdu);
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

    init_pair(MCDUColors::WHITE, COLOR_WHITE, COLOR_BLACK); // Set a pair
    init_pair(MCDUColors::BLACK, COLOR_BLACK, COLOR_WHITE); // Set a pair
    init_pair(MCDUColors::BLUE, COLOR_BLUE, COLOR_BLACK);   // Set a pair
    // init_pair(MCDUColors::CYAN, COLOR_CYAN, COLOR_BLACK);       // Set a pair
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

    writeTitle(strTitle, mcdu);
    write1LB(str1LB, mcdu);
    write1LD(str1LD, mcdu);
    write2LB(str2LB, mcdu);
    write2LD(str2LD, mcdu);
    write3LB(str3LB, mcdu);
    write3LD(str3LD, mcdu);
    write4LB(str4LB, mcdu);
    write4LD(str4LD, mcdu);
    write5LB(str5LB, mcdu);
    write5LD(str5LD, mcdu);
    write6LB(str6LB, mcdu);
    write6LD(str6LD, mcdu);
    writePADC("SCRACHPAD", mcdu);

    // writePADL(strPADL, mcdu);
    // writePADC(strPADC, mcdu);
    // writePADR(strPADR, mcdu);
    // writePAD(strPAD, mcdu);

    write1CB(str1CB, mcdu);
    write1CD(str1CD, mcdu);
    write2CB(str2CB, mcdu);
    write2CD(str2CD, mcdu);
    write3CB(str3CB, mcdu);
    write3CD(str3CD, mcdu);
    write4CB(str4CB, mcdu);
    write4CD(str4CD, mcdu);
    write5CB(str5CB, mcdu);
    write5CD(str5CD, mcdu);
    write6CB(str6CB, mcdu);
    write6CD(str6CD, mcdu);

    write1RB(str1RB, mcdu);
    write1RD(str1RD, mcdu);
    write2RB(str2RB, mcdu);
    write2RD(str2RD, mcdu);
    write3RB(str3RB, mcdu);
    write3RD(str3RD, mcdu);
    write4RB(str4RB, mcdu);
    write4RD(str4RD, mcdu);
    write5RB(str5RB, mcdu);
    write5RD(str5RD, mcdu);
    write6RB(str6RB, mcdu);
    write6RD(str6RD, mcdu);
    wrefresh(mcdu);
}

//--------------------------------  Create windows   ------------------------------------------------

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx); // Adding 1 for borders. To take into account when  writing
    // box(local_win, 0, 0);                                  //0, 0 gives default characters
    //  for the vertical and horizontal
    //* lines
    wrefresh(local_win); // Show that box

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

    wclear(mcdu); // ncurses clear mcdu window /* clear the screen */
    wrefresh(mcdu);
}

void testScreenTools(void)

// DEBUGGING  Function
//  This function checks the globals lines functions
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

        // if (//getTextColPosition(pageLines[i], lineText)
        // {
        //     cout << "\nText position:" << lineText;
        // }
        // else
        // {
        //     cout << "\nText position Not Found !!";
        //     // return false;
        // }
        // {

        //     cout << "\nText position int:" << textColPosition;
        // }
    } // for
} // void testScreenTools(void)

void getLineGlobals(string str)
{

    if (getTextCmd(str)) // Get CMD to draw line
    {
        getTextString(str); // Get string to draw line and check for getSpecialChars(str);
        getTextColor(str);  // Get Color to draw line
        getTextFont(str);   // Sets the global variable Font to draw line
        getCol(str);        // Check for cmd #NN, and if any, stores its value into: "textColPosition"
                            // "textColPosition" is gone a be udes when callto draw line
        // checkForVars           //So as update string text with VARs
    }
    else
        lineText = "NO TEXT";
}

void drawPage() // Takes the page stored into vector<string> pageLines;
{
    // if(mcduFlightMode == true)
    // {
    //     checkForINFLIGHT();
    //     if(inFlight == true)
    //     {
    //         getPage(inFlightPage);
    //         //Reset Globals
    //         inFlight = false;
    //         inFlightPage = "";
    //     }
    // }// if(mcduFlightMode == true)

    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    for (long unsigned int i = 0; i < pageLines.size(); i++)
    {
        getLineGlobals(pageLines[i]);
        // checkForVars(pageLines[i]); // Check for special function VAR(1-4), VARPAGE, VARPAGE1-4
        drawLine();
    }
    // getLSK(); // Is included in getPage()
    //  writePADC("Screen drawn!!", mcdu);
    //  wrefresh(mcdu);
    //  getchar();
    // appendFile_MCDU_Data();

} // drawPage

void drawLine()
{
    int x;
    int y;
    // string strLine = lineText.c_str();

    // Set coordinates using cmdTxtLine
    if (lineText == "NO TEXT")
        return;

    // Check for VARS
    lineText = checkVars(lineText); // replace the @VAR(X) with the string stored in global variables VARX

    if (cmdTxtLine == "TLL")
    {
        x = 0;
        y = 0; //
    }

    if (cmdTxtLine == "TLC")
    {
        x = (int)(COLS / 2);
        y = 0; //
        x = x - (lineText.length() / 2);
    }

    if (cmdTxtLine == "TLR")
    {
        x = COLS;
        if (specialChar == true)
        {
            x = x + 3;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
        y = 0; //
        x = x - lineText.length();
    }

    // Left side  coordinates
    if (cmdTxtLine == "_1LB")
    {
        x = 0;
        y = 1;
    }

    if (cmdTxtLine == "_1LD")
    {
        x = 0;
        y = 2;
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
        y = 5;
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
        x = COLS;
        y = 1;
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels on the RH
        }
    }

    if (cmdTxtLine == "_1RD")
    {
        x = COLS;
        y = 2; // All coordinates will increment by 1 because borders
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
    }

    if (cmdTxtLine == "_2RB")
    {
        x = COLS;
        y = 3;
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels on the RH
        }
    }

    if (cmdTxtLine == "_2RD")
    {
        x = COLS;
        y = 4;
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
    }

    if (cmdTxtLine == "_3RB")
    {
        x = COLS;
        y = 5;
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels on the RH
        }
    }

    if (cmdTxtLine == "_3RD")
    {
        x = COLS;
        y = 6;
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
    }

    if (cmdTxtLine == "_4RB")
    {
        x = COLS;
        y = 7;
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
    }

    if (cmdTxtLine == "_4RD")
    {
        x = COLS;
        y = 8; // All coordinates will increment by 1 because borders
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
    }

    if (cmdTxtLine == "_5RB")
    {
        x = COLS;
        y = 9; // All coordinates will increment by 1 because borders
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels on the RH
        }
    }

    if (cmdTxtLine == "_5RD")
    {
        x = COLS;
        y = 10; // All coordinates will increment by 1 because borders
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
    }

    if (cmdTxtLine == "_6RB")
    {
        x = COLS;
        y = 11; // All coordinates will increment by 1 because borders
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels on the RH
        }
    }

    if (cmdTxtLine == "_6RD")
    {
        x = COLS;
        y = 12; // All coordinates will increment by 1 because borders
        x = x - lineText.length();

        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
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
        y = 13; // All coordinates will increment by 1 because borders
    }

    if (cmdTxtLine == "PADC")
    {
        x = (int)(COLS / 2);
        y = 13; // All coordinates will increment by 1 because borders
        x = x - (int)(lineText.length() / 2);
    }

    if (cmdTxtLine == "PADR")
    {
        x = COLS;
        if (specialChar == true)
        {
            x = x + 2;           // To compensate because specialChars take up more than 1 char
            specialChar = false; // This correction, only applies to labels and data on the RH side
        }
        y = 13; // All coordinates will increment by 1 because borders
        x = x - lineText.length();
    }

    // Beacause is not possible change font on text terminal
    // Will use BOLD for BIGFONT (Labels) and NO BOLT for SMALLFONT(Labels)

    if (lineFont == BIGFONT)
        wattron(mcdu, A_BOLD);
    else
        wattroff(mcdu, A_BOLD);

    // Gets COLOR_PAIR
    // wattron(win, COLOR_PAIR(1));

    if (lineColor == "WHITE")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::WHITE));
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::WHITE));
    }

    else if (lineColor == "BLACK")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::BLACK));
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::BLACK));
    }

    else if (lineColor == "BLUE")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::BLUE));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::BLUE));
    }

    else if (lineColor == "CYAN")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::CYAN));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::CYAN));
    }

    else if (lineColor == "GREEN")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::GREEN));
        mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        // mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::GREEN));
    }

    else if (lineColor == "YELLOW")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::YELLOW));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::YELLOW));
    }

    else if (lineColor == "AMBER")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::AMBER));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::AMBER));
    }

    else if (lineColor == "RED")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::RED));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::RED));
    }

    else if (lineColor == "MAGENTA")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::MAGENTA));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::MAGENTA));
    }

    else if (lineColor == "GRAY")
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::GRAY));
        // mvwprintw(mcdu, y, x, "%s", lineText.c_str());
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::GRAY));
    }

    else
    {
        if (textColPosition != 0)
            x = textColPosition; // Use textColPosition if it is not 0
        wattron(mcdu, COLOR_PAIR(MCDUColors::WHITE));
        // mvwaddstr(WINDOW * win, int y, int x, const char *str);
        mvwaddstr(mcdu, y, x, lineText.c_str());
        wrefresh(mcdu);
        wattroff(mcdu, COLOR_PAIR(MCDUColors::WHITE));
    }

    lineColor = "WHITE"; // Reset lineColor
    textColPosition = 0; // Reset textColPosition
}

string checkVars(string strLine)
/*
This function is used on drawLine to replace the @VAR(X) with the string stored in global variables VARX
*/
{
    size_t found;
    found = strLine.find("@VAR(1)");
    if (found != std::string::npos)
    {
        strLine = VAR1;
        return strLine;
    }

    found = strLine.find("@VAR(2)");
    if (found != std::string::npos)
    {
        strLine = VAR2;
        return strLine;
    }

    found = strLine.find("@VAR(3)");
    if (found != std::string::npos)
    {
        strLine = VAR3;
        return strLine;
    }

    found = strLine.find("@VAR(4)");
    if (found != std::string::npos)
    {
        strLine = VAR4;
        return strLine;
    }
    return strLine;
} // checkVars(string strLine)

void clearLine(WINDOW *mcdu, int y)
{
    wmove(mcdu, y, 0); // Move cursor to line y, col x
    wclrtoeol(mcdu);
    wrefresh(mcdu);
}

void mcduTimer(int t_sec)
{
    const string strRemainTime = "REMAINING TIME:";
    const string strTimerEnd = "TEST FINISHED";
    string strTimeLeft;
    int t_sleep = 3;

    do
    {
        clearLine(mcdu, LINES / 2);
        attron(A_BOLD);
        lineFont = BIGFONT; // Set Font
        // wrefresh(mcdu);
        // lineFont = BIGFONT; //Set Font
        mvwprintw(mcdu, LINES / 2, ((COLS / 2) - (strRemainTime.size() / 2)), "%s %d  ", strRemainTime.c_str(), t_sec);
        wrefresh(mcdu);
        t_sec--;
        sleep(1); // sleep( time_period ); time_period in seconds
    } while (t_sec >= 0);

    clearLine(mcdu, LINES / 2);
    attron(A_BOLD);
    lineFont = BIGFONT; // Set Font
    mvwprintw(mcdu, (LINES / 2), ((COLS / 2) - (strTimerEnd.size() / 2)), "%s", strTimerEnd.c_str());
    wrefresh(mcdu);
    sleep(t_sleep);
    attroff(A_BOLD);
    // wrefresh(mcdu);
    // lineFont = SMALLFONT; //Reset Font

    getPage(strTestPage);
    drawPage();
    // Reset Globals
    strTestPage = "";
    testPage = false;
}
