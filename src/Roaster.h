#ifndef ROASTER_H
#define ROASTER_H

#include <Arduino.h>
#include <U8g2lib.h>
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

// Period in ms to reject extra button presses
#define BUTTON_UPDATE_PERIOD 100

// Button analogRead values
#define OK_BUTTON_VAL 5
#define BACK_BUTTON_VAL 7
#define UP_BUTTON_VAL 8
#define DOWN_BUTTON_VAL 9

// Maximum roast time in minutes
#define MAX_ROAST_TIME 14

// Cooling temperature target
#define COOLING_TEMP 50.0

// Number of pages in different modes
#define MENU_PAGES 3
#define ROASTING_PAGES 2
#define COOLING_PAGES 1
#define SUMMARY_PAGES 2
#define CONFIG_PAGES 2

#define STRING_BUFFER_SIZE 20

// enum for different machine states
enum Mode {Menu, Roasting, Cooling, Summary, Config};
// enum for different button presses
enum Button {NoB, UpB, DownB, BackB, OkB};

class Roaster
{
    public:
        /* begin()
        * ...Starts up the Triac and display drivers
        * ...Only call ONCE
        * ...Returns:
        * ......Nothing
        */
        void begin();

        /* input(...)
        * Accepts analogRead()/100 values
        * Sets internal variable for input handling
        * Configure the values in defines above
        * ...Parameters:
        * ......uint8_t buttonReadVal
        * ...Returns:
        * ......Nothing
        */
        void input(uint8_t);

        /* update()
        * Runs the machine, place in loop()
        * ...Returns:
        * ......Nothing
        */
        void update();

    private:
        void setMode(Mode);        // Transition between modes
        void interpolateProfile(); // For smooth profile curve
        bool readTemp();           // Gets thermocouple temps

        // Input handling functions, defined in Input.cpp
        void handleInput();         
        void nextPage();
        void prevPage();
        uint8_t getNumPages();

        // Display functions, defined in Display.cpp
        void drawDisp();
        void drawProfile();

        static Mode mode;                                  // Current state of machine
        static uint8_t dispPage;                           // Current page of mode to display
        static Button buttonPressed;                       // Holds the status of the buttons
        static uint8_t profileDuration;                    // Total minutes for profile
        static double profile[];                           // Minute by minute temperature targets
        static double profileSlope[];                      // For linear interpolation of profile
        static uint8_t roastStage;                         // Index in the profile index
        static unsigned long roastTime;                    // Elapsed roast time in seconds
        static U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled; // OLED display
        static MAX6675 tc1, tc2;                           // Thermocouples
        static Triac heater, fan;                          // Triacs for heating element and fan
        static uint8_t fanSpeed;                           // Fan power level (0-165)
        static double t1, t2, tAvg;                        // Thermocouple readings
        static double inputPID, outputPID, setpointPID;    // PID variables
        static double pGain, iGain, dGain;
        static PID roastPID;                               // PID controller for heating
};

#endif
