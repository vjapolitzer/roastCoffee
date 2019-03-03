#include <Arduino.h>
#include <stdio.h>
#include "Roaster.h"

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
                    Roaster::oled.drawStr(10, 35, buf);
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
                    sprintf(buf, "View Profile");
                    Roaster::oled.drawStr(10, 35, buf);
                    break;
                case 3:
                    sprintf(buf, "Start Roast?");
                    Roaster::oled.drawStr(20, 35, buf);
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
