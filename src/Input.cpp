#include <Arduino.h>
#include "Roaster.h"

void Roaster::input(uint8_t buttonReadVal)
{
    static unsigned long prevTime = millis() - BUTTON_UPDATE_PERIOD;
    unsigned long currTime = millis(); 
    unsigned long timeChange = currTime - prevTime;

    if (timeChange >= BUTTON_UPDATE_PERIOD)
    {
        switch (buttonReadVal)
        {
            case OK_BUTTON_VAL:
                Roaster::buttonPressed = OkB;
                break;
            case BACK_BUTTON_VAL:
                Roaster::buttonPressed = BackB;
                break;
            case UP_BUTTON_VAL:
                Roaster::buttonPressed = UpB;
                break;
            case DOWN_BUTTON_VAL:
                Roaster::buttonPressed = DownB;
                break;
            default:
                Roaster::buttonPressed = NoB;
        }
        prevTime = currTime;
    }
}

void Roaster::handleInput()
{
    switch (Roaster::mode)
    {
        case Menu:
            switch (Roaster::dispPage)
            {
                case 0: // Start roast
                    switch (Roaster::buttonPressed)
                    {
                        case UpB:
                            Roaster::nextPage();
                            break;
                        
                        case DownB:
                            Roaster::prevPage();
                            break;

                        case OkB:
                            Roaster::dispPage = MENU_PAGES;
                            break;

                        default:
                            break;
                    }
                    break;

                case 1: // View temperatures
                    switch (Roaster::buttonPressed)
                    {
                        case UpB:
                            Roaster::nextPage();
                            break;
                        
                        case DownB:
                            Roaster::prevPage();
                            break;

                        default:
                            break;
                    }
                    break;

                case 2: // View profile
                    switch (Roaster::buttonPressed)
                    {
                        case UpB:
                            Roaster::nextPage();
                            break;
                        
                        case DownB:
                            Roaster::prevPage();
                            break;

                        case OkB:
                            // Roaster::setMode(Config);
                            break;

                        default:
                            break;
                    }
                    break;

                case MENU_PAGES: // Confirmation
                    switch (Roaster::buttonPressed)
                    {
                        case BackB:
                            Roaster::dispPage = 0;
                            break;

                        case OkB:
                            Roaster::setMode(Roasting);
                            break;

                        default:
                            break;
                    }
                    break;
            }
            break;

        case Roasting:
            switch (Roaster::dispPage)
            {
                case 0:
                    switch (Roaster::buttonPressed)
                    {
                        case UpB:
                            // Roaster::nextPage();
                            break;
                        
                        case DownB:
                            // Roaster::prevPage();
                            break;

                        case BackB:
                            Roaster::dispPage = ROASTING_PAGES;
                            break;

                        default:
                            break;
                    }
                    break;
                
                case 1:
                    break;

                case ROASTING_PAGES: // Confirmation
                    switch (Roaster::buttonPressed)
                    {
                        case BackB:
                            Roaster::dispPage = 0;
                            break;

                        case OkB:
                            Roaster::setMode(Cooling);
                            break;

                        default:
                            break;
                    }
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
}

void Roaster::nextPage()
{
    Roaster::dispPage = (Roaster::dispPage + 1) % Roaster::getNumPages();
}

void Roaster::prevPage()
{
    uint8_t numPages = Roaster::getNumPages();
    Roaster::dispPage = (Roaster::dispPage + numPages - 1) % numPages;
}

uint8_t Roaster::getNumPages()
{
    switch (Roaster::mode)
    {
        case Menu:
            return MENU_PAGES;

        case Roasting:
            return ROASTING_PAGES;

        case Cooling:
            return COOLING_PAGES;

        case Summary:
            return SUMMARY_PAGES;
            
        case Config:
            return CONFIG_PAGES;
    }
    return 0;
}
