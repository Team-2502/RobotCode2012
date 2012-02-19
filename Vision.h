#ifndef VISION_H
#define VISION_H

#include "WPILib.h"

struct TargetReport 
{
	double width;
	double height;
	double x;
	double y;
	double size;
	double normalized_x; //units of Joystick plane
	double normalized_y; //units of Joystick plane
	double normalizedWidth;
	double normalizedHeight;
	double distance; //ft
	bool operator<(TargetReport &rhs) {return size < rhs.size;}
};

class VisionSpecifics
{
public:
	virtual ~VisionSpecifics() {}
	virtual void GetBestTargets(HSLImage * img, TargetReport* targets, int& count) = 0;
};

class Vision
{
public:
	Vision(VisionSpecifics *backend);
	~Vision();
	
	void start();
	void stop();
	
    /**
     * Find the best target.
     *
     * \param offset the relative offset to the left or right of the camera.
     * \param distance the distance to the target.
     * \param targetLevel the height level of the target.
     */
    void FindTarget(double& offset, double& distance, int& targetLevel);
	
private:
	static void loop();
    bool* GetTargetCase(TargetReport* targets, int targetCount);
	
	Task* visionTask;
	
    static int bestTargetCount;
	static TargetReport bestTargets[4];
	static AxisCamera* cam;
	static VisionSpecifics* engine;
};

#endif

