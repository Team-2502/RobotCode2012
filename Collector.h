#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>

class Collector
{
public:
	Collector(int channel1, int channel2);
	~Collector();
	
	void Shoot();
	void ManipulateRamp();
	void GotIR();
	void Stop();
	void Eject();
	void RunCollector();
	
private:
	int balls;
	bool collecting;
	bool shooting;
	Victor *bottom;
	Victor *center;
};

#endif
