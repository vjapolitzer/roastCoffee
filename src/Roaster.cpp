#include <Arduino.h>
#include "Roaster.h"

Mode Roaster::mode = Menu;
uint8_t Roaster::dispPage = 0;

// default profile
uint8_t Roaster::profileDuration = 11;
double profile[MAX_ROAST_TIME] = { 25.0, 60.0, 87.0, 112.0, 135.0, 155.0, 
                                   171.0, 184.0, 193.0, 199.0, 204.0, 204.5 };
uint8_t roastStage = 0;
unsigned long roastTime = 0;

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI Roaster::oled(U8G2_R2, DISP_CS, DISP_DC, DISP_RST);

MAX6675 Roaster::tc1(TC_PIN_1);
MAX6675 Roaster::tc2(TC_PIN_2);

Triac Roaster::heater(HEAT_PIN);
Triac Roaster::fan(FAN_PIN);

double Roaster::inputPID = 25.0;
double Roaster::outputPID = 0.0;
double Roaster::setpointPID = 25.0;

double Roaster::pGain = 10.0;
double Roaster::iGain = 0.5;
double Roaster::dGain = 3.0;
PID Roaster::roastPID(&Roaster::inputPID, &Roaster::outputPID, &Roaster::setpointPID,
                      Roaster::pGain, Roaster::iGain, Roaster::dGain,
                      (double)0, (double)166, Direct);

void Roaster::begin()
{
    heater.begin();
    oled.begin();
}

bool Roaster::update()
{
    static bool updateError = false;

    static unsigned long prevTime = millis();
    unsigned long currTime = millis(); 
    unsigned long timeChange = currTime - prevTime;
    
    unsigned long secondsIntoStage;
    static double profileSlope = (Roaster::profile[1] - Roaster::profile[0]) / 60.0;;

    Roaster::readTemp();
    switch (Roaster::mode)
    {
        case Menu:
            break;

        case Roasting:
            if (timeChange >= 1000)
            {
                Roaster::roastTime++;

                secondsIntoStage = Roaster::roastTime % 60;

                if (secondsIntoStage == 0)
                {
                    Roaster::roastStage++;
                    profileSlope = (Roaster::profile[Roaster::roastStage + 1]
                                    - Roaster::profile[Roaster::roastStage]) / 60.0;
                }

                Roaster::setpointPID = Roaster::profile[Roaster::roastStage]
                                       + profileSlope * secondsIntoStage;
            }

            if (Roaster::roastTime >= Roaster::profileDuration * 60000)
            {
                Roaster::setMode(Cooling);
                break;
            }
            break;

        case Cooling:
            if (Roaster::tcTempAvg <= COOLING_TEMP)
            {
                Roaster::fan.off();
                Roaster::setMode(Summary);
            }
            break;

        case Summary:
            break;
    }

    Roaster::drawDisp();

    return updateError;
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
            Roaster::inputPID = Roaster::tcTempAvg;
            Roaster::profile[0] = Roaster::tcTempAvg;
            Roaster::setpointPID = Roaster::tcTempAvg;
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
    }
    Roaster::dispPage = 0;
    Roaster::mode = m;
}

bool Roaster::readTemp()
{
    static unsigned long prevTime = millis() - TC_UPDATE_PERIOD;
    unsigned long currTime = millis(); 
    unsigned long timeChange = currTime - prevTime;
    if (timeChange >= TC_UPDATE_PERIOD)
    { // TODO: Handle NaN errors for TC disconnect
        tcTemp1 = tc1.readCelsius();
        tcTemp2 = tc2.readCelsius();
        tcTempAvg = (tcTemp1 + tcTemp2) / 2.0;
        return true;
    }
    return false;
}

void Roaster::drawDisp()
{
    return;
}