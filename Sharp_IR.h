#ifndef SHARP_IR_H
#define SHARP_IR_H

#include <WPILib.h>

class Sharp_IR : public AnalogChannel
{
private:
	double signalVoltage;
public:
	Sharp_IR(UINT8 moduleNumber, UINT32 channel, double signalVoltage);
	bool Get();
	
};

#endif
