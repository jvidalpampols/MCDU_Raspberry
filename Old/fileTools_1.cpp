//fileTools.cpp

#include <bits/stdc++.h>
#include <cassert>
#include <string>
#include <string_view>
#include <iostream>
#include <algorithm>
#include <cctype>
//#include <ncurses.h> /* ncurses.h includes stdio.h */
#include <ncursesw/ncurses.h>
#include "locale.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <wchar.h>

using namespace std;

// C++ tools to perform MCDU files handling
// Function declarations

int getMCDUPages(string strPath); // Opening the file name "MCDUPAGE.TXT"
                                  // and stores its content to allPages<string>vector.
void getLoadFile(string str);     // Get file name to be loaded into "loadFiles" string vector
bool loadFile(string fileName);   // Store contents of a file into allPages
bool getPage(string pageName); // getPage a "pageName" stored into allPages.


//Page formating Tools
void removeComments(vector<string> &pages);
void allCapitals(vector<string> &pages);
void removeEmptyLines(vector<string> &pages);
void removeSpaces(vector<string> &pages);
void formating(vector<string> pages);
void formatingAllpages();                     // remove Spaces(allPages); removeComments(allPages); allCapitals(allPages);
                                             // removeEmptyLines(allPages);
void trim(string &str);                      // Remove leading and trailing spaces from an string
void showVectorString(vector<string> pages); // Show on screem all items of vector <string> argument

void waitKeyPressed(void); // check if a key have been pressed
int kbhit(void);           // used on waitKeyPressed(void);
void exitProgram(string str); //Defined on MCDU_XX.cpp

// Page Global variables declaration

vector<string> allPages;  // To store all files data the program will manage
vector<string> loadFiles; // To store files names to be loaded
vector<string> pageLines; //To store lines of actual page
//string pageLines; // To store lines of actual page
// Path for all MCDU files
std::string loadFilePath = "/home/jvidal/Projects/MCDU/MCDU_Files/"; // Path for MCDU pages
                          
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
    // On each line before is loaded into "allPages", comments are removed and trimmed RH and LH

    //std::ifstream file(str, std::ios::in); // Declaring file handle and assigning to "filepath" MCDUPAGE.TXT"
    //fstream file("./MCDUPAGE.TXT");
    //std::ifstream file("MCDUPAGE.TXT", std::ios::in);
    ifstream file(str); // Declaring file handle and assigning to "filepath" MCDUPAGE.TXT"
    
    // Read lines from File, store them into vector string "allPages", until it reaches the end "EOF"

    if (file)
    {
        while (getline(file, line)) // Until EOF
        {
            // storing each line to string vector "all Pages"
            allPages.push_back(line);
        }

        file.close();
    }
    else
    {

        cout << "Fail openning the file:" << str << " \n Press any to exit!!\n"
             << endl
             << endl;
        getchar();
        return -1;
    }

    // Formating all Pages to better manage "LOADFILE=XXXXXXXX.TXT"
    //formating(allPages);
    formatingAllpages();

    // Get string names of MCDU Files,"LOADFILE=XXXXXXXX.TXT" from MCDUPAGE.TXT, and store into loadFile vector string

    for (size_t i = 0; i < allPages.size(); i++)
    {
        getLoadFile(allPages[i]); // Get file name to be loaded into "loadFiles" string vector
    }

    // Load the files stored into "loadFiles[]" vector string
    // and store their contents into "allPages[]" vector string
    // Note: the files are loaded into "allPages" vector string
    // and the contents of "allPages" vector string are formated
    // before they are loaded into "allPages" vector string
    

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

    // Formating all Pages
    //formating(allPages);
    formatingAllpages();

            cout
        << "After formating \"allPages\", show all lines in vector <string> allPages: " << endl
        << endl;
    showVectorString(allPages);

    waitKeyPressed();
    return 0;
} // getMCDUPages(string str)

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

void allCapitals(vector<string> &pages)
{
    // This function makes capital letters all strings stored into vector
    string strTemp;
    char ch;

    for (unsigned int i = 0; i < pages.size(); i++)
    {
        strTemp = pages[i];
        for (unsigned int j = 0; j < strTemp.size(); j++)
        {
            ch = toupper(strTemp[j]);
            strTemp[j] = ch;
        } // for strTemp
        pages[i] = strTemp;
    } // for pages

} // allCapitals

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

    // Opening the file
    ifstream newFile(pathString); // Declaring file handle and assigning to "filepath"

    if (newFile.is_open())
    {
        while (getline(newFile, line)) // Until EOF
        {
            // storing each line to string vector "all Pages"
            allPages.push_back(line);
            cout << line << endl;
        }

        newFile.close();
    }

    else
    {
        cout << "Fail openning the file:" << fileName << " \n Press any to exit!!\n"
             << endl
             << endl;
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
    //Test function
    // Show on screem all items of vector <string> argument
    //And stores into a text file file allPages.txt
    //To check the contents of allPages: formating, pages lost, etc

    //The ofstream class (short for "output file stream") is used to write into files.
    ofstream file("allPages.txt"); // Create the file
    if (!file)
    {
        std::cerr << "Error opening file! \"allPages.txt\"" << std::endl;
        //return (0); // Exit if file can't be opened
    }


    for (unsigned int i = 0; i < pages.size(); i++)
    {
        cout << pages[i] << endl;
        file << pages[i] << endl; // Print each line
    }

    cout << endl;
    // Close the file
    file.close();
    cout << "Vector String Size:" << pages.size() << endl
         << endl;
    waitKeyPressed();
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
}

int kbhit()
// Returns the keys on keyboard buffer.
// If no key have been pressed returns '0', else returns > 0
//Note: it does not return ASCII value of key pressed
{
    int k;


    ioctl(STDIN_FILENO, FIONREAD, &k);
    return (k);
}

void waitKeyPressed()
{
    flushinp(); // Flusing keyboard buffer
    while (1)
    {
       
        /* check for any key */
        if (kbhit())
        {
            getch();
            break;
        }
    }
}

bool getPage(string pageName)
{
    // into allPages[] vector<string>, find out a line that contains:
    // PAGE=PAGENAME.
    // Set indexStart
    // Next find out the first line that contains:
    // ENDPAGE.
    // Set indexEnd
    // Store the lines between indexes into new List: pagelines.

    string strTemp;
    string strCheckInit = "PAGE=" + pageName;
    string strCheckEnd = "ENDPAGE";

    // string strCheck = "PAGE=FIRSTPAGE";
    int indexStart = 0;
    int indexEnd = 0;
    size_t found;
    bool pageInit = false;
    bool pageFound = false;

    pageLines.clear(); // Clear pagelines to store a new MCDU PAGE
    // LskArrowCmdPageList.Clear();

    for (unsigned int i = 0; i < allPages.size(); i++)
    {

        found = allPages[i].find(strCheckInit);
        if (found != std::string::npos) // Si se encuentra "PAGE=" + pageName;
        {
            indexStart = i;
           //cout << "Encontrado inicio de pagina: " << allPages[i] << endl;
            pageInit = true;
        }

        if (pageInit == true)
        {
            found = allPages[i].find(strCheckEnd);
            if (found != std::string::npos) // Si se encuentra "ENDPAGE";
            {
                indexEnd = i;
                //cout << "Encontrado final de pagina: " << allPages[i] << endl;
                pageFound = true;
                break;
            }
        }
    }

    if (pageFound)
    {
        //cout << "Page Found: " << "PAGE=" + pageName << endl << endl;
        for (int i = 0; i <= (indexEnd - indexStart); i++)
        {
            pageLines.push_back(allPages[indexStart + i]);
            //cout << pageLines[i] << endl;
        }
    }
    else
    {
        //cout << "page NOT FOUND" << endl  << endl;
        exitProgram("Page not found:" + pageName);
        return false;
    }

    if (pageFound)
    {
        //cout << "\nShow pageLines stored: " << endl  << endl;
        // for (int i = 0; i < pageLines.size(); i++)
        // {
        //     cout << pageLines[i] << endl;
        // }
    }
    //cout << "\nPress any key to continue!!!\n";
    return true;

} // getPage


// Decoder files

