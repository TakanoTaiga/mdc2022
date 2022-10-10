#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include "ticker_api.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

#define PPR 48
#define RPM 114

Ticker flipper;

DigitalOut A_Digital(PC_8);
DigitalOut B_Digital(PC_6);
DigitalOut X_Digital(PC_5);


int X = 0;
int flags = 0;


void ser(){
    switch (flags) {
    case 0:
    A_Digital = 1;
    X_Digital = 0;
    flags++;
    X++;
    break;

    case 1:
    B_Digital = 1;
    flags++;
    break;

    case 2:
    A_Digital = 0;
    flags++;
    break;

    case 3:
    B_Digital = 0;
    flags = 0;

    if(X >= PPR){
        X_Digital = 1;
        X = 0;
    }
    break;

    }
}


int main()
{
    int pulseWidth = ((60 * 1000 * 1000) / RPM) / (48 * 4);
    flipper.attach_us(&ser, pulseWidth);
}
