#include <Arduino.h>
#include "Roaster.h"

void Roaster::startPIDStage(uint8_t stage)
{
        Roaster::stopPID();
    	Roaster::currStage = stage;
        Roaster::setPIDTarget(Roaster::setpointPID);
        Roaster::stagePID[Roaster::currStage].start();
}

void Roaster::setPIDTarget(double target)
{
    Roaster::stagePID[Roaster::currStage].set(target);
}

void Roaster::stopPID()
{
    Roaster::stagePID[Roaster::currStage].stop();
}

int16_t Roaster::computePID(double input)
{
    return Roaster::stagePID[Roaster::currStage].compute(input);
}

bool Roaster::isPIDEnabled()
{
    return Roaster::stagePID[Roaster::currStage].isEnabled();
}