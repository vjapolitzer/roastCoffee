#include <Arduino.h>
#include "Roaster.h"

#define BUTTON_PIN A0
#define DEBOUNCE 3

Roaster sr500;

void setup()
{
    pinMode(BUTTON_PIN, INPUT);
    sr500.begin();
}

void loop()
{
    static uint8_t debounceCount = 0;
    static uint16_t prevPress = 0;
    static uint16_t currPress;
    static unsigned long lastReadTime = millis();
    if (millis() != lastReadTime)
    {
        currPress = analogRead(BUTTON_PIN);
        if (currPress == prevPress && debounceCount > 0)
            debounceCount = 0;
        else if (currPress != prevPress)
            debounceCount++;

        if (debounceCount >= DEBOUNCE)
        {
            debounceCount = 0;
            prevPress = currPress;
            if (prevPress != 0) sr500.input(prevPress);
        }
        lastReadTime = millis();
    }
    sr500.update();
}
