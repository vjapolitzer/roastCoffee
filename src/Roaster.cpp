#include <Arduino.h>
#include <SPI.h>
#include "Roaster.h"

Mode Roaster::mode = Menu;
Button Roaster::buttonPressed = NoB;
uint8_t Roaster::dispPage = 0;

// default profile
uint8_t Roaster::profileDuration = 11;
double Roaster::profile[MAX_ROAST_TIME] = { 25.0, 58.0, 87.0, 113.0, 136.0, 155.0,
                                            171.0, 184.0, 193.0, 199.0, 204.0, 204.5 };
double Roaster::profileSlope[MAX_ROAST_TIME - 1] = { 0 };
uint8_t Roaster::roastStage = 0;
unsigned long Roaster::roastTime = 0;

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI Roaster::oled(U8G2_R2, DISP_CS, DISP_DC, DISP_RST);

MAX6675 Roaster::tc1(TC_PIN_1);
MAX6675 Roaster::tc2(TC_PIN_2);

Triac Roaster::heater(HEAT_PIN);
Triac Roaster::fan(FAN_PIN);
uint8_t Roaster::fanSpeed = 0;

double Roaster::t1 = 25.0;
double Roaster::t2 = 25.0;
double Roaster::tAvg = 25.0;

int16_t Roaster::outputPID = 0;
double Roaster::setpointPID = 25.0;

PID Roaster::roastPID(100, 1, 0,
                      0, 165, TC_UPDATE_PERIOD);

void Roaster::begin()
{
    Roaster::heater.begin();
    Roaster::oled.begin();
    Roaster::oled.setFont(u8g2_font_t0_13_tf);
}

void Roaster::update()
{
    // Time tracking
    static unsigned long prevTime = millis();
    unsigned long currTime; 
    unsigned long timeChange;

    // Time for profile interpolation and incrementing roast stage
    unsigned long secondsIntoStage;

    // Get the latest temperature reading if available
    Roaster::readTemp();

    switch (Roaster::mode)
    {
        case Menu:
            break;

        case Roasting:
            // Time tracking
            currTime = millis(); 
            timeChange = currTime - prevTime;

            // Update the PID setpoint according the profile, once per second
            if (timeChange >= 1000)
            {
                Roaster::roastTime++;

                secondsIntoStage = Roaster::roastTime % 60;
                // Increment to next stage every minute
                if (secondsIntoStage == 0) Roaster::roastStage++;
                // Update the temperature target for the PID controller
                Roaster::setpointPID = Roaster::profile[Roaster::roastStage]
                                       + (Roaster::profileSlope[Roaster::roastStage]
                                          * secondsIntoStage);
                Roaster::roastPID.set(Roaster::setpointPID);
                
                // Ramp down fan to 88% halfway through the roast
                if (Roaster::roastTime >= Roaster::profileDuration * 30
                    && Roaster::fanSpeed > 146)
                {
                    Roaster::fan.set(--Roaster::fanSpeed);
                }

                prevTime = currTime;
            }

            if (Roaster::roastTime >= Roaster::profileDuration * 60)
            {
                Roaster::setMode(Cooling);
                break;
            }

            if (Roaster::roastPID.isEnabled())
            {
                Roaster::outputPID = Roaster::roastPID.compute(Roaster::tAvg);
                Roaster::heater.set((uint8_t)Roaster::outputPID);
            }

            break;

        case Cooling:
            if (Roaster::tAvg <= COOLING_TEMP) Roaster::setMode(Menu); //Roaster::setMode(Summary);
            break;

        case Summary:
            break;
        
        case Config:
            break;
    }
    if (Roaster::buttonPressed != NoB)
    {
        Roaster::handleInput();
        Roaster::buttonPressed = NoB;
    }
    Roaster::drawDisp();
}

void Roaster::setMode(Mode m)
{
    switch (m)
    {
        case Menu:
            Roaster::fan.off();
            break;

        case Roasting:
            // Ensure we have a valid temperature
            // TODO: add a timeout here
            while(!Roaster::readTemp());
            // Initialize PID variables to room temp
            Roaster::profile[0] = Roaster::tAvg;
            Roaster::setpointPID = Roaster::tAvg;
            Roaster::roastPID.set(Roaster::setpointPID);
            Roaster::interpolateProfile();
            Roaster::roastStage = 0;
            Roaster::roastTime = 0;
            Roaster::fanSpeed = 165;
            // And we are off to the races!
            Roaster::fan.on();
            Roaster::roastPID.start();
            break;

        case Cooling:
            Roaster::roastPID.stop();
            Roaster::heater.off();
            Roaster::fan.on();
            break;

        case Summary:
            Roaster::fan.off();
            break;

        case Config:
            break;
    }
    Roaster::dispPage = 0;
    Roaster::mode = m;
}

void Roaster::interpolateProfile()
{
    for (int i = 0; i < Roaster::profileDuration; i++)
    {
        Roaster::profileSlope[i] = (Roaster::profile[i + 1]
                                    - Roaster::profile[i]) / 60.0;
    }
}

bool Roaster::readTemp()
{
    static unsigned long prevTime = millis() - TC_UPDATE_PERIOD;
    unsigned long currTime = millis(); 
    unsigned long timeChange = currTime - prevTime;

    double t1, t2;
    if (timeChange >= TC_UPDATE_PERIOD)
    { // TODO: Better handle NaN errors for TC disconnect
        t1 = Roaster::tc1.readCelsius();
        t2 = Roaster::tc2.readCelsius();

        if (t1 == NAN && t2 == NAN) return false;

        if (t1 != NAN && t2 != NAN)
        {
            Roaster::t1 = t1;
            Roaster::t2 = t2;
        }
        else if (t1 == NAN && t2 != NAN)
        {
            Roaster::t1 = t2;
            Roaster::t2 = t2;
        }
        else
        {
            Roaster::t1 = t1;
            Roaster::t2 = t1;
        }
        Roaster::tAvg = (Roaster::t1 + Roaster::t2) / 2.0;
        prevTime = currTime;
        return true;
    }
    return false;
}
