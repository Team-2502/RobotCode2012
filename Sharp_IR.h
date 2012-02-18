#ifndef SHARP_IR_H
#define SHARP_IR_H

#include <WPILib.h>

class Sharp_IR : public AnalogChannel
{
public:
	Sharp_IR(UINT8 moduleNumber, UINT32 channel) : AnalogChannel(moduleNumber, channel) {}
	bool Get() { return getDistance() < 4.5; }
	
private:
	inline float getDistance() { return 4.0+GetVoltage()/(30.0-4.0); }
};

#endif
