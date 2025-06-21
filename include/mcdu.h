// mcdu.h
//This header file is only for main MCDU program
// It contains the function prototypes and global variables
// needed for the MCDU program
// It is included in the main MCDU program
// It is neither included nor used in any other file


#ifndef MCDU_H
#define MCDU_H

// MCDU Lights
#define ESC 27 // Program exit
#define CLEAR '0'
#define MENU '1'
#define MCDU '2'
#define FMGC '3'
#define FAIL '4'
#define FM1 '5'
#define IND '6'
#define RDY '7'
#define FM2 '8'

// MCDU Keys
#define DIR 0x4F
#define PROG 0x4E
#define PERF 0x4D
#define INIT 0x4C
#define DATA 0x4B
#define FPLN 0x47
#define RAD_NAV 0x46
#define FUEL_PRED 0x45
#define SEC_PLN 0x44
#define ATC_COMM 0x43
#define MCDU_MENU 0x42
#define AIRPORT 0x41
#define ARROW_LEFT 0x3F
#define ARROW_UP 0x3E
#define ARROW_RIGHT 0x37
#define ARROW_DOWN 0x36
#define MCDU_KEY_A 0x3D
#define MCDU_KEY_B 0x3C
#define MCDU_KEY_C 0x3B
#define MCDU_KEY_D 0x3A
#define MCDU_KEY_E 0x39
#define MCDU_KEY_F 0x35
#define MCDU_KEY_G 0x34
#define MCDU_KEY_H 0x33
#define MCDU_KEY_I 0x32
#define MCDU_KEY_J 0x31
#define MCDU_KEY_K 0x1D
#define MCDU_KEY_L 0x1C
#define MCDU_KEY_M 0x1B
#define MCDU_KEY_N 0x1A
#define MCDU_KEY_O 0x19
#define MCDU_KEY_P 0x15
#define MCDU_KEY_Q 0x14
#define MCDU_KEY_R 0x13
#define MCDU_KEY_S 0x12
#define MCDU_KEY_T 0x11
#define MCDU_KEY_U 0x0D
#define MCDU_KEY_V 0x0C
#define MCDU_KEY_W 0x0B
#define MCDU_KEY_X 0x0A
#define MCDU_KEY_Y 0x09
#define MCDU_KEY_Z 0x05
#define MCDU_KEY_SLASH 0x04
#define MCDU_KEY_SP 0x03
#define MCDU_KEY_OVFY 0x02
#define MCDU_KEY_CLR 0x01
#define MCDU_KEY_1 0x20
#define MCDU_KEY_2 0x1F
#define MCDU_KEY_3 0x1E
#define MCDU_KEY_4 0x18
#define MCDU_KEY_5 0x17
#define MCDU_KEY_6 0x16
#define MCDU_KEY_7 0x10
#define MCDU_KEY_8 0x0F
#define MCDU_KEY_9 0x0E
#define MCDU_KEY_POINT 0x08
#define MCDU_KEY_0 0x07
#define MCDU_KEY_PLUS_MINUS 0x06

// define MCDU LSK's
#define LSK_1L 0x38
#define LSK_2L 0x40
#define LSK_3L 0x48
#define LSK_4L 0x50
#define LSK_5L 0x58
#define LSK_6L 0x57

#define LSK_1R 0x56
#define LSK_2R 0x55
#define LSK_3R 0x54
#define LSK_4R 0x53
#define LSK_5R 0x4A
#define LSK_6R 0x52


// Screen
// char mesg[] = "MCDU"; /* message to be appeared on the screen */
// void testScreenTools(void);
// int init_hid(void);
// void exit_hid(void);
// void testHid(void);
// void testModeKeys(void);

#endif
