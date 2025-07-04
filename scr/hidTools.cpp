// hidTools.cpp
#include <hidapi/hidapi.h>
#include "fileTools.h"
//#include "screenTools.h"
//#include "hidTools.h"

void hidSelectCLEAR(void);

hid_device *handle;
unsigned char buf[65] = {0}; // Make sure this is big enough: 1 for Report ID + 64 for data
unsigned char cmd[65] = {0}; // HID report ID is often 0
unsigned char cmdByte_1 = 0b00000000; //Byte _1 Global to keep the cmd's to LED's
unsigned char cmdByte_2 = 0b00000000; //Byte_2 Global to keep the cmd's to LED's

bool init_hid()
{
    system("clear");
    printf("\nInit HID\n");
    hid_init();
    sleep(1); // Wait for 5 seconds

    handle = hid_open(0x0000, 0x0013, NULL); // Use your correct VID/PID
    if (!handle)
    {
        printf("Unable to open HIDdevice\n");
        sleep(5); // Wait for 5 seconds
        return FALSE;
    }

    printf("\nHID device OPEN\n");
    sleep(2); // Wait for 1 seconds

    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1); //
    printf("\nHID init OK\n");
    //waitKeyPressed();
    sleep(2); // Wait for 1 seconds
    // reset MCDU lights
    cmd[1] = 0;
    hid_write(handle, cmd, sizeof(cmd));
   
    // MCDU light RDY ON
    cmd[1] = 64;
    hid_write(handle, cmd, sizeof(cmd));
    printf("\nMCDU light RDY ON\n");
    sleep(2); // Wait for 1 seconds
    
    return TRUE;
}

void exit_hid()
{
    // reset MCDU lights
    hidSelectCLEAR();
    // Normal exit. Close the HID device
    hid_close(handle); // Always close the device before exiting

    // Finalize the hidapi library
    hid_exit(); // Library function. This finalizes and frees resources

    printf("\n Exiting HID program\n");
    //waitKeyPressed();
    // return TRUE; // Ensure no access to handle or buf hereafter
}

void hidSelectCLEAR()
{
    cmdByte_1 = 0b0000000;
    cmdByte_2 = 0b0000000;
    cmd[1] = cmdByte_1; // LED FAIL to turn on
    cmd[2] = cmdByte_2; // LED FAIL to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_MENU()
{

    cmdByte_1 = cmdByte_1 | 0b00000001; // LED MENU 1
    cmd[1] = cmdByte_1; //LED FAIL to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_MENU()
{

    cmdByte_1 = cmdByte_1 & 0b11111110; // LED MENU 1
    cmd[1] = cmdByte_1;                 // LED MENU to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_MCDU()
{

    cmdByte_1 = cmdByte_1 | 0b00000010; // LED MCDU 2
    cmd[1] = cmdByte_1;                 // LED MCDU to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_MCDU()
{

    cmdByte_1 = cmdByte_1 & 0b11111101; // LED MCDU 2
    cmd[1] = cmdByte_1;                 // LED MCDU to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_FMGC()
{

    cmdByte_1 = cmdByte_1 | 0b00000100; // LED FMGC 4
    cmd[1] = cmdByte_1;                 // LED FMGC to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_FMGC()
{

    cmdByte_1 = cmdByte_1 & 0b11111011; // LED FMGC 4
    cmd[1] = cmdByte_1;                 // LED FMGC to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_FAIL()
{

    cmdByte_1 = cmdByte_1 | 0b00001000; // LED FAIL 8
    cmd[1] = cmdByte_1;                 // LED FAIL to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_FAIL()
{

    cmdByte_1 = cmdByte_1 & 0b11110111; // LED  FAIL 8
    cmd[1] = cmdByte_1;                 // LED FAIL to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_FM1()
{

    cmdByte_1 = cmdByte_1 | 0b00010000; // LED FM1 16
    cmd[1] = cmdByte_1;                 // LED FM1 to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_FM1()
{

    cmdByte_1 = cmdByte_1 & 0b11101111; // LED  FM1 16
    cmd[1] = cmdByte_1;                 // LED FM1 to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_IND()
{

    cmdByte_1 = cmdByte_1 | 0b00100000; // LED IND 32
    cmd[1] = cmdByte_1;                 // LED IND to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_IND()
{

    cmdByte_1 = cmdByte_1 & 0b11011111; // LED  IND 32
    cmd[1] = cmdByte_1;                 // LED FM1 to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_RDY()
{

    cmdByte_1 = cmdByte_1 | 0b01000000; // LED RDY 64
    cmd[1] = cmdByte_1;                 // LED RDY to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_RDY()
{

    cmdByte_1 = cmdByte_1 & 0b10111111; // LED  RDY 64
    cmd[1] = cmdByte_1;                 // LED RDY to turn off
    hid_write(handle, cmd, sizeof(cmd));
}

void setLed_FM2()
{

    cmdByte_2 = cmdByte_2 | 0b00000001; // LED FM2 1
    cmd[2] = cmdByte_2;                 // LED FM2 to turn on
    hid_write(handle, cmd, sizeof(cmd));
}

void resetLed_FM2()
{

    cmdByte_2 = cmdByte_2 & 0b11111110; // LED FM2 1
    cmd[2] = cmdByte_2;                 // LED FM2 to turn off
    hid_write(handle, cmd, sizeof(cmd));
}
