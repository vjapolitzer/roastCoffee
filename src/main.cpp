#include <Arduino.h>
#include "Roaster.h"

Roaster sr500;

void setup()
{
    sr500.begin();
}

void loop()
{
    sr500.update();
}
