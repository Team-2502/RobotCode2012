#include <WPILib.h>
#include "nivision.h"
#include <cmath>

#include "Constants.h"
#include "Display.h"
#include "Singleton.h"
#include "Vision.h"

AxisCamera *Vision::cam = NULL;
VisionSpecifics *Vision::engine = NULL;
int Vision::bestTargetCount = 0;
TargetReport Vision::bestTargets[] = {0, 0, 0, 0};

static double GetDistanceFromHeight(double realHeight, double imageHeight)
{
	return (realHeight / imageHeight) * CAMERA_FOCAL_LENGTH;
}

Vision::Vision(VisionSpecifics *backend)
{
	engine = backend;
	cam = &AxisCamera::GetInstance("10.25.2.11");
	cam->WriteResolution(AxisCamera::kResolution_320x240);

	visionTask = new Task("2502Vn",(FUNCPTR)loop);
}

Vision::~Vision()
{
	delete engine;
	visionTask->Stop();
	delete visionTask;
}

void Vision::start()
{
	visionTask->Start();
}

void Vision::stop()
{
	visionTask->Stop();
}

void Vision::loop()
{
	while(true)
	{
		HSLImage* cap = new HSLImage;
		cam->GetImage(cap);
		engine->GetBestTargets(cap, bestTargets, bestTargetCount);
		delete cap;
		Wait(0.01);
	}
}

bool* Vision::GetTargetCase(TargetReport* targets, int targetCount)
{
	return NULL;
}

void Vision::FindTarget(double& offset, double& distance, int& targetLevel)
{
	targetLevel = 0;
	distance = 0.0;
	offset = 0.0;
	
	if( bestTargetCount == 0 )
		return;
	bool* targetCase = GetTargetCase(bestTargets, bestTargetCount);
	
	//Handle target cases
	if( targetCase[0] && targetCase[3] )
	{
		//Top / Bottom (best case scenario)
		offset = bestTargets[0].normalized_x;
		targetLevel = 3;
		
		double realHeight = BASKET_3_ELEVATION - BASKET_1_ELEVATION;
		distance = GetDistanceFromHeight(realHeight, bestTargets[0].normalizedHeight);
	}
	else if( targetCase[0] && (targetCase[1] || targetCase[2]) )
	{
		//One of the top diagonals
		offset = bestTargets[0].normalized_x;
		targetLevel = 3;
		
		double realHeight = BASKET_3_ELEVATION - BASKET_2_ELEVATION;
		distance = GetDistanceFromHeight(realHeight, bestTargets[0].normalizedHeight);
	}
	else if( targetCase[3] && (targetCase[1] || targetCase[2]) )
	{
		//One of the bottom diagonals
		offset = bestTargets[3].normalized_x;
		targetLevel = 1;
		
		double realHeight = BASKET_2_ELEVATION - BASKET_1_ELEVATION;
		distance = GetDistanceFromHeight(realHeight, bestTargets[3].normalizedHeight);
	}
	
	delete [] targetCase;
}
