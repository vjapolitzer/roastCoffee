#include <Arduino.h>
#include <stdio.h>
#include "Roaster.h"

Mode Roaster::mode = Menu;
uint8_t Roaster::dispPage = 0;

// default profile
uint8_t Roaster::profileDuration = 11;
double Roaster::profile[MAX_ROAST_TIME] = { 25.0, 60.0, 87.0, 112.0, 135.0, 155.0, 
                                            171.0, 184.0, 193.0, 199.0, 204.0, 204.5 };
double Roaster::profileSlope[MAX_ROAST_TIME - 1] = { 0 };
uint8_t Roaster::roastStage = 0;
unsigned long Roaster::roastTime = 0;

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI Roaster::oled(U8G2_R2, DISP_CS, DISP_DC, DISP_RST);

MAX6675 Roaster::tc1(TC_PIN_1);
MAX6675 Roaster::tc2(TC_PIN_2);

Triac Roaster::heater(HEAT_PIN);
Triac Roaster::fan(FAN_PIN);

double Roaster::t1 = 25.0;
double Roaster::t2 = 25.0;
double Roaster::tAvg = 25.0;

double Roaster::inputPID = 25.0;
double Roaster::outputPID = 0.0;
double Roaster::setpointPID = 25.0;

double Roaster::pGain = 10.0;
double Roaster::iGain = 0.5;
double Roaster::dGain = 3.0;
PID Roaster::roastPID(&Roaster::inputPID, &Roaster::outputPID, &Roaster::setpointPID,
                      Roaster::pGain, Roaster::iGain, Roaster::dGain,
                      (double)0, (double)165, Direct);

void Roaster::begin()
{
    Roaster::heater.begin();
    Roaster::oled.begin();
    Roaster::oled.setFont(u8g2_font_t0_13_tf);
    Roaster::interpolateProfile();
}

bool Roaster::update()
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
            currTime = millis(); 
            timeChange = currTime - prevTime;
            if (timeChange >= 1000)
            {
                Roaster::roastTime++; // One second has passed

                secondsIntoStage = Roaster::roastTime % 60;
                // Increment to next stage every minute
                if (secondsIntoStage == 0) Roaster::roastStage++;
                // Update the temperature target for the PID controller
                Roaster::setpointPID = Roaster::profile[Roaster::roastStage]
                                       + (Roaster::profileSlope[Roaster::roastStage]
                                          * secondsIntoStage);
                prevTime = currTime;
            }

            if (Roaster::roastTime >= Roaster::profileDuration * 60000)
            {
                Roaster::setMode(Cooling);
                break;
            }

            // TODO: Ramp down of fan halfway through profile

            if (Roaster::roastPID.compute())
                Roaster::heater.set((uint8_t)Roaster::outputPID);

            break;

        case Cooling:
            if (Roaster::tAvg <= COOLING_TEMP)
            {
                Roaster::fan.off();
                Roaster::setMode(Summary);
            }
            break;

        case Summary:
            break;
        
        case Config:
            break;
    }
    // Update the oled display
    Roaster::drawDisp();

    return true;
}

void Roaster::setMode(Mode m)
{
    switch (m)
    {
        case Menu:
            // TODO: set page to summary page
            break;

        case Roasting:
            while(!Roaster::readTemp());
            Roaster::inputPID = Roaster::tAvg;
            Roaster::profile[0] = Roaster::tAvg;
            Roaster::setpointPID = Roaster::tAvg;
            Roaster::roastStage = 0;
            Roaster::roastTime = 1;
            Roaster::fan.on();
            Roaster::roastPID.start();
            break;

        case Cooling:
            Roaster::roastPID.stop();
            Roaster::heater.off();
            Roaster::fan.on();
            break;

        case Summary:
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

void Roaster::drawDisp()
{
    char buf[STRING_BUFFER_SIZE];
    Roaster::oled.clearBuffer();
    switch (Roaster::mode)
    {
        case Menu:
            switch (Roaster::dispPage)
            {
                case 0: // Start roast
                    break;
                
                case 1: // View temperatures
                    sprintf(buf, "Temp1: %3d.%02d\xB0\x43", (int)Roaster::t1, (int)(Roaster::t1 * 100) % 100);
                    Roaster::oled.drawStr(1, 10, buf);

                    sprintf(buf, "Temp2: %3d.%02d\xB0\x43", (int)Roaster::t2, (int)(Roaster::t2 * 100) % 100);
                    Roaster::oled.drawStr(1, 30, buf);

                    sprintf(buf, "  Avg: %3d.%02d\xB0\x43", (int)Roaster::tAvg, (int)(Roaster::tAvg * 100) % 100);
                    Roaster::oled.drawStr(1, 50, buf);
                    break;

                case 2: // View profile
                    break;
            }
            break;

        case Roasting:
            switch (Roaster::dispPage)
            {
                case 0:
                    break;
                
                case 1:
                    break;
            }
            break;

        case Cooling:
            break;

        case Summary:
            switch (Roaster::dispPage)
            {
                case 0:
                    break;
                
                case 1:
                    break;
            }
            break;

        case Config:
            switch (Roaster::dispPage)
            {
                case 0:
                    break;
                
                case 1:
                    break;
            }
            break;
    }
    Roaster::oled.sendBuffer();
}