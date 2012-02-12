#ifndef SQUAREFINDER_H
#define SQUAREFINDER_H

#include "vision.h"

class SquareFinder : public VisionSpecifics
{
public:
	SquareFinder();
	~SquareFinder();
	
	TargetReport getBestTarget(HSLImage *img);
	
private:
	Image *lumPlane;
};

#endif
