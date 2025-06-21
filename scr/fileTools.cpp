// fileTools.cpp

#include <bits/stdc++.h>
#include <cassert>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <cctype>
// #include <ncurses.h> /* ncurses.h includes stdio.h */
#include <ncursesw/ncurses.h>
#include "locale.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

// C++ tools to perform MCDU files handling
// Function declarations

int getMCDUPages(string strPath); // Opening the file name "MCDUPAGE.TXT"
                                  // and stores its content to allPages<string>vector.
void getLoadFile(string str);     // Get file name to be loaded into "loadFiles" string vector
bool loadFile(string fileName);   // Store contents of a file into allPages
bool getPage(string pageName);    // getPage a "pageName" stored into allPages.
bool findPage(string pageName);   // New function used in getPage() to check for "INFLIGH" MCDU pages

// OLd functions
bool getLSK_Old();                  // Get LSK, ARROW commands from  each string of each pageLine[]
int getLSKIndex(string LSKCmd);     // This function gets the index of LSK's command from string
int getARROWIndex(string ARROWCmd); // This function gets the index of ARROW's command from string
int getStrCmdsIndex(string strCmd); // This function gets the index of LSK's command from string

// New functions for LSK and ARROW//////////////////
bool getLSK(string strLine);
bool getARROW(string strLine);
int getCmdIndex(string Cmd);
void resetPageCmds(void);

////////////////////////

void removeComments(vector<string> &pages);
void allCapitals(vector<string> &pages);
void removeEmptyLines(vector<string> &pages);
void removeSpaces(vector<string> &pages);
void formating(vector<string> pages);
void formatingAllpages();             // remove Spaces(allPages); removeComments(allPages); allCapitals(allPages);
                                      // removeEmptyLines(allPages);
void trimmingPageHeader(void);        // Trimming all lines PAGE = PAGENAME, so as better identification
void trim(string &str);               // Remove leading and trailing spaces from an string
string l_trim(const string &s);       // Trim from start (left)
string r_trim(const string &s);       // Trim from end (right)
string lr_trim(const std::string &s); // Trim from both ends

static inline void ltrim(std::string &s);      // Trim from start (in place)
static inline void rtrim(std::string &s);      // Trim from end (in place)
static inline void trimString(std::string &s); // Trim from both ends (in place)

void showVectorString(vector<string> pages); // Show on screem all items of vector <string> argument
void createFile_allPages();                  // Creates a text file to check allPages[] storage
void waitKeyPressed(void);                   // check if a key have been pressed
int kbhit(void);                             // used on waitKeyPressed(void);
void exitProgram(string str);                // Defined on MCDU_XX.cpp

string getVar(string strLine, string strSearch);
void getSpecialCmdVAR(string str, size_t initOffset);
void getSpecialCmdVARPAGE(string str, size_t initOffset);
void checkCmds(void);
void resetPageCmdsOld(void);
void createFile_MCDU_Data();
void appendFile_MCDU_Data();
void checkGOTO3SEC(void);              // To check if this page has "GOTO3SEC", so as to call timer routine
                                       // GOTO3SEC = MCDU PAGE, this page will be stored into global strTestPage = "";
                                       // This is the page to be caller after timer is over
                                       // timer routine should reset: globals strTestPage = "", and testPage = false;
string checkForINFLIGHTPage(void);     // Checks for INFLIGHT = MCDUPAGE. If so returns string "MCDUPAGE"
                                       // else returns an empty string "".
void checkForHistory(string pageName); // To set global historyPages "true" when no ARROW's keys in the Page and "false"
// when there is any
string getLastPage(void); // To manage historyPages vector<string> pop pages and remove pages,to avoid getting to empty vector

// Page Global variables declaration

vector<string> allPages;     // To store all files data the program will manage
vector<string> loadFiles;    // To store files names to be loaded
vector<string> pageLines;    // To store lines of actual page
vector<string> pageCmds;     // To store MCDU Page LSK + ARROW Cmd's
vector<string> strLineLSK;   // To store de pageLine where defined LSK,
                             // so as on run time to update VAR(X) and VARPAGEX
vector<string> historyPages; // To keep former pages so as to use on $RETURN
bool pageHistory = true;     // So as to not store pages when using ARROW's keys ARROWUP. ARROWDN, ARRORRH, ARROWLH
string lastPage;             // To store last page. Page name to be used by special Command $RETURN
string actualPage;           // To store actual page name to be used ????????????

bool testPage = false; // Flag to be set if the actual page contains "GOTO3SEC". If so
                       //  the checkGOTO3SEC(void); will store the page "testPage"
                       // this MCDU page will be called when the timer is over.
// void checkForINFLIGHT(void);
bool inFlight = false;    // Global to check if there is a INFLIGHT MCDU Page
string inFlightPage = ""; // Global to keep INFLIGHT MCDU Page

string strTestPage = "";
extern bool mcduFlightMode;
bool pageFound = false; // Global

// fiunctions to detect #NN
bool getCol(string strLine);
void cleanString(string &texto);
void removeSpaces(string &str);
bool isNumber(const string &texto);
int secureConvert(const string &texto);
// Reset textColPosition; Do it after drawLine();
extern int textColPosition; // To store #NN
extern bool returnPage;     // So as not to store the new page, because is already on historyPages and should be removed instead

vector<string>
    strCmdsIndex = {
        "LSK1L",
        "LSK2L",
        "LSK3L",
        "LSK4L",
        "LSK5L",
        "LSK6L",
        "LSK1R",
        "LSK2R",
        "LSK3R",
        "LSK4R",
        "LSK5R",
        "LSK6R",
        "ARROWUP",
        "ARROWDN",
        "ARROWLH",
        "ARROWRH"};

// MCDU Global variables
string VAR1 = "";
string VAR2 = "";
string VAR3 = "";
string VAR4 = ""; // Used for MCDU ERROR Page
string VARPAGE = "";
string VARPAGE1 = "";
string VARPAGE2 = "";
string VARPAGE3 = "";
string VARPAGE4 = "";

// Path for all MCDU files
string loadFilePath = "/mnt/mcdu/MCDU_Files/";
// string loadFilePath = "/home/avionica/Projects/MCDU/MCDU_Files/";

/// @brief
// this function loads file MCDUPAGE.txt
/// @param strPath

int getMCDUPages(string str)
{
    string line;

    // Opening the file name "MCDUPAGE.TXT", str is the full path to file "MCDUPAGE.TXT"
    // and stores its content to allPages<string>vector.
    // Note the extension file "txt", should be included in "str"
    // Each LOADFILE: XXXXXXX.TXT is loaded into allPages
    // On each line before is loaded into "allPages",
    // On each line comments are removed and trimmed RH and LH

    ifstream file(str); // Declaring file handle and assigning to "filepath" MCDUPAGE.TXT"

    // Read lines from File, store them into vector string "allPages", until it reaches the end "EOF"

    if (file.is_open())
    {
        while (getline(file, line)) // Until EOF
        {
            // storing each line to string vector "all Pages"
            allPages.push_back(line);
            // Debugging
            //cout << line << endl;
        }

        file.close();
    }

    // Debugging
    formatingAllpages();
    //cout << "MCDUPAGE.TXT Pages formated" << endl;
    //getchar();

    // Get string names of MCDU Files, and store into loadFile vector string

    for (size_t i = 0; i < allPages.size(); i++)
    {
        getLoadFile(allPages[i]);
    }

    // cout << "getLoadFiles OK" << endl;
    // getchar();

    for (size_t i = 0; i < loadFiles.size(); i++)
    {
        if (!loadFile(loadFiles[i]))
        {
            cout << "Failure to load file: " << loadFiles[i] << endl
                 << endl;
            cout << "Press any key to exit the program!!!\n"
                 << endl;
            getchar();
            return -1;
        }
    }

    // cout << "LoadFiles OK" << endl;
    // getchar();

    // Debugging
    formatingAllpages();

    // cout << "After formating \"allPages\", show all lines in vector <string> allPages: " << endl
    //      << endl;
    // showVectorString(allPages);
    // waitKeyPressed();
    //getchar();
    return 0;
} // getMCDUPages(string str)

void trimmingPageHeader(void)
// Remove all spaces from any line starting with "PAGE" so as better identification
{
    string strSearch = "PAGE";
    string strTemp; // Used for better clarity
    size_t found;
    // for each line "PAGE=" remove all spaces
    for (unsigned int i = 0; i < allPages.size(); i++) // Check all Pagelines
    {
        // strTemp = pageLines[i];
        found = allPages[i].find(strSearch);

        if ((found != string::npos) && found == 0)
        {
            strTemp = allPages[i];
            strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
            allPages[i] = strTemp;
        }
    } // trimmingPageHeader(void)
} // trimmingPageHeader(void)

// bool getLSK_Old() // Get LSK's and ARROW's command from string of each pageLine[]
//     {
//         // This function gets the LSK's and ARROW's commands from a string
//         // The LSK's and ARROW's commands are stored into pageCmds <string> vector
//         // The command string LSK's or ARROW's are used to string comparation with strCmdsIndex <string> vector
//         // so as to get the index on which the command is stored into strCmdsIndex <string> vector

//         string strTemp;
//         size_t found;
//         // Note pageLines has already been trimmed, on getMCDUPages(string str)
//         // so as we can apply fixes position tokens without the error of blank spaces
//         string strCheckLSK = "LSK";     // LSKxx= Command starts at 6th character position
//         string strCheckARROW = "ARROW"; // ARROWxx= Command starts at 8th character position

//         // init pageCmds
//         pageCmds.resize(strCmdsIndex.size(), ""); // Resize pageCmds to the size of strCmdsIndex
//                                              // Fill pageCmds with empty strings

//         // Check for LSKxx= Command
//         for (unsigned int i = 0; i < pageLines.size(); i++) // Check all Pagelines
//         {
//             // Check if LSKxx= is found
//             strTemp = pageLines[i];
//             found = pageLines[i].find(strCheckLSK);
//             if (found != string::npos)
//             {
//                 // get LSKxx= Command ends at found + 5th character position
//                 strTemp = strTemp.substr(found, found + 5);
//                 // Get the index to the end of: LSKxx
//                 int index = getLSKIndex(strTemp);
//                 if (index == -1)
//                 {
//                     cout << "LSK Command not found:" << strTemp << endl;
//                     exitProgram("LSK Command not found:" + strTemp);
//                 }
//                 // Get the LSK Command: MCDU page or Special Command
//                 // Note: LSKxx= MCDU Page or Special CMD starts at 6th character position
//                 strTemp = pageLines[i];
//                 strTemp = strTemp.substr(found + 6, strTemp.length() - (found + 6));
//                 //Trim LSK Command before store it
//                 //strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
//                 strTemp = lr_trim(strTemp);
//                 // Store into pageCmds using index
//                 pageCmds.at(index) = strTemp;
//             } // if LSK (found)

//         } // for LSK's Command's

//         // Check for ARROWUP, ARROWDN, ARROWLH, ARROWRH
//         for (unsigned int i = 0; i < pageLines.size(); i++) // Check all Pagelines
//         {
//             // Check if ARROWxx= is found
//             strTemp = pageLines[i];
//             found = pageLines[i].find(strCheckARROW);
//             if (found != string::npos)
//             {
//                 // get ARROWxx= Command ends at found + 7th character position
//                 strTemp = strTemp.substr(found, found + 7);
//                 // Get the index of LSKxx= Command
//                 int index = getARROWIndex(strTemp);
//                 if (index == -1)
//                 {
//                     cout << "ARROW Command not found:" << strTemp << endl;
//                     exitProgram("ARROW Command not found:" + strTemp);
//                 }
//                 // Get the ARROW Command: MCDU page or Special Command
//                 // Note: ARROWxx= MCDU Page or Special CMD starts at 8th character position
//                 strTemp = pageLines[i];
//                 strTemp = strTemp.substr(found + 8, strTemp.length() - (found + 8));

//                 // Trim LSK Command before store it
//                 strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());

//                 // Store into pageCmds using index
//                 pageCmds.at(index) = strTemp;
//             } // if LSK (found)

//         } // for ARROW's Command's
//         //checkCmds(); // To fill with ERROR page all LSK's and ARROW's not assigned an MCDU Page
//         return true;
//     } // bool getLSK(string pageName)

void checkCmds(void)
/*
 To fill with ERROR page all LSK's and ARROW's not assigned an MCDU Page
*/
{
    for (size_t i = 0; i < pageCmds.size(); i++)
    {
        if (pageCmds[i] == "")
            pageCmds[i] = "ERROR"; // To call ERROR defined page
    }
}

int getLSKIndex(string LSKCmd)
{
    // This function gets the index of LSK's command from string
    // The LSK's command is used to string comparation with strCmdsIndex <string> vector

    for (unsigned int i = 0; i < strCmdsIndex.size(); i++)
    {
        if (LSKCmd == strCmdsIndex[i])
            return i;
    }
    return -1;
} // getLSKIndex(string LSKCmd)

int getARROWIndex(string ARROWCmd)
{
    // This function gets the index of ARROW's command from string
    // The LSK's command is used to string comparation with strCmdsIndex <string> vector

    for (unsigned int i = 0; i < strCmdsIndex.size(); i++)
    {
        if (ARROWCmd == strCmdsIndex[i])
            return i;
    }
    return -1;
} // getLSKIndex(string lSKCmd)

int getStrCmdsIndex(string strCmd)
{
    // This function gets the index of LSK's command from string
    // The LSK's command is used to string comparation with strCmdsIndex <string> vector

    for (unsigned int i = 0; i < strCmdsIndex.size(); i++)
    {
        if (strCmd == strCmdsIndex[i])
            return i;
    }
    return -1;
} // getStrCmdsndex(string strCmd)

/////////////////// SPECIAL COMMANDS VAR(1-4), VARPAGE, VARPAGE1-4  START  //////////////////
void checkForVars(string strLine)
{
    // This function checks for special functions VAR(X), VARPAGE, VARPAGE1-4
    // and stores the text "VALUE" VAR(X) = "VALUE, XXXX" into global variable
    // VARX OR VARPAGE, VARPAGE 1-4
    // checkForVars(strLine) "pageLine[i]", checks for special function VAR(X), VARPAGE, VARPAGE1-4
    // and then calls to getVar(strLine, strSearch) to get the string "VALUE" assigned to VAR(X) or
    // VARPAGEx = VALUE.
    // The return string "VALUE" from getVar, is stored into global variable VARX or VARPAGE, VARPAGE1-4
    // On drawLine, if pageLine contains @VAR(X) will be replaced by the string stored into VARX
    // On getCmdLSK or getCmdARROW, if pageLine contains VARPAGE, VARPAGE1-4
    // will be replaced by the string stored into VARPAGE, VARPAGE1-4

    string strTemp = strLine;
    // Remove spaces on the LH and RH sides
    // To be implemented

    // Using this function:
    // Check for VAR(1)
    if (strLine.find("VAR(1)") != string::npos)
    {
        strTemp = getVar(strLine, "VAR(1)");
        // store into global variable VAR1 if not FAIL or EMPTY
        // if ((strTemp != "FAIL") && (strTemp != ""))
        if ((strTemp != "FAIL"))
            VAR1 = strTemp;
    }

    // Check for VAR(2)
    if (strLine.find("VAR(2)") != string::npos)
    {
        strTemp = getVar(strLine, "VAR(2)"); // store into global variable VAR2
                                             // store into global variable VAR2 if not FAIL or EMPTY
                                             // if ((strTemp != "FAIL") && (strTemp != ""))
        if (strTemp != "FAIL")
            VAR2 = strTemp;
    }

    // Check for VAR(3)
    if (strLine.find("VAR(3)") != string::npos)
    {
        strTemp = getVar(strLine, "VAR(3)"); // store into global variable VAR3
        // store into global variable VAR3
        // if ((strTemp != "FAIL") && (strTemp != ""))
        if (strTemp != "FAIL")
            VAR3 = strTemp;
    }

    // Check for VAR(4) // Reserved for page name to be showed up on ERROR Page
    if (strLine.find("VAR(4)") != string::npos)
    {
        strTemp = getVar(strLine, "VAR(4)"); // store into global variable VAR4
        // store into global variable VAR4
        // if ((strTemp != "FAIL") && (strTemp != ""))
        if (strTemp != "FAIL")
            VAR4 = strTemp;
    }

    // Check for VAR("VARPAGE1")
    if (strLine.find("VARPAGE1") != string::npos)
    {
        strTemp = getVar(strLine, "VARPAGE1");
        if (strTemp != "FAIL")
        {
            // Trim all spaces from "PAGENAME"
            strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
            // store into global variable VARPAGE1
            VARPAGE1 = strTemp;
        }
    }

    // Check for VAR("VARPAGE2")
    if (strLine.find("VARPAGE2") != string::npos)
    {
        strTemp = getVar(strLine, "VARPAGE2");
        if (strTemp != "FAIL")
        {
            // Trim all spaces from "PAGENAME"
            strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
            // store into global variable VARPAGE2
            VARPAGE2 = strTemp;
        }
    }

    // Check for VAR("VARPAGE3")
    if (strLine.find("VARPAGE3") != string::npos)
    {
        strTemp = getVar(strLine, "VARPAGE3");
        if (strTemp != "FAIL")
        {
            // Trim all spaces from "PAGENAME3"
            strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
            // store into global variable VARPAGE3
            VARPAGE3 = strTemp;
        }
    }

    // Check for VAR("VARPAGE4")
    if (strLine.find("VARPAGE4") != string::npos)
    {
        strTemp = getVar(strLine, "VARPAGE4");
        if (strTemp != "FAIL")
        {
            // Trim all spaces from "PAGENAME4"
            strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
            // store into global variable VARPAGE4
            VARPAGE4 = strTemp;
        }
    }

    // VARPAGE should be the last one, because VARPAGE is met by VARPAGEX
    // Pending to fix
    // Check for VAR("VARPAGE")
    if (strLine.find("VARPAGE=") != string::npos)
    {

        strTemp = getVar(strLine, "VARPAGE=");
        if (strTemp != "FAIL")
        {
            // Trim all spaces from "PAGENAME"
            strTemp.erase(remove(strTemp.begin(), strTemp.end(), ' '), strTemp.end());
            // store into global variable VARPAGE
            VARPAGE = strTemp;
        }
    }
    return;

} // CheckForVars(string strLine)

string getVar(string strLine, string strSearch)
{
    // strSearch is the string "VAR(X)" or "VARPAGE" to be searched to.
    // This function checks for special function VAR(X), VARPAGE, VARPAGE1-4
    // and returns the text "XXXX, VAR(X) = "XXXX" to be stored into global variable:
    // VARX, VARPAGE or VARPAGE1-4.
    // TEXT: On drawLine, if pageLine contains @VAR(X) will be replaced by the string stored into VARX
    // PAGES LSK'x, ARROW's: On getCmdLSK or getCmdARROW, if pageLine contains VARPAGE, VARPAGE1-4
    // will be replaced by the string stored into VARPAGE, VARPAGE1-4

    // string strSubVar;          // To get the string "VALUE" assigned to @VARPAGE = VALUE
    string strSubVarEnd = "="; // To check for any @VARPAGE = VALUE,
    size_t startOffset;
    size_t endOffset;
    // char target = ',';
    bool found = false;

    startOffset = strLine.find(strSearch); // get to the start of strSearch VAR(X) =
                                           // or VARPAGE=, VARPAGE1-4 =
    if (startOffset == string::npos)
        exitProgram("Failed to getVAR");

    // Looking for the "="
    for (size_t i = startOffset; i < strLine.length(); i++)
    {
        if (strLine[i] == '=')
        {
            startOffset = i + 1; // To skip the "="
            found = true;
            break;
        }
    } // for

    if (found == false)
        return "FAIL";

    // Looking for ',' VAR(X)=XXXX, end of value
    for (size_t i = startOffset; i < strLine.length(); i++) // Looking for ',' VAR(X)=XXXX,
    {
        if ((strLine[i] == ','))
        {
            endOffset = i; // Offset at ',' VAR(X)=XXXX, or VARPAGE=XXXX,
            return strLine.substr(startOffset, endOffset - startOffset);
        }
    } // for

    endOffset = strLine.length(); // if no comma found ',' Return the whole string
    return strLine.substr(startOffset, endOffset - startOffset);

} // string getVAR(string str)

////////////////////  SPECIAL COMMANDS @VAR(1-4), VARPAGE, VARPAGE1-4  END  //////////////////

void removeComments(vector<string> &pages)
{
    // This function removes all comments of strings stored into vector string
    size_t found;
    string strTemp;

    for (unsigned int i = 0; i < pages.size(); i++)
    {
        strTemp = pages[i];
        found = strTemp.find(';');

        if (found != std::string::npos)
        { // if found ';'
            strTemp = strTemp.substr(0, found);
        } // if

        pages[i] = strTemp;

        // if (strTemp.empty())
    } // for

    // return;
}

void allCapitals(vector<string> &str)
{

    // This function makes capital letters all strings stored into vector
    string strTemp;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        for (auto &c : str[i])
            c = (char)toupper(c);

    } // for pages
}

void removeEmptyLines(vector<string> &pages)
{
    // This function removes all empty lines stored into vector
    string strTemp;

    for (unsigned int i = 0; i < pages.size(); i++)
    {
        if (pages[i].empty())
        {
            pages.erase(pages.begin() + i);
            i--;
        }
    }
}

void removeSpaces(vector<string> &pages)
{
    // This function removes all spaces left and right of strings stored into vector

    for (unsigned int i = 0; i < pages.size(); i++)
    {
        // pages[i].erase(remove_if(pages[i].begin(), pages[i].end(), ::isspace), pages[i].end());
        trim(pages[i]);
    }
}

void trim(string &str)
{
    // This function removes left and right spaces
    int i = 0;

    // left trim
    while (isspace(str[i]) != 0)
        i++;
    str = str.substr(i, str.length() - i);

    // right trim
    i = str.length() - 1;
    while (isspace(str[i]) != 0)
        i--;
    str = str.substr(0, i + 1);
}

string l_trim(const string &s)
// Trim from start (left)
{
    return string(
        find_if(s.begin(), s.end(), [](unsigned char ch)
                { return !isspace(ch); }),
        s.end());
}

string r_trim(const string &s)
// Trim from end (right)
{
    return std::string(
        s.begin(),
        find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                { return !isspace(ch); })
            .base());
}

string lr_trim(const std::string &s)
// Trim from both ends
{
    return l_trim(r_trim(s));
}

void getLoadFile(string str)
{
    // This function checks on file ""MCDUPAGE.TXT", wheather the string argument is a file to be loaded
    // into: "loadFiles" vector <string>
    //  string str to be checked = "LOADFILE=XXXXXX.TXT";
    string strSubInit = "LOADFILE=";
    string strSubEnd = ".TXT";

    // Remove spaces
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    size_t initOffset = str.find(strSubInit);
    if (initOffset != string::npos)
    {
        // Testing cout << "\"LOADFILE=\" found at: " << initOffset << '\n';
        initOffset = initOffset + 9; // Position right after '='sign
    }
    else
        return;

    size_t endOffset = str.find(strSubEnd);
    if (endOffset != string::npos)
    {
        // Testing std::cout << "\".TXT\" found at: " << endOffset << '\n';
        endOffset = endOffset + 4;
        loadFiles.push_back(str.substr(initOffset, endOffset - initOffset));
    }

    else
        return;
}

bool loadFile(string fileName)
{
    // This functions load the contents of a file stored into string vector "loadFiles"
    // into string vector "allPages"
    // Note Linux is case sensitive so .TXT should in capital letters,
    string line;
    string pathString = loadFilePath;
    pathString.append(fileName);
    
    // cout << "Now in loadFile() function" << endl;
    // getchar();

    // Opening the file
    ifstream newFile(pathString); // Declaring file handle and assigning to "filepath"

    if (newFile.is_open())
    {
        while (getline(newFile, line)) // Until EOF
        {
            // storing each line to string vector "all Pages"
            allPages.push_back(line);
            // DEBUGGING 
            //cout << line << endl;
        }
        // cout << "Now closing newFile" << endl;
        // getchar();
        newFile.close();
    }

    else
    {
        exitProgram("Fail openning the file:" + fileName);
        getchar();
        return false;
    }

    // cout << "PathFile loaded:" << fileName << endl;
    // cout << "Press any key to continue" << endl;
    // getchar();

    return true;
}

void showVectorString(vector<string> pages)
{
    // Test function for DEBUGGING
    //  Show on screem all items of vector <string> argument

    for (unsigned int i = 0; i < pages.size(); i++)
        cout << pages[i] << endl;

    cout << endl;
    cout << "Vector String Size:" << pages.size() << endl
         << endl;
}

void createFile_allPages()
{
    // This function should be used before iniciate ncurses initscr();
    //  creates a file to store allPages <string> vector

    ofstream myfile;

    myfile.open("allPages.txt");

    if (myfile.is_open())
    {
        for (unsigned int i = 0; i < allPages.size(); i++)
            myfile << allPages[i] << endl;

        myfile.close();
    }
    else
        exitProgram("Unable to open file allPages.txt");
    return;
}

void appendFile_MCDU_Data()
{
    auto now = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(now);

    string myfile = "/home/avionica/Projects/MCDU/bin/MCDU_DATA.txt";

    // Open the file in append mode
    ofstream outfile;
    outfile.open(myfile, ios::app); // std::ios::app means "append"

    if (outfile)
    {
        outfile << "\n\nMCDU Actual Page:" << actualPage << endl;

        outfile << "Current date and time: "
                << put_time(localtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
                << endl;

        outfile << "PAGE LINES:" << endl;
        for (unsigned int i = 0; i < pageLines.size(); i++)
            outfile << pageLines[i] << endl;

        outfile << "\nPAGE COMMANDS:\n"
                << endl;
        for (unsigned int i = 0; i < pageCmds.size(); i++)
            outfile << strCmdsIndex[i] << ':' << pageCmds[i] << endl;

        outfile << "\nPAGE COMMANDS UPDATED ON runTimeCmds()\n"
                << endl;
        outfile << "Complete LSK: MCDUPAGE\n"
                << endl;
        for (unsigned int i = 0; i < strLineLSK.size(); i++)
            outfile << strLineLSK[i] << ':' << pageCmds[i] << endl;

        // outfile << "PAGE LSK & ARROW" << endl;
        // for (unsigned int i = 0; i < strCmdsIndex.size(); i++)
        //     outfile << strCmdsIndex[i] << ':' << pageCmds[i] << endl;

        outfile << "VAR(X)" << endl;
        outfile << "VAR(1)=" << VAR1 << endl;
        outfile << "VAR(2)=" << VAR2 << endl;
        outfile << "VAR(3)=" << VAR3 << endl;
        outfile << "VAR(4)=" << VAR4 << endl;

        outfile << "VARPAGE(X)" << endl;
        outfile << "VARPAGE=" << VARPAGE << endl;
        outfile << "VARPAGE(1)=" << VARPAGE1 << endl;
        outfile << "VARPAGE(2)=" << VARPAGE2 << endl;
        outfile << "VARPAGE(3)=" << VARPAGE3 << endl;
        outfile << "VARPAGE(4)=" << VARPAGE4 << endl;
        outfile << "TEST FLAG=" << testPage << endl;
        outfile << "MCDU TEST PAGE=" << strTestPage << endl;
        outfile << "MCDU FLIGHT MODE=" << mcduFlightMode << endl;
        for (size_t i = 0; i < historyPages.size(); i++)
        {
            outfile << "Pages History[]" << i << "]" << historyPages[i] << endl;
        }
    }
    // else
    //     cout << "Unable to open file";
    outfile.close();
    return;
}

void createFile_MCDU_Data(void)
{
    // This function should be used to test all VAR(X) and VARPAGEX

    ofstream myfile;
    auto now = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(now);

    myfile.open("MCDU_DATA.txt");

    if (myfile.is_open())
    {
        myfile << "MCDU PAGE=" << actualPage << endl;

        myfile << "\nCurrent date and time: "
               << put_time(localtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
               << endl;

        myfile << "PAGE LINES" << endl;
        for (unsigned int i = 0; i < pageLines.size(); i++)
            myfile << pageLines[i] << endl;

        myfile << "PAGE COMMANDS" << endl;
        for (unsigned int i = 0; i < pageCmds.size(); i++)
            myfile << strCmdsIndex[i] << "=" << pageCmds[i] << endl;

        // myfile << "PAGE LSK & ARROW" << endl;
        // for (unsigned int i = 0; i < strCmdsIndex.size(); i++)
        //     myfile << strCmdsIndex[i] << endl;

        myfile << "TEXT VAR" << endl;
        myfile << "VAR(1)=" << VAR1 << endl;
        myfile << "VAR(2)=" << VAR2 << endl;
        myfile << "VAR(3)=" << VAR3 << endl;
        myfile << "VAR(4)=" << VAR4 << endl;

        myfile << "VARPAGE's" << endl;
        myfile << "VARPAGE=" << VARPAGE << endl;
        myfile << "VARPAGE(1)=" << VARPAGE1 << endl;
        myfile << "VARPAGE(2)=" << VARPAGE2 << endl;
        myfile << "VARPAGE(3)=" << VARPAGE3 << endl;
        myfile << "VARPAGE(4)=" << VARPAGE4 << endl;
        myfile << "TEST FLAG=" << testPage << endl;
        myfile << "MCDU TEST PAGE=" << strTestPage << endl;

        myfile.close();
    }
    else
        exitProgram("Unable to open file: MCDU_DATA.txt");

    return;
}

void formating(vector<string> pages)
{
    removeSpaces(pages);
    removeComments(pages);
    allCapitals(pages);
    removeEmptyLines(pages);
}
void formatingAllpages()
{
    removeSpaces(allPages);
    removeComments(allPages);
    allCapitals(allPages);
    removeEmptyLines(allPages);
    trimmingPageHeader();
}

int kbhit()
// Returns the keys on keyboard buffer.
// If no key have been pressed returns '0', else returns > 0
// Note: it does not return ASCII value of key pressed
{
    int k;
    ioctl(STDIN_FILENO, FIONREAD, &k);
    return (k);
}

void waitKeyPressed()
{
    // First: Flusing keyboard buffer
    // while (kbhit())
    getchar();
    // Second: Wait for a key hit
    // while (1)
    // {

    //     /* check for any key */
    //     if (kbhit())
    //     {
    //         getch();
    //         break;
    //     }
    // }
} // int kbhit()

bool findPage(string pageName) // New function used in getPage() to check for "INFLIGH" MCDU pages
{
    // into allPages[] vector<string>, find out a line that contains:
    // PAGE=PAGENAME.
    // Set indexStart
    // Next find out the first line that contains:
    // ENDPAGE.
    // Set indexEnd
    // Store the lines between indexes into new List: pagelines[].

    string strTemp;
    string strCheckInit;
    string strCheckEnd = "ENDPAGE";

    // string strCheck = "PAGE=FIRSTPAGE";
    int indexStart = 0;
    int indexEnd = 0;
    size_t index = 0;
    // size_t found;
    bool pageInit = false;

    strCheckInit = "PAGE=" + pageName;

    lastPage = actualPage; // keep last page for $RETURN

    for (index = 0; index < allPages.size(); index++)
    {
        if (allPages[index] == strCheckInit)
        {
            indexStart = index;
            // cout << "Encontrado Inicio de pagina: " << allPages[i] << endl;
            pageInit = true;
            break;
        }
    }

    if (pageInit == true)
    {
        for (index = indexStart; index < allPages.size(); index++)
        {
            if (allPages[index] == strCheckEnd)
            {
                indexEnd = index;
                // cout << "Encontrado final de pagina: " << allPages[i] << endl;
                pageFound = true;
                break;
            }
        }
    } // if pageInit == true

    if (pageFound)
    {
        // Reset pageLines
        pageLines.clear(); // Clear pagelines to store a new MCDU PAGE
        resetPageCmds();
        // DEBUGGING cout << "Page Found: " << "PAGE=" + pageName << endl << endl;

        // Load MCDU Page into pageLines vector <string>
        for (int i = 0; i <= (indexEnd - indexStart); i++)
        {

            pageLines.push_back(allPages[indexStart + i]);
            // DEBUGGING cout << pageLines[i] << endl;
        }
    } // if (pageFound)

    return pageFound; // Global BOOL to check if page have been loaded into pageLines[]
}

bool getPage(string pageName)
{
    string strFlightPage = "";
    pageFound = false; // Global BOOL to check if page have been loaded into pageLines[]

    // Check for MISC page name
    //  If mcduFlightMode == true >> load MISC_FLT
    //  If mcduFlightMode == false >> load MISC_GND
    if (pageName == "MISC")
    {
        if (mcduFlightMode == true)
            pageName = "MISC_FLT";
        if (mcduFlightMode == false)
            pageName = "MISC_GND";
    }

    // Get the page

    pageFound = findPage(pageName);

    if (!pageFound)
    {
        exitProgram("Page not found:" + pageName); // Will load ERROR page instead
        VAR4 = "NOT FOUND";
        // appendFile_MCDU_Data();
        return false;
    } // if (!pageFound)

    // Check if there is an INFLIGHT Page and no MISC page, so as to update the new INFLIGHT PAGE
    // Importan to distinguish between "INFLIGHT" asociated to MISC LSK4L = INFLIGHT on MCDU Page MISC
    // and "INFLIGHT"  as "SPECIAL CMD" on a MCDUPAGE, definning the page to be shown on Flighit MODE:
    // INFLIGHT = MCDUPAGE
    if ((mcduFlightMode == true) && (pageName != "MISC_GND") && (pageName != "MISC_FLT"))
    {
        strFlightPage = checkForINFLIGHTPage(); // Check for INFLIGHT MCDU PAGE

        if (!strFlightPage.empty()) // If there is a Flight Page find it!!
        {
            pageFound = findPage(strFlightPage); // Find and load into pageLines the MCDU FLIGHT PAGE
        }

        if (!pageFound)
        {
            exitProgram("Page not found:" + strFlightPage); // Will load ERROR page instead
            VAR4 = "NOT FOUND";
            return false;
        } // if (!pageFound)
    } // if(mcduFlightMode == true)

    if (pageFound)
    {
        for (size_t i = 0; i < pageLines.size(); i++)
        {
            checkForVars(pageLines[i]); // Check for special function VAR(1-4), VARPAGE, VARPAGE1-4
            getARROW(pageLines[i]);
            getLSK(pageLines[i]);
        }

        lastPage = actualPage;
        actualPage = pageName; // In case of ERROR Store actualPage = lastPage $RETURN when ERROR page
                               // When no ERROR, $RETURN will bring us to former page
                               // actualPage, will be used in former updates

        checkGOTO3SEC(); // To check if this page has "GOTO3SEC", so as to call timer routine
                         // GOTO3SEC = MCDU PAGE, this page will be stored into global strTestPage = "";
                         // This is the page to be caller after timer is over
                         // timer routine should reset: globals strTestPage = "", and testPage = false;

        // For debugging
        appendFile_MCDU_Data();
        if (returnPage == false)
        {
            checkForHistory(lastPage); // To set global historyPages "true" when no ARROW's keys in the Page and "false"
                                       // when there is any
        }

        return true;
    } // if (pageFound)

    // cout << "\nPress any key to continue!!!\n";
    return true;

} // getPage

// bool getPage_Old(string pageName)
// {
//     // into allPages[] vector<string>, find out a line that contains:
//     // PAGE=PAGENAME.
//     // Set indexStart
//     // Next find out the first line that contains:
//     // ENDPAGE.
//     // Set indexEnd
//     // Store the lines between indexes into new List: pagelines.

//     string strTemp;
//     string strCheckInit = "PAGE=" + pageName;
//     string strCheckEnd = "ENDPAGE";

//     // string strCheck = "PAGE=FIRSTPAGE";
//     int indexStart = 0;
//     int indexEnd = 0;
//     size_t found;
//     bool pageInit = false;
//     bool pageFound = false;

//     lastPage = actualPage; //keep last page for $RETURN
//     // Reset pageLines
//     pageLines.clear(); // Clear pagelines to store a new MCDU PAGE

//     //Looking forward the new MCDU Page
//     for (unsigned int i = 0; i < allPages.size(); i++)
//     {

//         found = allPages[i].find(strCheckInit);
//         if (found != std::string::npos) // Si se encuentra "PAGE=" + pageName;
//         {
//             indexStart = i;
//             // cout << "Encontrado inicio de pagina: " << allPages[i] << endl;
//             pageInit = true;
//         }

//         if (pageInit == true)
//         {
//             found = allPages[i].find(strCheckEnd);
//             if (found != std::string::npos) // Si se encuentra "ENDPAGE";
//             {
//                 indexEnd = i;
//                 // cout << "Encontrado final de pagina: " << allPages[i] << endl;
//                 pageFound = true;
//                 break;
//             }
//         }
//     } // For

//     if (pageFound)
//     {
//         resetPageCmds();
//         // cout << "Page Found: " << "PAGE=" + pageName << endl << endl;
//         for (int i = 0; i <= (indexEnd - indexStart); i++)
//         {
//             pageLines.push_back(allPages[indexStart + i]);
//             // cout << pageLines[i] << endl;
//         }
//     }

//     if (pageFound)
//     {
//         for (size_t i = 0; i < pageLines.size(); i++)
//         {
//             checkForVars(pageLines[i]); // Check for special function VAR(1-4), VARPAGE, VARPAGE1-4
//             getARROW(pageLines[i]);
//             getLSK(pageLines[i]);
//         }

//         //getLSK();            // Get LSK, ARROW commands from  the actual MCDU  pageLine[]

//         //if (pageName != "ERROR") //???? check this!!!
//         lastPage = actualPage;
//         actualPage = pageName; // In case of ERROR Store actualPage = lastPage $RETURN when ERROR page
//                                     // When no ERROR, $RETURN will bring us to former page
//                                     //actualPage, will be used in former updates

//         checkGOTO3SEC(); // To check if this page has "GOTO3SEC", so as to call timer routine
//                          // GOTO3SEC = MCDU PAGE, this page will be stored into global strTestPage = "";
//                          // This is the page to be caller after timer is over
//                          // timer routine should reset: globals strTestPage = "", and testPage = false;

//         // For debugging
//         appendFile_MCDU_Data();
//         return true;
//     }
//     else
//     {
//         exitProgram("Page not found:" + pageName); // Will load ERROR page instead
//         VAR4 = "NOT FOUND";
//         appendFile_MCDU_Data();
//         return false;
//     }

//     // cout << "\nPress any key to continue!!!\n";
//     return true;

// } // getPageOld

string checkForINFLIGHTPage(void)
{
    // To draw the page of INFLIGHT = MCDUPAGE,  instead of loaded MCDUPAGE
    // returns the MCDUPAGE if INFLIGHT = MCDUPAGE or an empty string ""

    bool foundInFlt = false;
    // bool foundFltPage = false;
    size_t index = 0;
    string strInFlt = "INFLIGHT";

    // Reset globals
    inFlightPage = "";
    inFlight = false;

    // Check for "INFLIGHT"
    for (size_t i = 0; i < pageLines.size(); i++)
    {
        index = pageLines[i].find(strInFlt);
        if (index != string::npos)
        {
            foundInFlt = true;
            index = i;
            break;
        }
    } // for (size_t i = 0; i < pageLines.size(), i++)

    if (foundInFlt)
    {
        size_t res = pageLines[index].find('='); // Find '='
        if (res == string::npos)                 // if '=' is not found, return
        {
            return ""; // Empty string
        }

        // Get substring with the MCDU page
        res = res + 1;                               // skip '='
        inFlightPage = pageLines[index].substr(res); // Store Flight Page
        trim(inFlightPage);                          // Trim both ends of string
        inFlight = true;                             // Don't used yet
        return inFlightPage;                         // returns an MCDU FLIGHT PAGE
    } // if(foundInFlt)

    return inFlightPage;
} // string checkForINFLIGHTPage(void)

// Decoder files

void checkGOTO3SEC(void)
// this function checks for "GOTO3SEC" label into actual MCDU page.
// if found, sets a flag and retrieve the MCDU page as argument: GOTO3SEC = MCDUPAGE
// To check if this page has "GOTO3SEC", so as to call timer routine
// GOTO3SEC = MCDU PAGE, this page will be stored into global strTestPage = "";
// This is the page to be caller after timer is over
// timer routine should reset: globals strTestPage = "", and testPage = false;
{
    // int offsetStart;
    string strSearch = "GOTO3SEC";
    string strTemp;
    size_t found;
    bool strFound = false;

    for (unsigned int i = 0; i < pageLines.size(); i++)
    {
        found = pageLines[i].find(strSearch);
        if (found != string::npos)
        {
            strTemp = pageLines[i].substr(found); // substring from start of "GOTO3SEC" to end of string
            strFound = true;
            break;
        } // if
    } // for

    if (strFound == true) // Find '=' and skip it
    {
        for (unsigned int i = 0; i < strTemp.size(); i++)
        {
            if (strTemp[i] == '=')
            {
                strTemp = strTemp.substr(i + 1); // Skip '=' and take the substring
                strTemp = lr_trim(strTemp);      // Trim LH and RH sides
                testPage = true;
                // store MCDU Test page into global
                strTestPage = strTemp;
                // return;
            }
        } // For
    } // if(found == true) // Find '=' and skip it

    // Check for $VARPAGE's
    if (testPage == true)
    {
        if (strTestPage == "$VARPAGE")
            strTestPage = VARPAGE;
        if (strTestPage == "$VARPAGE1")
            strTestPage = VARPAGE1;
        if (strTestPage == "$VARPAGE2")
            strTestPage = VARPAGE2;
        if (strTestPage == "$VARPAGE3")
            strTestPage = VARPAGE3;
        if (strTestPage == "$VARPAGE4")
            strTestPage = VARPAGE4;
        return;
    }
    else // Reset Globals
    {
        testPage = false;
        strTestPage = "";
    }

} // void checkGOTO3SEC(void)

bool getLSK(string strLine)
// On a pageLine, checks for an LSKXX commad.
// If found, gets and stores on the right vector<string> pageCmds, the MCDU PAGE associate to LSK
// To associate uses the vector <string> strComds.
{
    string strSearch = "LSK";
    string strLSKCmd;
    string strLSKPage;

    string strTemp;
    // size_t found; // Removed unused variable
    size_t offsetStart;
    size_t offsetEnd;
    int indexLSKCmd;
    char target; // To be used to find '=' and ','

    // Check if LSKXX = is found
    offsetStart = strLine.find(strSearch);
    if (offsetStart == string::npos)
        return false; // LSKXX not found

    // Next find offsetStart from offsetStar to '=' LSKXX = XXXXXXXXXX
    target = '=';
    offsetEnd = strLine.find(target, offsetStart);
    if (offsetEnd == std::string::npos) // If not found
        return false;
    // Next will get a substring of LSK Command
    strLSKCmd = strLine.substr(offsetStart, offsetEnd - offsetStart);
    // Now will trim the string so as to get rid of blanks chars
    trim(strLSKCmd);
    // Next will check for a MCDU PAGE associated to this LSK Command.
    // So as will reverse the offset index
    offsetStart = offsetEnd;
    offsetStart++; // To skip '='
    // Now the target is a comma ',' if there are other "special functions", VAR's, etc
    // If not found comma ',', then will take the string from offsetStart to the end
    target = ',';
    offsetEnd = strLine.find(target, offsetStart);
    if (offsetEnd != std::string::npos) // If comma is found, the string from offsetStart to the comma
                                        // is the MCDU PAGE
        strLSKPage = strLine.substr(offsetStart, offsetEnd - offsetStart);

    else // will take string from offsetStart to the end of string
        strLSKPage = strLine.substr(offsetStart);

    // Once we already have the LSK Command and the MCDU PAGE associated, we should move it to
    //  vector<string> pageCmds, using vector <string> strComds "strLSKCmd".
    indexLSKCmd = getCmdIndex(strLSKCmd);

    if (indexLSKCmd == -1)
    {
        cerr << "[ERROR] LSK Command '" << strLSKCmd << "' not found in strComds!" << endl;
        return false;
    } // getCmdIndex(string LSKCmd)
    pageCmds[indexLSKCmd] = strLSKPage;
    strLineLSK[indexLSKCmd] = strLine; // Store strLine so as to update VAR(X) and VARPAGEX on runTimeCmds()
    return true;
} // getLSK(string strLine)

bool getARROW(string strLine)
// On a pageLine, checks for an LSKXX commad.
// If found, gets and stores on the right vector<string> pageCmds, the MCDU PAGE associate to LSK
// To associate uses the vector <string> strComds.
{
    string strSearch = "ARROW";
    string strARROWCmd;
    string strARROWPage;

    string strTemp;
    // size_t found; // Removed unused variable
    size_t offsetStart;
    size_t offsetEnd;
    int indexARROWCmd;
    char target; // To be used to find '=' and ','

    // Check if ARROWXX = is found
    offsetStart = strLine.find(strSearch);
    if (offsetStart == string::npos)
        return false; // ARROWXX is not found

    // Next find offsetEnd from offsetStar to '=' ARROWXX = XXXXXXXXXX
    target = '=';
    offsetEnd = strLine.find(target, offsetStart);
    if (offsetEnd == std::string::npos) // If not found
        return false;
    // Next will get a substring of ARROW Command
    strARROWCmd = strLine.substr(offsetStart, offsetEnd - offsetStart);
    // Now will trim the string so as to get rid of blanks chars
    trim(strARROWCmd);
    // Next will check for a MCDU PAGE associated to this LSK Command.
    // So as will reverse the offset index
    offsetStart = offsetEnd;
    offsetStart++; // To skip '='
    // Now the target is a comma ',' if there are other "special functions", VAR's, etc
    // If not found comma ',', then will take the string from offsetStart to the end
    target = ',';
    offsetEnd = strLine.find(target, offsetStart);
    if (offsetEnd != std::string::npos) // If comma is found, the string from offsetStart to the comma
                                        // is the MCDU PAGE
        strARROWPage = strLine.substr(offsetStart, offsetEnd - offsetStart);

    else // will take string from offsetStart to the end of string
        strARROWPage = strLine.substr(offsetStart);

    // Once we already have the LSK Command and the MCDU PAGE associated, we should move it to
    //  vector<string> pageCmds, using vector <string> strComds "strLSKCmd".
    indexARROWCmd = getCmdIndex(strARROWCmd);

    if (indexARROWCmd == -1)
    {
        cerr << "[ERROR] ARROW Command '" << strARROWCmd << "' not found in strComds!" << endl;
        return false;
    } // getARROWIndex(string LSKCmd)
    pageCmds[indexARROWCmd] = strARROWPage;
    return true;
} // bool getARROW(string strLine)

int getCmdIndex(string Cmd)
{
    // This function gets the index of LSK's command or AROW's from a vector <string> strComds.
    // The LSK's or ARROW's command is used to string comparation with strCmdsIndex <string> vector
    // Used internally by getLSK and getARROW

    // int index = strCmdsIndex.size();

    for (unsigned int i = 0; i < strCmdsIndex.size(); i++)
    {
        if (Cmd == strCmdsIndex[i])
            return i;
    }
    return -1;
}

void resetPageCmds(void)
// Fill the vector<string> pageCmds with empty strings
// Fill the vector<string> strLineLSK with empty strings

{
    pageCmds.clear();
    pageCmds.resize(strCmdsIndex.size());
    strLineLSK.clear();
    strLineLSK.resize(strCmdsIndex.size());
    // cout << "PageCmds Size: " << pageCmds.size() << endl;
}

bool getCol(string strLine)
// returns a true if found #3NN
// the #NN, is stored into global "strTemp"
{
    string strTemp;
    bool foundStart = false;
    bool foundEnd = false;
    size_t offsetStart = 0;
    size_t offsetEnd = 0;
    ;

    // Reset textColPosition; Do it after drawLine();
    textColPosition = 0;

    // Checking for #
    for (size_t i = 0; i < strLine.size(); i++)
    {
        if (strLine[i] == '#')
        {
            offsetStart = i;
            offsetStart++; // To skip '#' >> #NN,    yyyyy
            foundStart = true;
        }

        // Checking for next options if any. If no more options will take the string upto its end
        if ((offsetStart > 0) && (strLine[i] == ','))
        {
            offsetEnd = i;
            foundEnd = true;
            break;
        }
    } // for

    if (offsetStart == 0)
        return false; // No #NN

    // Only "#' start >> #NN
    if ((offsetStart > 0) && (offsetEnd == 0))
    {
        strTemp = strLine.substr(offsetStart); // Get string from offsetStart to end

        trimString(strTemp); // Trim the string, so no blanks chars

        if (isNumber(strTemp)) // Check if the string is a number
        {
            textColPosition = stoi(strTemp); // store value
        }

        return true;
    }

    //"#' start and end >> #NN, YYYY, ZZZZZ
    if ((offsetStart > 0) && (offsetEnd > 0))
    {
        strTemp = strLine.substr(offsetStart, (offsetEnd - offsetStart)); // Getting the string
        // cleanString(strTemp); //Trim the string, so no blanks chars
        trimString(strTemp);   // Trim the string, so no blanks chars
        if (isNumber(strTemp)) // Check if the string is a number
        {
            textColPosition = stoi(strTemp); // store value
        }
        return true;
    }
    return false;
} // bool getCol(string strLine)

static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// Trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// Trim from both ends (in place)
static inline void trimString(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

void cleanString(string &texto)
{
    // This function removes the LH and RH, nonprintable chars
    texto.erase(
        std::remove_if(texto.begin(), texto.end(),
                       [](unsigned char c)
                       { return !std::isprint(c); }),
        texto.end());
}
// Function to in-place trim all spaces in the
// string such that all words should contain only
// a single space between them.
void removeSpaces(string &str)
{
    // n is length of the original string
    int n = str.length();

    // i points to next position to be filled in
    // output string/ j points to next character
    // in the original string
    int i = 0, j = -1;

    // flag that sets to true is space is found
    bool spaceFound = false;

    // Handles leading spaces
    while (++j < n && str[j] == ' ')
        ;

    // read all characters of original string
    while (j < n)
    {
        // if current characters is non-space
        if (str[j] != ' ')
        {
            // remove preceding spaces before dot,
            // comma & question mark
            if ((str[j] == '.' || str[j] == ',' ||
                 str[j] == '?') &&
                i - 1 >= 0 &&
                str[i - 1] == ' ')
                str[i - 1] = str[j++];

            else
                // copy current character at index i
                // and increment both i and j
                str[i++] = str[j++];

            // set space flag to false when any
            // non-space character is found
            spaceFound = false;
        }
        // if current character is a space
        else if (str[j++] == ' ')
        {
            // If space is encountered for the first
            // time after a word, put one space in the
            // output and set space flag to true
            if (!spaceFound)
            {
                str[i++] = ' ';
                spaceFound = true;
            }
        }
    }

    // Remove trailing spaces
    if (i <= 1)
        str.erase(str.begin() + i, str.end());
    else
        str.erase(str.begin() + i - 1, str.end());
} // void removeSpaces(string &str)

bool isNumber(const string &texto)
{
    // This function checks if the string is a number
    if (texto.empty())
        return false;
    size_t i = 0;
    // Permitir signo negativo
    // if (texto[0] == '-') i = 1; //No aplicable

    for (i = 0; i < texto.size(); ++i)
    {
        if (!std::isdigit(texto[i]))
        {
            return false;
        }
    }
    return true;
} // bool isNumber(const string& texto)

// bool isNumber(const string& texto)
// {
//     // This function checks if the string is a number
//     if (texto.empty()) return false;
//     //size_t i = 0;
//     // Permitir signo negativo
//     //if (texto[0] == '-') i = 1; //No aplicable

//     for (size_t i=0; i < texto.size(); ++i) {
//         if (!std::isdigit(texto[i])) {
//             return false;
//         }
//     }
//     return true;
// }

int secureConvert(const string &texto)
{
    if (isNumber(texto))
    {
        return stoi(texto);
    }
    else
    {
        // throw std::invalid_argument("El string no es numérico");
        return 0;
    }
}

void checkForHistory(string pageName)
{

    /*
    This function will:
    1º.- To set global <<historyPages>> "true" when no ARROW's keys in the Page and "false" when there is any.
    2º.- Also will store the MCDU Page into <<historyPages>> vector <string> if historyPages is set to true.
    3º.- Reset the vector <string> when the MCDU Page is a root directory as:
    -MCDU MENU
    -MAINTENANCE MENU
    -ACMS
    -CMS
    -SAT
    -ATSU
    4º.- The data to check will be:
    - argument <<pageName>> of: getPage(string pageName) to check for root page
    - vector <string> "pageCmds" for ARROW's
    5º.- As a safety, if vector <string> historyPages is empty and a $RETURN try to get and stored page, the function will provide
     "FIRSTPAGE".
    */
    // Note if returnPage == true, the lastPage will not be stored on history pages. Instead will be removed from historyPages,
    // last position
    historyPages.push_back(pageName); // Store last page into the historyPages
}

string getLastPage(void) // Function called by $RETURN
{
    string mcduPage;

    if (historyPages.empty())
    {
        historyPages.push_back("FIRSTPAGE");
        return "FIRSTPAGE";
    }
    else
    {
        mcduPage = historyPages.back(); // Get last page
        historyPages.pop_back();        // Remove last page
        return mcduPage;
    }
}
