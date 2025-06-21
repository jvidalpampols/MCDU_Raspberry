// MCDU_1.cpp
//  C++ Program to implement drawPage() and drawLine():
//
//   fileTools.cpp + fileTools.h
//   screenTools_1.cpp + screenTools.h
//   ncurses.lib
//   hid library
// Checking  all MCDU keys
// Cheking some MCDU Pages
// drawLine() Label is shonw as  NO BOLD, Data is shown as BOLD
// MODE KEYS OK

#include "../include/fileTools.h"   // Adjust the path based on the actual location of fileTools.h
#include "../include/screenTools.h" // Adjust the path based on the actual location of screenTools.h
#include "../include/hidTools.h"    // Adjust the path based on the actual location of hidTools.h
#include "../include/mcdu.h"        // Adjust the path based on the actual location of mcdu.h
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <hidapi/hidapi.h>
#include <fstream>
#include <string>

// Declare extern variables
extern string actualPage;
extern bool testPage;
extern string strTestPage;
// Path for initial MCDU page
string filePath = "/mnt/mcdu/MCDU_Files/MCDUPAGE.TXT";
bool mcduFlightMode = false;

void testModeKeys(void);
void runTimeCmds(void);   // Get LSK, ARROW commands from  the actual MCDU  page, and ModeKeys
void testMCDUPages(void); // Test MCDU some pages
void gettingCmds(void);   // Get LSK, ARROW commands from  the actual MCDU  page
void testHid(void);       // Test HID device and MCDU keys
void runTimeModeKeys();   // Checking Mode Keys
void getCmdLSK1L(void);   // Get LSK1L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK2L(void);   // Get LSK2L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK3L(void);   // Get LSK3L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK4L(void);   // Get LSK4L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK5L(void);   // Get LSK5L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK6L(void);   // Get LSK6L MCDU Page or $Special command pageCmds <string> vector

void getCmdLSK1R(void); // Get LSK1L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK2R(void); // Get LSK2L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK3R(void); // Get LSK3L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK4R(void); // Get LSK4L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK5R(void); // Get LSK5L MCDU Page or $Special command pageCmds <string> vector
void getCmdLSK6R(void); // Get LSK6L MCDU Page or $Special command pageCmds <string> vector

void getCmdARROWLEFT(void);            // Get ARROWLEFT MCDU Page or $Special command pageCmds <string> vector
void getCmdARROWRIGHT(void);           // Get ARROWLEFT MCDU Page or $Special command pageCmds <string> vector
void getCmdARROWUP(void);              // Get ARROWLEFT MCDU Page or $Special command pageCmds <string> vector
void getCmdARROWDOWN(void);            // Get ARROWLEFT MCDU Page or $Special command pageCmds <string> vector
bool checkSpecialFunction(string str); // Check for special function $
bool checkReturn(string str);          // Check for special function $RETURN
bool checkSpecialCmds(string str);
void mcduDebugging(void); // Program to test all data and variables managed
void drawMcduData(void);  // Program to show MCDU data for mcduDebugging
void createFile_MCDU_Data(void);
void resetVars(void);
void reload(void);
bool checkInFlight(string str);
string getLastPage(void);
bool returnPage = false;  //So as not to store the new page, because is already on historyPages and should be removed instead

int main()
{

    /////////////////////// GETTING FILES //////////////////
    // cout << "Press any key to start MCDU\n"
    //      << endl;
    // waitKeyPressed();
    int result = getMCDUPages(filePath); // Gets all MCDU pages needed, file: MCDUPAGE.TXT
    if (result == -1)                    // and stores into "allPages" <string> vector
    {
        // Exit
        cout << "\n getMCDUPages failed\n Press any key to Exit !!!" << endl;

        
    } // if(result)
    
    

    
    createFile_allPages(); // Creates a text file allPages.txt to check allPages[] storage

    /////////////////////// GETTING FILES END //////////////////

    system("clear");
    //cout << "\nMCDU pages OK\n Press any Key\n to following Tests" << endl;
    // waitKeyPressed();


    ///////////////// MCDU TESTING START   ////////////////////////////
    init_hid(); // Init HID device
  
    initMCDU(); // Init ncurses, mcdu window and colors

    getPage("FIRSTPAGE");
    //getPage("LABELCALL1"); //Testing
    drawPage();
    // getchar();
    //  mcduTimer(10); //Test

  

    runTimeCmds();
    // Testing:
    // getchar();
    // waitKeyPressed();

    ///////////////// TESTING END   ////////////////////////////

    //endwin(); // End curses mode
    exitProgram("Normal Exit");
    //getchar();

    return 0;
} // Main

void mcduDebugging(void)
{
    // getPage("FIRSTPAGE");
    getPage("MYTEST");
    // cout << "FIRSTPAGE OK" << endl;
    // cout << "MYTEST OK" << endl;
    getchar();
    // getPage("MAINMENU"); // getLSK(); //Is included in getPage()
    // cout << "MAINMENU OK" << endl;
    getchar();
    drawMcduData();
    exitProgram("Normal exit");
}

void drawMcduData(void)
{
    //DEBUGGING  Function
    system("clear");
    cout << "MCDU Data\n";
    getchar();
    getchar();
    cout << "pageLines[i]" << "\t" << "drawPageCmds[i]\n";
    for (size_t i = 0; i < pageLines.size(); i++)
    {
        cout << "pageLine[" << i << "]: " << pageLines[i] << "\n";
    }

    for (size_t j = 0; j < pageCmds.size(); j++)
    {
        cout << strCmdsIndex[j] << ": " << pageCmds[j] << "\n";
    }

    // Variable Globals
    cout << "VAR1:" << VAR1 << endl;
    cout << "VAR2:" << VAR2 << endl;
    cout << "VAR3:" << VAR3 << endl;
    cout << "VAR4:" << VAR4 << endl;
    cout << "VARPAGE:" << VARPAGE << endl;
    cout << "VARPAGE1:" << VARPAGE1 << endl;
    cout << "VARPAGE2:" << VARPAGE2 << endl;
    cout << "VARPAGE3:" << VARPAGE3 << endl;
    cout << "VARPAGE4:" << VARPAGE4 << endl;
    getchar();
    waitKeyPressed();
}

void resetVars(void)
{
    VAR1 = "";
    VAR2 = "";
    VAR3 = "";
    VAR4 = "";
    VARPAGE = "";
    VARPAGE1 = "";
    VARPAGE2 = "";
    VARPAGE3 = "";
    VARPAGE4 = "";
}

void exitProgram(string str)
{
    /*
    The exitProgram function terminates the program by performing several cleanup operations,
    including ending curses mode, shutting down the HID device, and clearing the terminal screen.
    It then displays a message, waits for a key press, and exits the program with a status code of 0.
    */
    endwin();   // End curses mode
    exit_hid(); // End HID Device
    sleep(1);
    system("clear");
    cout << endl
         << str << endl;
    cout << "\n Wait 2 seconds\n\n";
    sleep(2);
    // waitKeyPressed();
    exit(0);
}

void testHid(void)
// Test to be done on MCDU
// This function checks the HID device
// and the MCDU keys
// It uses ncurses to show the results
// It uses the HIDAPI library to communicate with the MCDU
// hid_write commands to switch MCDU LED's ON/OFF
// Checks all the MCDU keys codes sent by hid_read() function
{
    // Moved to hidTools.h, hid global buffers
    //  unsigned char buf[65] = {0}; // Make sure this is big enough: 1 for Report ID + 64 for data
    //  unsigned char cmd[65] = {0}; // HID report ID is often 0
    int res;

    int key = 0;
    int mcdu_key = 0;

    clear(); // ncurses clear std screen
    mvprintw(0, 0, "Program Init OK");

    // reset MCDU lights
    cmd[1] = 0;
    hid_write(handle, cmd, sizeof(cmd));

    mvprintw(1, 0, "while(loop)");
    refresh();

    // MCDU light RDY ON
    cmd[1] = 64;
    hid_write(handle, cmd, sizeof(cmd));
    refresh();

    while (mcdu_key != MCDU_KEY_OVFY)

    {
        res = hid_read(handle, buf, sizeof(buf));

        if (res > 0)
        {
            mcdu_key = buf[0];
            mvprintw(3, 0, "Received bytes: %d  ", res);
            mvprintw(4, 0, "MCDU Key DEC: %d  ", mcdu_key);
            mvprintw(5, 0, "MCDU Key HEX: 0x%X  ", mcdu_key);
            refresh();
        }

        if ((key = kbhit()) != 0) // Any to be read??
        {
            key = getch(); // Get key from keyboard buffer
            mvprintw(LINES / 2, 0, "kbhit: %d", key);
            refresh();
        }

        if (key >= '0' && key <= '8') // Check if key is between '0' and '6'
        {
            switch (key)
            {
            case CLEAR:
            {
                cmd[1] = 0;
                hid_write(handle, cmd, sizeof(cmd));

                cmd[2] = 0;
                hid_write(handle, cmd, sizeof(cmd));

                mvprintw(LINES / 2, (COLS / 2), "Clear all");
                refresh();
                break;
            }

            case MENU:
            {
                cmd[1] = 1;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "MENU   ");
                refresh();
                break;
            }

            case MCDU:
            {
                cmd[1] = 2;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "MCDU   ");
                refresh();
                break;
            }

            case FMGC:
            {
                cmd[1] = 4;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "FMGC   ");
                refresh();
                break;
            }

            case FAIL:
            {
                cmd[1] = 8;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "FAIL   ");
                refresh();
                break;
            }

            case FM1:
            {
                cmd[1] = 16;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "FM1   ");
                refresh();
                break;
            }

            case IND:
            {
                cmd[1] = 32;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "IND   ");
                refresh();
                break;
            }
            case RDY:
            {
                cmd[1] = 64;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "RDY   ");
                refresh();
                break;
            }
            case FM2:
            {
                cmd[2] = 1;
                hid_write(handle, cmd, sizeof(cmd));
                mvprintw(LINES / 2, (COLS / 2), "FM2   ");
                refresh();
                break;
            }

            default:
                break;
            } // switch
              // default statements
        } // if
    } // while

    // Turn MCDU lights OFF

    cmd[1] = 0;
    hid_write(handle, cmd, sizeof(cmd));

    cmd[2] = 0;
    hid_write(handle, cmd, sizeof(cmd));
} // testHid

void testModeKeys()
// Test to be done on MCDU
//  Test all MCDU MODE keys
//  #define DIR 0x4F
//  #define PROG 0x4E
//  #define PERF 0x4D
//  #define INIT 0x4C
//  #define DATA 0x4B
//  #define FPLN 0x47
//  #define RAD_NAV 0x46
//  #define FUEL_PRED 0x45
//  #define SEC_PLN 0x44
//  #define ATC_COMM 0x43
//  #define MCDU_MENU 0x42
//  #define AIRPORT 0x41

{
    // Moved to hidTools.h, hid global buffers
    //  unsigned char buf[65] = {0}; // Make sure this is big enough: 1 for Report ID + 64 for data
    //  unsigned char cmd[65] = {0}; // HID report ID is often 0

    clear(); // ncurses clear std screen
    wclear(mcdu);

    // reset MCDU lights
    cmd[1] = 0;
    hid_write(handle, cmd, sizeof(cmd));

    // MCDU light RDY ON
    cmd[1] = 64;
    hid_write(handle, cmd, sizeof(cmd));
    refresh();

    // get MAIN MENU PAGE
    getPage("MAINMENU");
    drawPage();
    runTimeModeKeys();

    // Turn MCDU lights OFF

    cmd[1] = 0;
    hid_write(handle, cmd, sizeof(cmd));

    cmd[2] = 0;
    hid_write(handle, cmd, sizeof(cmd));
} // TestModeKeys

void runTimeModeKeys() // Checking Mode Keys
{
    int res;
    int mcdu_key = 0;
    getchar();
    // reset MCDU lights
    cmd[1] = 0;
    hid_write(handle, cmd, sizeof(cmd));
    getchar();

    while (mcdu_key != MCDU_KEY_OVFY)
    {
        res = hid_read(handle, buf, sizeof(buf));

        if (res > 0)
        {
            mcdu_key = buf[0];

            switch (mcdu_key)
            {
            case DIR:
            {
                wclear(mcdu);
                getPage("DIRFLTPLANA");
                drawPage();
                break;
            }

            case PROG:
            {
                wclear(mcdu);
                getPage("PROGPAGES");
                drawPage();
                break;
            }

            case PERF:
            {
                wclear(mcdu);
                getPage("PERFTKOFF");
                drawPage();
                break;
            }

            case INIT:
            {
                wclear(mcdu);
                getPage("FMSINITA");
                drawPage();
                break;
            }

            case DATA:
            {
                wclear(mcdu);
                getPage("FMSDATA1");
                drawPage();
                break;
            }

            case FPLN:
            {
                wclear(mcdu);
                getPage("FLTPLANA");
                drawPage();
                break;
            }

            case RAD_NAV:
            {
                wclear(mcdu);
                getPage("RADNAV");
                drawPage();
                break;
            }
            case FUEL_PRED:
            {
                wclear(mcdu);
                getPage("FUELPRED");
                drawPage();
                break;
            }
            case SEC_PLN:
            {
                wclear(mcdu);
                getPage("SECNAVPLN");
                drawPage();
                break;
            }
            case ATC_COMM:
            {
                wclear(mcdu);
                getPage("ATCMAIN");
                drawPage();
                break;
            }
            case MCDU_MENU:
            {
                wclear(mcdu);
                getPage("MAINMENU");
                drawPage();
                break;
            }
            case AIRPORT:
            {
                wclear(mcdu);
                getPage("AIRPORTKEY");
                drawPage();
                break;
            }

            default:
                break;
            } // switch
              // default statements
        } // if res >0

    } // while
} // runTimeModeKeys()

void runTimeCmds() // Get LSK, ARROW commands from  the actual MCDU  page
{
    int res;
    int mcdu_key = 0;

    // writePADC("   Now in Run Time   ", mcdu);
    // wrefresh(mcdu);

    while (mcdu_key != MCDU_KEY_OVFY)
    {
        if (testPage)
        {
            mcduTimer(10);
        }

        res = hid_read(handle, buf, sizeof(buf));

        if (res > 0) // key from hid
        {
            mcdu_key = buf[0];

            switch (mcdu_key) // switch keys
            {
            case DIR:
            {
                wclear(mcdu);
                getPage("DIRFLTPLANA");
                drawPage();
                break;
            }

            case PROG:
            {
                wclear(mcdu);
                getPage("PROGPAGES");
                drawPage();
                break;
            }

            case PERF:
            {
                wclear(mcdu);
                getPage("PERFTKOFF");
                drawPage();
                break;
            }

            case INIT:
            {
                wclear(mcdu);
                getPage("FMSINITA");
                drawPage();
                break;
            }

            case DATA:
            {
                wclear(mcdu);
                getPage("FMSDATA1");
                drawPage();
                break;
            }

            case FPLN:
            {
                wclear(mcdu);
                getPage("FLTPLANA");
                drawPage();
                break;
            }

            case RAD_NAV:
            {
                wclear(mcdu);
                getPage("RADNAV");
                drawPage();
                break;
            }
            case FUEL_PRED:
            {
                wclear(mcdu);
                getPage("FUELPRED");
                drawPage();
                break;
            }
            case SEC_PLN:
            {
                wclear(mcdu);
                getPage("SECNAVPLN");
                drawPage();
                break;
            }
            case ATC_COMM:
            {
                wclear(mcdu);
                getPage("ATCMAIN");
                drawPage();
                break;
            }
            case MCDU_MENU:
            {
                wclear(mcdu);
                getPage("MAINMENU");
                drawPage();
                break;
            }
            case AIRPORT:
            {
                wclear(mcdu);
                getPage("AIRPORTKEY");
                drawPage();
                break;
            }

            case LSK_1L:
            {
                getCmdLSK1L();
                break;
            }

            case LSK_2L:
            {
                getCmdLSK2L();
                break;
            }
            case LSK_3L:
            {
                getCmdLSK3L();
                break;
            }
            case LSK_4L:
            {
                getCmdLSK4L();
                break;
            }
            case LSK_5L:
            {
                getCmdLSK5L();
                break;
            }
            case LSK_6L:
            {
                getCmdLSK6L();
                break;
            }

            case LSK_1R:
            {
                getCmdLSK1R();
                break;
            }

            case LSK_2R:
            {
                getCmdLSK2R();
                break;
            }
            case LSK_3R:
            {
                getCmdLSK3R();
                break;
            }
            case LSK_4R:
            {
                getCmdLSK4R();
                break;
            }
            case LSK_5R:
            {
                getCmdLSK5R();
                break;
            }
            case LSK_6R:
            {
                getCmdLSK6R();
                break;
            }
            case ARROW_LEFT:
            {
                getCmdARROWLEFT();
                break;
            }

            case ARROW_RIGHT:
            {
                getCmdARROWRIGHT();
                break;
            }
            case ARROW_UP:
            {
                getCmdARROWUP();
                break;
            }
            case ARROW_DOWN:
            {
                getCmdARROWDOWN();
                break;
            }

            default:
                break;
            } // switch (mcdu_key) //switch keys
              // default statements
        } // if (res > 0) //key from hid

    } // while
} // runTimeCmds

void getCmdLSK1L(void)
/*
The getCmdLSK1L function retrieves a command string from the pageCmds vector using the LSK1L index,
checks if it corresponds to a special command using checkSpecialCmds, and if not,
processes the command by calling getPage and drawPage.
It is designed to handle commands associated with the LSK1L enum value.
*/
{
    string strCmd;
    // strCmd = pageCmds[LSK1L]; // Get index from enum LSK1L for index to pageCmds <string> vector
    strCmd = pageCmds.at(LSK1L); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK1L];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();

        // Set VAR(X) and VARPAGEX for next page.
        // CABPRESS is linked to LSK1L and will be drawn.
        // On the MCDU page drawn, when selected:
        // LSK1L will be linked to CPCTEST, but VAR(1)  and VARPAGE1 will be updated to new values
        // LSK1R will be linked to CPCTEST, but VAR(1)  and VARPAGE1 will be updated to new values
        /*
        PAGE=CABPRESS
        TLL= SYSTEM REPORT/TEST 1/1
        1CB=AIRCOND,SMALLFONT
        1LD=<CPC 1
        1RD=CPC 2>
        2CB=CAB PRESS CTL UNIT,SMALLFONT
        6LD=<RETURN,CYAN
        LSK6L=SYSTEST1
        LSK1L=CPCTEST, VAR(1)=CPC 1, VARPAGE1=TESTCPC4
        LSK1R=CPCTEST, VAR(1)=CPC 2, VARPAGE1=TESTCPC6
        ENDPAGE
        */
    }

} // void getCmdLSK1L(void)

void getCmdLSK2L(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK2L); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK2L];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK2L(void)

void getCmdLSK3L(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK3L); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string lskLine = strLineLSK[LSK3L];
        checkForVars(lskLine);
        getPage(strCmd);
        drawPage();

        // writePADL(lskLine, mcdu);
        // wrefresh(mcdu);
        // sleep(5);
    }

} // void getCmdLSK3L(void)

void getCmdLSK4L(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK4L); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string lskLine = strLineLSK[LSK4L];
        checkForVars(lskLine);
        getPage(strCmd);
        drawPage();

        // writePADL(lskLine, mcdu);
        // wrefresh(mcdu);
        // sleep(5);
    }

} // void getCmdLSK4L(void)

void getCmdLSK5L(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK5L); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string lskLine = strLineLSK[LSK5L];
        checkForVars(lskLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK5L(void)

void getCmdLSK6L(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK6L); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK6L];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK6L(void)

void getCmdLSK1R(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK1R); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK1R];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK1R(void)

void getCmdLSK2R(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK2R); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK2R];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK2R(void)

void getCmdLSK3R(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK3R); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK3R];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK3R(void)

void getCmdLSK4R(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK4R); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK4R];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK4R(void)

void getCmdLSK5R(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK5R); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK5R];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK5R(void)

void getCmdLSK6R(void)
{
    string strCmd;
    strCmd = pageCmds.at(LSK6R); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        string strLine = strLineLSK[LSK6R];
        checkForVars(strLine);
        getPage(strCmd);
        drawPage();
    }

} // void getCmdLSK6R(void)

void getCmdARROWLEFT(void)

/*
The getCmdARROWLEFT function retrieves a command string from the pageCmds vector using the ARROWLH index,
checks if it corresponds to a special command using checkSpecialCmds, and if not,
processes the command by calling getPage and drawPage.
It is designed to handle commands associated with the ARROWLEFT enum value.
*/
{
    string strCmd;
    strCmd = pageCmds.at(ARROWLH); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        getPage(strCmd);
        drawPage();
    }
} // void getCmdARROWLEFT()(void)

void getCmdARROWRIGHT(void)
{
    string strCmd;
    strCmd = pageCmds.at(ARROWRH); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        getPage(strCmd);
        drawPage();
    }

} // void getCmdARROWRIGHT()(void)

void getCmdARROWUP(void)
{
    string strCmd;
    strCmd = pageCmds.at(ARROWUP); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        getPage(strCmd);
        drawPage();
    }

} // void getCmdARROWUP()(void)

void getCmdARROWDOWN(void)
{
    string strCmd;
    strCmd = pageCmds.at(ARROWDN); // Get index from enum LSK1L for index to pageCmds <string> vector

    if (strCmd == "") // To avoid program exit when no LSK command assigned
        return;

    if (!checkSpecialCmds(strCmd)) // Check for special function $XXXXXXX
    {
        getPage(strCmd);
        drawPage();
    }

} // void getCmdARROWDN()(void)

bool checkSpecialCmds(string strTest)
{
    // check for special function $
    if (checkSpecialFunction(strTest)) // Check for special function $XXXXXXXXXX
    {
        if (strTest == "$RELOAD") // Check for $RETURN
        {
            reload();
            return true;
        }

        if (strTest == "$RETURN") // Check for $RETURN
        {
            string strLastPage = getLastPage();
            returnPage = true;
            getPage(strLastPage);
            returnPage = false;
            drawPage();
            return true;
        }
        else if (strTest == "$VARPAGE")
        {
            getPage(VARPAGE);
            drawPage();
            return true;
        }
        else if (strTest == "$VARPAGE1")
        {
            getPage(VARPAGE1);
            drawPage();
            return true;
        }
        else if (strTest == "$VARPAGE2")
        {
            getPage(VARPAGE2);
            drawPage();
            return true;
        }
        else if (strTest == "$VARPAGE3")
        {
            getPage(VARPAGE3);
            drawPage();
            return true;
        }
        else if (strTest == "$VARPAGE4")
        {
            getPage(VARPAGE4);
            drawPage();
            return true;
        }
        else if (strTest == "$LASTSELC")
        {
            getPage(lastPage);
            drawPage();
            return true;
        }

        else if ((strTest == "$QUITPROGRAM"))
        {
            exitProgram("Normal Exit");
            return true;
        }
        
        else if ((strTest == "$FMSPAGE"))
        {
            getPage("FIRSTPAGE"); //Should call lasT FMS page or "FIRSTPAGE". To be implemented
            drawPage();
            return true;
        }


        else
            ; // return false;
    } // if $ (checkSpecialFunction($strTest));

    // Check Special Cmds not starting with $
    if ((strTest == "QUITPROGRAM"))
    {
        exitProgram("Normal Exit");
        return true;
    }

    if ((strTest == "INFLIGHT")) // Check the mode *GND MODE/*FLT MODE on MISC MCDU PAGE
    {
        return checkInFlight(strTest);
        // return true;
    }

    return false;
} // checkSpecialCmds(string strTest)

bool checkInFlight(string strTest)
{
    // This function toggles the *GND MODE /*FLT MODE on MISC page and calls to getPage with the
    //  selected page. See getPage for full management of INFLIGHT
    if (strTest == "INFLIGHT")
    {
        if (mcduFlightMode == false)
        {
            mcduFlightMode = true;
            getPage("MISC_FLT");
            drawPage();
            return true;
        }
        if (mcduFlightMode == true)
        {
            mcduFlightMode = false;
            getPage("MISC_GND");
            drawPage();
            return true;
        }
    } // if (strTest == "INFLIGHT")
    return false;
} // bool checkInFlight(string str)

bool checkSpecialFunction(string str) // Check for special function $
{
    // This function checks for special function $ in string
    char target = '$';
    string strTarget = "INFLIGHT";

    if (str.find(target) != string::npos)
    {
        return true;
    }
    // else if(str.find(strTarget) != string::npos)
    // {
    //     return true;
    // }
    else
    {
        return false;
    }
} // bool checkSpecialFunction(string str)

bool checkReturn(string str) // Not used. checkSpecialFunction(string str) does it
{
    // This function checks for special function $RETURN
    // and gets the lastPage name to show it on MCDU
    string strTest = "$RETURN";

    if (str.find(strTest) != string::npos)
        return true;
    else
        return false;

} // void checkReturn(string str)

//////////////////// Testing Functions ///////////////////
void testMCDUPages()
{
    // Test MCDU writting functions
    //  mcduTest();
    //  waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU CMSPAGE1A
    getPage("CMSPAGE1A");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU PERFORMANCEPAGE
    getPage("PERFTKOFF");
    drawPage();
    waitKeyPressed();
    wclear(mcdu);

    // MCDU FIRSTPAGE
    getPage("FIRSTPAGE");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU ACMSHELP.TXT

    getPage("MAINMENU");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window, ncurses clear function

    // MCDU RADNAV
    getPage("RADNAV");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU MISC
    getPage("MISC");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU BACKUPNAV
    getPage("BACKUPNAV");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU BACKUPNAV
    getPage("BKNAVPROG");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU BKNAVDIR
    getPage("BKNAVDIR");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU FMSXLOAD
    getPage("FMSXLOAD1");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU FMSXLOAD
    getPage("FMSXLOAD2");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU FMSXLOAD
    getPage("FMSXLOAD3");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU FMSXLOAD
    getPage("FMSXLOAD4");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU FMSXLOAD
    getPage("FMSXLOAD5");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    // MCDU PERFORMANCEPAGEx
    getPage("PERFTKOFF");
    drawPage();
    waitKeyPressed();
    wclear(mcdu);

    // return 0;

    // actions on standard screen
    mvprintw(max_y - 1, 0, "Press any key to manage HID!!!          ");
    wrefresh(mcdu);

    waitKeyPressed();

    // testHid();
    testModeKeys();
}

void gettingCmds()
{
    //DEBUGGING  Function
    // This function gets the LSK's and ARROW's command from a string
    // The LSK's and ARROW's commands are stored into pageCmds <string> vector
    // The command string LSK's or ARROW's are used to string comparation with strCmdsIndex <string> vector
    // so as to get the index on which the command is stored into strCmdsIndex <string> vector
    int index;
    /////////////////  Testing RUN TIME GETTING COMMANDS START /////////////
    // Get LSK command from string
    getPage("CMSPAGE1A");

    // get LSK and ARROW Commands from MCDU Page "CMSPAGE1A";
    // getLSK(); // Get LSK, ARROW commands from  the actual MCDU  pageLine[]

    // Show all pageLines from, MCDU page "CMSPAGE1A"
    cout << "\n\nMCDU page \"CMSPAGE1A\" lines: " << endl;
    for (string pageLines : pageLines)
    {
        cout << pageLines << "\n";
    }

    cout << "\n\nMCDU page \"CMSPAGE1A\" Commands:" << strCmdsIndex.size() - 1 << endl;
    for (string strCmdsIndex : strCmdsIndex)
    {
        cout << strCmdsIndex << "\n";
    }

    waitKeyPressed();

    initMCDU(); // Init ncurses, mcdu window and colors

    // MCDU CMSPAGE1A
    getPage("CMSPAGE1A");
    drawPage();
    waitKeyPressed();
    wclear(mcdu); // ncurses clear mcdu window // ncurses clear

    for (size_t i = 0; i < strCmdsIndex.size(); i++)
    {
        cout << strCmdsIndex[i] << " = " << pageCmds[i] << endl;
    }

    cout << "Press any key to continue" << endl;
    waitKeyPressed();
    getchar();

    // Example of LSK1L command runTime
    index = getStrCmdsIndex("LSK1L"); // This function gets the index of LSK's command from string

    cout << "LSK1L index:" << index << endl;
    getPage(pageCmds[index]);
    drawPage();
    waitKeyPressed();

    // Example of LSK2L command runTime
    index = getStrCmdsIndex("LSK2L"); // This function gets the index of LSK's command from string
    getPage(pageCmds[index]);
    drawPage();
    waitKeyPressed();

    // Example of LSK3L command runTime
    index = getStrCmdsIndex("LSK2L"); // This function gets the index of LSK's command from string
    getPage(pageCmds[index]);
    drawPage();
    waitKeyPressed();

    // Example of ARROWDN command runTime
    index = getStrCmdsIndex("ARROWDN"); // This function gets the index of LSK's command from string
    getPage(pageCmds[index]);
    drawPage();
    waitKeyPressed();

    // Example of ARROWUP command runTime
    index = getStrCmdsIndex("ARROWUP"); // This function gets the index of LSK's command from string
    getPage(pageCmds[index]);
    drawPage();
    waitKeyPressed();
    // return 0;

    //////////////////////////////  Testing RUN TIME GETTING COMMANDS END /////////
}

void reload(void)
{

    wclear(mcdu);
    wrefresh(mcdu);

    write3CD("RELOAD", mcdu);
    write5CB("WAIT", mcdu);
    wrefresh(mcdu);
    
    //sleep(2); // End curses mode

    int result = getMCDUPages(filePath); // Gets all MCDU pages needed, file: MCDUPAGE.TXT
    if (result == -1)                    // and stores into "allPages" <string> vector
    {
        // Exit
        exitProgram("Fail to getMCDUPages");
    }

    createFile_allPages(); // Creates a text file allPages.txt to check allPages[] storage

    getPage("FIRSTPAGE");  
    drawPage();
    runTimeCmds();
} //$Reload