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
	int GetNumberOfBalls();
	void Stop();
	void Eject();
	void RunCollector();
	void Update();
	
private:
	int balls;
	bool collecting;
	bool shooting;
	Victor *bottom;
	Victor *center;
};

#endif