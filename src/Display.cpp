#include <Arduino.h>
#include <stdio.h>
#include "Roaster.h"
#include "Logo.h"

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
                    sprintf(buf, "roastCoffee");
                    Roaster::oled.drawStr(24, 10, buf);
                    sprintf(buf, "[OK - Begin Roast]");
                    Roaster::oled.drawStr(2, 60, buf);
                    Roaster::oled.drawXBMP(20, 15, LOGO_WIDTH, LOGO_HEIGHT, logo_bits);
                    break;
                
                case 1: // View temperatures
                    sprintf(buf, "Thermocouples");
                    Roaster::oled.drawStr(10, 10, buf);

                    sprintf(buf, "Temp1: %3d.%02d\xB0\x43",
                            (int)Roaster::t1, (int)(Roaster::t1 * 100) % 100);
                    Roaster::oled.drawStr(1, 25, buf);

                    sprintf(buf, "Temp2: %3d.%02d\xB0\x43",
                            (int)Roaster::t2, (int)(Roaster::t2 * 100) % 100);
                    Roaster::oled.drawStr(1, 40, buf);

                    sprintf(buf, "  Avg: %3d.%02d\xB0\x43",
                            (int)Roaster::tAvg, (int)(Roaster::tAvg * 100) % 100);
                    Roaster::oled.drawStr(1, 55, buf);
                    break;

                case 2: // View profile
                    sprintf(buf, "Roast Profile");
                    Roaster::oled.drawStr(10, 10, buf);
                    Roaster::drawProfile();
                    break;

                case MENU_PAGES: // Confirmation
                    sprintf(buf, "Start Roast?");
                    Roaster::oled.drawStr(20, 35, buf);
                    break;
            }
            break;

        case Roasting:
            switch (Roaster::dispPage)
            {
                case 0:
                    sprintf(buf, "Time: %2dm %2ds",
                            (int)Roaster::roastTime / 60, (int)Roaster::roastTime % 60);
                    Roaster::oled.drawStr(1, 10, buf);

                    sprintf(buf, "Actual: %3d.%02d\xB0\x43",
                            (int)Roaster::tAvg, (int)(Roaster::tAvg * 100) % 100);
                    Roaster::oled.drawStr(1, 25, buf);

                    sprintf(buf, "Target: %3d.%02d\xB0\x43",
                            (int)Roaster::setpointPID, (int)(Roaster::setpointPID * 100) % 100);
                    Roaster::oled.drawStr(1, 40, buf);

                    sprintf(buf, "Heat:%3d Fan:%3d",
                            (int)(Roaster::heater.getPower() * 100 / 165), (int)(Roaster::fan.getPower() * 100 / 165));
                    Roaster::oled.drawStr(1, 55, buf);
                    break;
                
                case 1:
                    break;

                case ROASTING_PAGES: // Confirmation
                    sprintf(buf, "Stop Roast?");
                    Roaster::oled.drawStr(20, 35, buf);
                    break;
            }
            break;

        case Cooling:
            sprintf(buf, "Cooling");
            Roaster::oled.drawStr(10, 10, buf);

            sprintf(buf, "Actual: %3d.%02d\xB0\x43",
                    (int)Roaster::tAvg, (int)(Roaster::tAvg * 100) % 100);
            Roaster::oled.drawStr(1, 25, buf);

            sprintf(buf, "Target: 40.00\xB0\x43");
            Roaster::oled.drawStr(1, 40, buf);
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

void Roaster::drawProfile()
{
    uint8_t i, x0, y0, x1, y1;
    // Vertical scale
    Roaster::oled.drawLine(1, 62, 1, 20);
    for (i = 62; i > 20; i -= 4)
        Roaster::oled.drawPixel(0, i);
    // Horizontal scale
    Roaster::oled.drawLine(1, 62, 97, 62);
    for (i = 1; i < 98; i += 4)
        Roaster::oled.drawPixel(i, 63);

    for (i = 0; i < Roaster::profileDuration - 1; i++)
    {
        x0 = i * 7 + 1;
        y0 = 63 - (int)(Roaster::profile[i] / 5.0);
        x1 = (i + 1) * 7 + 1;
        y1 = 63 - (int)(Roaster::profile[i + 1] / 5.0);
        Roaster::oled.drawLine(x0, y0, x1, y1);
    }
}
