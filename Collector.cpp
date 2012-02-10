#include <WPILib.h>
#include "Collector.h"

Collector::Collector(int channel1, int channel2)
{
	this->balls = 0;
	this->bottom = new Victor(channel1);
	this->center = new Victor(channel2);
	this->shooting = false;
	this->collecting = true;
}

Collector::~Collector()
{
	delete bottom;
	delete center;
}

void Collector::Shoot()
{
	if (shooting)
		center->Set(1);
	shooting = false;
}

void Collector::ManipulateRamp()
{
	collecting = false; // Shouldn't collect while going across ramp
	shooting = false; // Shouldn't shoot while going across ramp
	bottom->Set(0);
	center->Set(0);
}

// Turn off everything
void Collector::Stop()
{
	bottom->Set(0);
	center->Set(0);
	collecting = false;
	shooting = false;
}

// Ejects ALL Balls - as long as it is running
void Collector::Eject()
{
	bottom->Set(-1);
	center->Set(-1);
}

void Collector::RunCollector()
{
	
}
