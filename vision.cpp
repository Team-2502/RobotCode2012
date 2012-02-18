#include <WPILib.h>
#include "nivision.h"
#include <cmath>
#include "vision.h"
#include "Singleton.h"
#include "Display.h"

AxisCamera *VisionSystem::cam = NULL;
VisionSpecifics *VisionSystem::engine = NULL;
int VisionSystem::bestTargetCount = 0;
TargetReport VisionSystem::bestTargets[] = {0, 0, 0, 0};

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
		engine->getBestTargets(cap, bestTargets, bestTargetCount);
		delete cap;
		Wait(0.01);
	}
}

double VisionSystem::getTargetHeadingX()
{
	return bestTargets[0].normalized_x; 
}

void VisionSystem::findTarget(double& offset, double& distance, int& targetLevel)
{
    targetLevel = 0;
    distance = 0.0;
    offset = 0.0;
}

