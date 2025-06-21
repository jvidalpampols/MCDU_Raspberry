//fileTools.h
// C++ Program to perform MCDU files handling

#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <bits/stdc++.h>
#include <cassert>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <cctype>
//#include <ncurses.h> /* ncurses.h includes stdio.h */
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;  //namespace needed for all  std types used
// Function declarations

// Note: In the case of functions, the extern keyword is used implicitly.
// The first two functions use "extern" keyboard while the rest don't use it.
// Neither compliler nor linker complains about that

int getMCDUPages(string strPath); // Opening the file name "MCDUPAGE.TXT"
                                  // and stores its content to allPages<string>vector.
// Formating functions
void removeEmptyLines(vector<string> &pages);
void removeComments(vector<string> &pages);
void allCapitals(vector<string> &pages);
void removeSpaces(vector<string> &pages);
void formating(vector<string> pages); // removeEmptyLines(allPages);
                                      // removeComments(allPages);
                                      // allCapitals(allPages);
                                      // remove Spaces(allPages);
bool getLskArrowCmds(string pageName);         // Get LSK command from string
                                      void trim(string &str);                      // Remove leading and trailing spaces from an string

void getLoadFile(string str);                // Get file name to be loaded into "loadFiles" string vector
bool loadFile(string fileName);              // Store contents of a file into "allPages" vector <string>
bool getPage(string pageName);               // getPage a "pageName" stored into "allPages" vector <string>.

bool getLSK(); // Get LSK, ARROW commands from  each string of each pageLine[]
int  getLSKIndex(string LSKCmd); // This function gets the index of LSK's command from string
int  getARROWIndex(string ARROWCmd); // This function gets the index of ARROW's command from string
int getStrCmdsIndex(string strCmd);  // This function gets the index of LSK's command from string

void showVectorString(vector<string> pages); // Show on screem all items of vector <string> argument
                                             // Used for testing

void createFile_allPages(); // Creates a text file to check allPages[] storage. Used for testing

void exitProgram(string str);
string getVar(string strLine, string strSearch);

//
extern void (*writeMCDU[])(string str, WINDOW *mcdu); // Array of functions to write on MCDU window
// Index= *writeMCDU[index]: Command Text, defined on enum txtCmds.
// str: Global string stored in: lineText;
//void pressAnyKey(); // Used for testing
void waitKeyPressed(void); // check if a key have been pressed
int kbhit(void); // check if a key have been pressed
//void createFile_MCDU_Data(const std::string &pageName);
void createFile_MCDU_Data(void);

void appendFile_MCDU_Data(void);
void checkForVars(string strLine);
void  checkForINFLIGHT(void); //To draw the page of INFLIGHT instead

extern bool inFlight; //Global to check if there is a INFLIGHT MCDU Page
extern string inFlightPage; //Global to keep INFLIGHT MCDU Page

    // Page Global variables declaration
    //  Note: In the case of variables, the extern keyword is used explicitly.
    // In the case of functions, the extern keyword is used implicitly(not necesary to use it).
extern vector<string> allPages; // To store all data files, that the program will manage
extern vector<string> loadFiles; // To store files names to be loaded to build up "allPages"
extern vector<string> pageLines; // To store lines of actual page
extern vector <string> pageCmds; // To store MCDU Page LSK + ARROW Cmd's
extern vector<string> strCmdsIndex; // To store LSK + ARROW commands
extern vector<string> strLineLSK; //To store de pageLine where defined LSK, 
                            //so as on run time to update VAR(X) and VARPAGEX

extern string lastPage; // To store last page name to be used by special Command $RETURN

// MCDU Global variables
extern string VAR1;
extern string VAR2;
extern string VAR3;
extern string VAR4; //Used for MCDU ERROR Page
extern string VARPAGE;
extern string VARPAGE1;
extern string VARPAGE2;
extern string VARPAGE3;
extern string VARPAGE4;

#endif
