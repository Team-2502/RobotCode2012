#include <WPILib.h>
#include "nivision.h"
#include <cmath>
#include "vision.h"
#include "Singleton.h"
#include "Display.h"

TargetReport VisionSystem::bestTarget = TargetReport();
AxisCamera *VisionSystem::cam = NULL;
VisionSpecifics *VisionSystem::engine = NULL;

VisionSystem::VisionSystem(VisionSpecifics *backend)
{
	engine = backend;
	cam = &AxisCamera::GetInstance("10.25.2.11");
	cam->WriteResolution(AxisCamera::kResolution_320x240);

	visionTask = new Task("2502Vn",(FUNCPTR)loop);
}

VisionSystem::~VisionSystem()
{
	delete engine;
	visionTask->Stop();
	delete visionTask;
}

void VisionSystem::start()
{
	visionTask->Start();
}

void VisionSystem::stop()
{
	visionTask->Stop();
}

void VisionSystem::loop()
{
	while(true)
	{
		HSLImage *cap = new HSLImage;
		cam->GetImage(cap);
		bestTarget = engine->getBestTarget(cap);
		delete cap;
		Wait(0.01);
	}
}

double VisionSystem::getTargetHeadingX()
{
	return bestTarget.normalized_x; 
}
