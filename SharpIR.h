#ifndef SHARPIR_H
#define SHARPIR_H

#include <WPILib.h>

class SharpIR : public AnalogChannel
{
private:
	double signalVoltage;
public:
	SharpIR(UINT8 moduleNumber, UINT32 channel, double signalVoltage);
	bool Get();
	
};

#endif
