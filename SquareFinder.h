#ifndef SQUAREFINDER_H
#define SQUAREFINDER_H

#include "vision.h"

class SquareFinder : public VisionSpecifics
{
public:
	SquareFinder();
	~SquareFinder();
	
	void GetBestTargets(HSLImage *img, TargetReport* targets, int& count);
	
private:
	Image* lumPlane;
};

#endif
