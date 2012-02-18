#ifndef SHARP_IR_H
#define SHARP_IR_H

#include <WPILib.h>

class Sharp_IR : public AnalogChannel
{
public:
	Sharp_IR(UINT8 moduleNumber, UINT32 channel) : AnalogChannel(moduleNumber, channel) {}
	bool Get() { return GetVoltage() > 2.5; } //Experimentally determined.
	
};

#endif
