#include <WPILib.h>
#include "SharpIR.h"

SharpIR::SharpIR(UINT8 moduleNumber, UINT32 channel, double signalVoltage) :
	AnalogChannel(moduleNumber, channel)
{
	this->signalVoltage = signalVoltage;
}

bool SharpIR::Get() 
{ 
	return GetVoltage() > signalVoltage; 
}
