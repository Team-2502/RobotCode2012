#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>
#include "Sharp_IR.h"

enum CollectorState
{
	OFF,
	LOOKING_FOR_BALLS,
	STAGE1,
	STAGE2,
	PREPARE_TO_SHOOT,
	SHOOTING
};
	
#define BLOCKED 1
#define OPEN 0
#define MAX_BALLS 2
#define RUNFAST .5
#define RUNSLOW .2
#define STOP 0
class Collector
{
public:
	Collector(int bottomChannel, int middleChannel, int frontIRChannel, int middleIRChannel, int topIRChannel, int numberOfBalls);
	~Collector();
	
	void Shoot();
	void ManipulateRamp();
	void Stop();
	void Eject();
	void RunCollector();
	
private:
	static int balls;
	static Victor *grabber;
	static Victor *lifter;
	static Sharp_IR *frontIR;
	static Sharp_IR *middleIR;
	static Sharp_IR *topIR;
	Task *collectorTask;
	static CollectorState collectorState;
	static void ThreadLoop();
	static void RejectBall();
	static void PrepareToShoot();

};

#endif
