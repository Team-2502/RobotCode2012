#ifndef VISION_H
#define VISION_H

#include "WPILib.h"

struct TargetReport {
	double width;
	double height;
	double x;
	double y;
	double size;
	double normalized_x; //units of Joystick plane
	double normalized_y; //units of Joystick plane
	double distance; //ft
	bool operator<(TargetReport &rhs) {
		return size < rhs.size;
	}
};

class VisionSpecifics
{
public:
	virtual ~VisionSpecifics() {}
	virtual TargetReport* getBestTargets(HSLImage * img, int& count) = 0;
};

class VisionSystem
{
public:
	VisionSystem(VisionSpecifics *backend);
	~VisionSystem();
	
	void start();
	void stop();
	
	TargetReport* getBestTargets() const {return bestTargets;}
    int getBestTargetCount() const {return bestTargetCount;}
	double getTargetHeadingX();
	
private:
	static void loop();
	Task *visionTask;
	
    static int bestTargetCount;
	static TargetReport bestTargets[4];
	static AxisCamera *cam;
	static VisionSpecifics *engine;
};

#endif

