#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>
#include "Constants.h"
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

enum RampState
{
	RAMP_OFF,
	UP,
	DOWN
};
	

class Collector
{
public:
	Collector();
	~Collector();
	
	void Shoot();
	void ManipulateRamp(RampState state);
	void Stop();
	void Eject();
	void Start();
	static void SetBallCount( int balls );
	
private:
	static unsigned balls;
	static Victor *grabber;
	static Victor *lifter;
	static Sharp_IR *frontIR;
	static Sharp_IR *middleIR;
	static Sharp_IR *topIR;
	Task *collectorTask;
	Relay *strike1;
	Relay *strike2;
	static CollectorState collectorState;
	static void ThreadLoop();
	static void RejectBall();
	static void PrepareToShoot();

};

#endif
