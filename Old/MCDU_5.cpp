// MCDU_4.cpp
//  C++ Program to implement drawPage() and drawLine():
//
//   fileTools.cpp + fileTools.h
//   screenTools.cpp + screenTools.h
//   ncurses.lib
//   hid library
// Check  all the keys

#include "fileTools.h" // Adjusted path to match the correct directory structure
#include "screenTools.h"
#include "hidTools.h"
#include <ncursesw/curses.h>
#include <locale.h>
#include <hidapi/hidapi.h>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string.h>

#define ESC 27
#define CLEAR '0'
#define MENU '1'
#define MCDU '2'
#define FMGC '3'
#define FAIL '4'
#define FM1 '5'
#define IND '6'
#define RDY '7'
#define FM2 '8'

//HID
//extern hid_device *handle;

//Path for initial MCDU page

//std::string filePath = "/home/avionica/Projects/MCDU/MCDU_Files/MCDUPAGE.TXT";
std::string filePath = "/home/jvidal/Projects/MCDU/MCDU_Files/MCDUPAGE.TXT"; // Path to the file

void exitProgram(string str);


// Screen
char mesg[] = "MCDU"; /* message to be appeared on the screen */
void testScreenTools(void);
int init_hid(void);
void exit_hid(void);
void manage_hid(void);

/////////////

int main()
{
    
    /////////////////////// FILES MANAGEMENT //////////////////
    cout << "Press any key to start MCDU\n"
         << endl;
    waitKeyPressed();


    int result = getMCDUPages(filePath); // Gets all MCDU pages needed, file: MCDUPAGE.TXT
    //int result = getMCDUPages("MCDUPAGE.TXT"); // Gets all MCDU pages needed, file: MCDUPAGE.TXT

    if (result == -1)                   // and stores into "allPages" <string> vector
    {
        // Exit
        cout << "\n getMCDUPages failed\n Press any key to Exit !!!" << endl;
           
        waitKeyPressed();
        exit(-1);
    }


    
    system("clear");
    cout << "\nMCDU pages OK\n Press any Key\n to init HID" << endl;
    waitKeyPressed();

    //init_hid();
    initMCDU(); // Init ncurses, mcdu window and colors
    mcduTest(); // Test mcdu window
     waitKeyPressed();

        //MCDU FIRSTPAGE
         getPage("FIRSTPAGE");
         drawPage();
         waitKeyPressed();
         clear(); // ncurses clear mcdu window // ncurses clear

        //MCDU ACMSHELP.TXT

        getPage("MAINMENU");
        drawPage();
        waitKeyPressed();
        clear(); // ncurses clear mcdu window                               // ncurses clear

        //MCDU RADNAV
        getPage("RADNAV");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear
        refresh();

        // MCDU MISC
        getPage("MISC");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear
        //refresh();

        // MCDU BACKUPNAV
        getPage("BACKUPNAV");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear
        //refresh();

        //MCDU BACKUPNAV
        getPage("BKNAVPROG");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear
        // refresh();

        // MCDU BKNAVDIR
        getPage("BKNAVDIR");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear

        // MCDU FMSXLOAD
        getPage("FMSXLOAD1");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear
        //refresh();

        // MCDU FMSXLOAD
        getPage("FMSXLOAD2");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear

        // MCDU FMSXLOAD
        getPage("FMSXLOAD3");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear

        // MCDU FMSXLOAD
        getPage("FMSXLOAD4");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear

        // MCDU FMSXLOAD
        getPage("FMSXLOAD5");
        drawPage();
        waitKeyPressed();
        clear; // ncurses clear mcdu window // ncurses clear

        // MCDU CMSPAGE1A
        getPage("CMSPAGE1A");
    drawPage();
    waitKeyPressed();
    clear(); // ncurses clear mcdu window // ncurses clear
    //refresh(); 

    //actions on standard screen
    mvprintw(max_y - 1, 0, "Press any key to manage HID!!!          ");
    refresh();

    waitKeyPressed();
   
    //manage_hid();
    exitProgram("Normal Exit");

    return 0;
} // Main



int init_hid()
{
    system("clear");
    printf("\nInit HID\n");
    // HID
    hid_init();

    handle = hid_open(0x0000, 0x0013, NULL); // Use your correct VID/PID
    if (!handle)
    {
        printf("Unable to open HIDdevice\n");
        sleep(5); // Wait for 5 seconds
        return FALSE;
    }

    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1); //
    printf("\nHID init OK\nPress any key\n");
    waitKeyPressed();
    sleep(2); // Wait for 5 seconds
  
    return TRUE;
}

void exit_hid()
{
    // Normal exit
    //  Close the device
    hid_close(handle); // Always close the device before exiting

    // Finalize the hidapi library
    hid_exit(); // This finalizes and frees resources

    printf("\n Exiting HID program\nPress any key");
    waitKeyPressed();
    //return TRUE; // Ensure no access to handle or buf hereafter
}

void exitProgram(string str)
{
    endwin(); // End curses mode
    exit_hid();
    system("clear");
    cout << endl  << str << endl;
    cout << "Press any to MCDU Program exit\n\n";
    waitKeyPressed();
    exit(0);
}

void manage_hid()
{
    
        unsigned char buf[65] = {0}; // Make sure this is big enough: 1 for Report ID + 64 for data
        unsigned char cmd[65] = {0}; // HID report ID is often 0
        int res;

        int key = 0;

        clear(); // ncurses clear std screen
        mvprintw(0, 0, "Program Init OK");

        mvprintw(1, 0, "while(loop)");
        refresh();
        while (key != ESC)

        {
            res = hid_read(handle, buf, sizeof(buf));

            if (res > 0)
            {
                mvprintw(3, 0, "Received bytes: %d", res);
                mvprintw(4, 0, "MCDU Key: %d", buf[0]);
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

          
}
