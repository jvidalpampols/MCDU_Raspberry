//hidTools.h

#ifndef HIDTOOLS_H
#define HIDTOOLS_H
#include <hidapi/hidapi.h>
extern hid_device *handle;
extern unsigned char buf[65]; // Make sure this is big enough: 1 for Report ID + 64 for data
extern unsigned char cmd[65]; // HID report ID is often 0

bool init_hid();
void exit_hid();
void hidSelectCLEAR(void);
void setLed_MENU();
void resetLed_MENU();
void setLed_MCDU();
void resetLed_MCDU();
void setLed_FMGC();
void resetLed_FMGC();
void setLed_FAIL();
void resetLed_FAIL();
void setLed_FM1();
void resetLed_FM1();
void setLed_IND();
void resetLed_IND();
void setLed_RDY();
void resetLed_RDY();
void setLed_FM2();
void resetLed_FM2();


#endif