#ifndef ROASTER_H
#define ROASTER_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "MAX6675_lib.h"
#include "PID_lib.h"
#include "Triac_lib.h"

#define TC_PIN_1 7
#define TC_PIN_2 6

#define DISP_CS 10
#define DISP_DC 9
#define DISP_RST 8

enum Mode {Menu, Roasting, Cooling};

class Roaster
{
    public:
        Roaster();
        void begin();
        void input(uint8_t);
        bool update();

    private:
        void setMode(Mode);
        void readTemp();
        void drawDisp();

        static Mode mode;
        static uint8_t menuPage;
        static U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI roasterDisp;
        static MAX6675 tc1, tc2;
        static Triac heater, fan;
        static PID roastPID;

        static double inputPID, outputPID, setpointPID;
        static double tcTemp1, tcTemp2, tcTempAvg;
};

#endif
