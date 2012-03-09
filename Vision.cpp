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

static bool compareTargetReportY(TargetReport a, TargetReport b) { return (a.y < b.y); }
static bool compareTargetReportX(TargetReport a, TargetReport b) { return (a.x < b.x); }

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
//		Singleton<Display>::GetInstance().PrintfLine(1, "Vis:(%f,%f)", bestTargets[0].normalized_x, bestTargets[0].normalized_y);
//		Singleton<Display>::GetInstance().PrintfLine(2, "#:%d,Dist:%f", bestTargetCount, bestTargets[0].distance);
		delete cap;
		Wait(0.01);
	}
}

bool* Vision::GetTargetCase(TargetReport* targets, int targetCount)
{
	vector <TargetReport> ordered;
	for (int i = 0; i < targetCount; i++) ordered.push_back(targets[i]);
	
	sort(ordered.begin(), ordered.end(), compareTargetReportY);
	bool *ret = new bool[4];
	ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0;
	double last_dif = 0;
	double middle_y = 0;
	double middle_x = 0;
	for (unsigned int i = 1; i < ordered.size(); i++) {
		last_dif = ordered[i].y - ordered[i-1].y;
		if (last_dif <= ordered[i].height * .2 && last_dif >= 0-ordered[i].height * .2) {
			ret[1] = 1;
			ret[2] = 1;
			if (ordered[i].x < ordered[i-1].x) {
				targets[1] = ordered[i];
				targets[2] = ordered[i-1];
			} else {
				targets[2] = ordered[i];
				targets[1] = ordered[i-1];
			}
			middle_y = ordered[i].y;
		}
	}
	sort(ordered.begin(), ordered.end(), compareTargetReportX);
	for (unsigned int i = 1; i < ordered.size(); i++) {
		last_dif = ordered[i].x - ordered[i-1].x;
		if (last_dif <= ordered[i].width * .1 && last_dif >= 0-ordered[i].width * .1) {
			ret[0] = 1;
			ret[3] = 1;
			if (ordered[i].y < ordered[i-1].y) {
				targets[0] = ordered[i];
				targets[3] = ordered[i-1];
			} else {
				targets[3] = ordered[i];
				targets[0] = ordered[i-1];
			}
			middle_x = ordered[i].x;
		}
	}
	
	for (unsigned int i = 0; i < ordered.size(); i++) {
		if (middle_y != 0) {
			if (ordered[i].y < middle_y
			 && ordered[i].x >= (targets[1].x + targets[2].x) / 2 - ordered[i].width * .1
			 && ordered[i].x <= (targets[1].x + targets[2].x) / 2 + ordered[i].width * .1){
				ret[0] = 1;
				targets[0] = ordered[i];
			}
			if (ordered[i].y > middle_y
			 && ordered[i].x >= (targets[1].x + targets[2].x) / 2 - ordered[i].width * .1
			 && ordered[i].x <= (targets[1].x + targets[2].x) / 2 + ordered[i].width * .1){
				ret[3] = 1;
				targets[3] = ordered[i];
			}
		}
		if (middle_x != 0) {
			if (ordered[i].x > middle_x 
			 && ordered[i].y >= (targets[0].y + targets[3].y) / 2 - ordered[i].height * .2
			 && ordered[i].y <= (targets[0].y + targets[3].y) / 2 + ordered[i].height * .2 ){
				ret[2] = 1;
				targets[2] = ordered[i];
			}
			if (ordered[i].x < middle_x 
			 && ordered[i].y >= (targets[0].y + targets[3].y) / 2 - ordered[i].height * .2
			 && ordered[i].y <= (targets[0].y + targets[3].y) / 2 + ordered[i].height * .2 ){
				ret[1] = 1;
				targets[1] = ordered[i];
			}
		}
	}
	return ret;
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
	if( targetCase[TOP_TARGET] && targetCase[BOTTOM_TARGET] )
	{
		//Top / Bottom (best case scenario)
		offset = bestTargets[0].normalized_x;
		targetLevel = 3;
		
		double realHeight = BASKET_3_ELEVATION - BASKET_1_ELEVATION;
		distance = GetDistanceFromHeight(realHeight, bestTargets[0].normalizedHeight);
	}
	else if( targetCase[TOP_TARGET] && (targetCase[LEFT_TARGET] || targetCase[RIGHT_TARGET]) )
	{
		//One of the top diagonals
		offset = bestTargets[0].normalized_x;
		targetLevel = 3;
		
		double realHeight = BASKET_3_ELEVATION - BASKET_2_ELEVATION;
		distance = GetDistanceFromHeight(realHeight, bestTargets[0].normalizedHeight);
	}
	else if( targetCase[BOTTOM_TARGET] && (targetCase[LEFT_TARGET] || targetCase[RIGHT_TARGET]) )
	{
		//One of the bottom diagonals
		offset = bestTargets[3].normalized_x;
		targetLevel = 1;
		
		double realHeight = BASKET_2_ELEVATION - BASKET_1_ELEVATION;
		distance = GetDistanceFromHeight(realHeight, bestTargets[3].normalizedHeight);
	}
	
	delete [] targetCase;
}
