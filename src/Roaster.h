#ifndef ROASTER_H
#define ROASTER_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "MAX6675_lib.h"
#include "PID_lib.h"
#include "Triac_lib.h"

// Placeholder pin definitions, subject to change
#define DISP_CS 10
#define DISP_DC 9
#define DISP_RST 8

#define TC_PIN_1 7
#define TC_PIN_2 6

#define HEAT_PIN 5
#define FAN_PIN 4

// Period in ms to update temperature readings
#define TC_UPDATE_PERIOD 300

// Maximum roast time in minutes
#define MAX_ROAST_TIME 14

// Cooling temperature target
#define COOLING_TEMP 40.0

#define STRING_BUFFER_SIZE 20

enum Mode {Menu, Roasting, Cooling, Summary, Config};

class Roaster
{
    public:
        void begin();
        void input(uint8_t);
        bool update();

    private:
        void setMode(Mode);
        void interpolateProfile();
        bool readTemp();
        void drawDisp();

        static Mode mode;
        static uint8_t dispPage;                           // current page of mode to display
        static uint8_t profileDuration;                    // Total minutes for profile
        static double profile[];                           // minute by minute temperature targets
        static double profileSlope[];                      // for linear interpolation of profile
        static uint8_t roastStage;                         // index in the profile index
        static unsigned long roastTime;                    // elapsed roast time in seconds
        static U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled; // OLED display
        static MAX6675 tc1, tc2;                           // Thermocouples
        static Triac heater, fan;                          // Triacs for heating element and fan
        static PID roastPID;                               // PID controller for heating

        static double t1, t2, tAvg;
        static double inputPID, outputPID, setpointPID;
        static double pGain, iGain, dGain;

        // drawDisp helpers
        
};

#endif
