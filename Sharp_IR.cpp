#include <WPILib.h>
#include "Sharp_IR.h"

Sharp_IR::Sharp_IR(UINT8 moduleNumber, UINT32 channel, double signalVoltage) :
	AnalogChannel(moduleNumber, channel)
{
	this->signalVoltage = signalVoltage;
}

bool Sharp_IR::Get() 
{ 
	return GetVoltage() > signalVoltage; 
}
