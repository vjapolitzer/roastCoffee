#include <Arduino.h>
#include "Roaster.h"

#define BUTTON_PIN A0

Roaster sr500;

void setup()
{
    pinMode(BUTTON_PIN, INPUT);
    sr500.begin();
}

void loop()
{
    static uint16_t prevPress = 0;
    static uint16_t currPress;
    static unsigned long lastReadTime = millis();

    // Poll buttons once every millisecond
    if (millis() != lastReadTime)
    {
        currPress = analogRead(BUTTON_PIN) / 100;
        // Don't spam input for press and holds
        if (currPress != prevPress)
        {
            prevPress = currPress;
            if (prevPress != 0) sr500.input(prevPress);
        }
        lastReadTime = millis();
    }
    sr500.update();
}
